#include "port.h"

static void prvTaskExitError(void)
{
    /* 函数停止在这里*/
    for (;;)
        ;
}

StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack,
                                   TaskFunction_t pxCode,
                                   void *pvParameters)
{
    /* 异常发生时，自动加载到CPU寄存器的内容 */
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;
    pxTopOfStack--;
    *pxTopOfStack = ((StackType_t)pxCode) & portSTART_ADDRESS_MASK;
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t)prvTaskExitError;
    pxTopOfStack -= 5;
    *pxTopOfStack = (StackType_t)pvParameters;

    /* 异常发生时，手动加载到CPU寄存器的内容 */
    pxTopOfStack -= 8;

    /* 返回栈顶指针，此时pxTopOfStack指向空闲栈 */
    return pxTopOfStack;
}

BaseType_t xPortStartScheduler(void)
{
    /* 配置PenSV和SysTick的终端优先级为最低 */
    portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;

    /* 启动第一个任务，不再返回 */
    prvStartFirstTask();

    /* 不应该运行到这里 */
    return 0;
}

void prvStartFirstTask(void)
{
    /* 使栈8字节对齐 */

    __asm__(
        /* 在Cortex-M中，0xE000ED08是SCB_VTOR这个寄存器的地址，里面存放的是msp的起始地址，msp的首地址通常存的是向量表的首地址 */
        "PRESERVE8"
        "ldr r0, =0xE000ED08"
        "ldr r0, [r0]" // 获取的是msp的起始地址，也是向量表第一个向量所在的地址
        "ldr r0, [r0]" // 向量结构第一个量为栈顶指针的初始值

        /* 设置主堆栈栈顶指针，即设置svc的栈为当前栈 */
        "msr msp, r0"

        /* 打开中断和异常 */
        "cpsie i"
        "cpsie f"
        "dsb" // 确保数据同步
        "isb" // 确保指令同步

        /* 调用svc中断函数 */
        "svc 0"
        "nop"
        "nop");
}

void vPortSVCHandler(void)
{
    extern pxCurrentTCB;
    __asm__(
        "PRESERVE8"

        "ldr r3, =pxCurrentTCB" // TCB_t *，即获取的是指针的地址
        "ldr r1, [r3]"          // 获取的是指针的内容，即当前任务块的地址
        "ldr r0, [r1]"          // 获取的是任务块内容，即第一个成员
        "ldmia r0!, {r4-r11}"   // 将栈中保存的r4-r11的值放入寄存器中
        "msr psp, r0"           // 将新的栈顶存入psp栈指针中
        "isb"                   // 指令同步
        "mov r0, #0"            // r0寄存器清0
        "msr basepri, r0"       // 打开中断
        "msr r14, #0xd"         // 设置返回模式

        "bx r14"); // 异常返回，同时cpu会自动加载栈中的值：xpsr,pc,r14, r12, r3, r2, r1, r0，这也是为什么svc中断函数中要手动加载r4-r11的值到寄存器，此时psp会回到栈顶
}

void xPortPendSVHandle(void)
{
    extern pxCurrent;
    extern vTaskSwitchContext;

    ___asm__(
        "PRESERVE8"

        "mrs r0, psp" // 在保存玩自动保存的内容后，psp位于r0在栈中的位置
        "isb"
        "ldr r3, =pxCurrentTCB"
        "ldr r2, [r3]"

        /* 此处为手动需要保存的寄存器的值到栈中，在进入异常时，一部分寄存器会自动保存入栈中 */
        "stmdb r0!, {r4-r11}"
        "str r0, [r2]" // 在保存完手动保存的部份后，更新TCB中的TopOfStack为正确的值

        "stmdb sp!, {r3,r14}" // 因为要调用其他函数，所以将r3(pxCurrentTCB的地址)和r14(返回地址)压入主栈中
        "mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY"
        "msr basepri, r0"
        "dsb"
        "isb"
        "bl vTaskSwitchContext"
        "mov r0, #0"
        "msr basepri, r0"
        "ldmia sp!, {r3,r14}"

        "ldr r1, [r3]"
        "ldr r0, [r1]"
        "ldmia r0!, {r4,r11}"
        "msr psp, r0"
        "isb"
        "bx r14"

        "nop");
}