#include <projdefs.h>

#define portINITIAL_XPSR (0x01000000)
#define portSTART_ADDRESS_MASK ((StackType_t)0xfffffffeUL)

/*内核外设SCB中的SHPR3寄存器用于设置SysTick和PendSV的异常优先级*/
#define portNVIC_SYSPRI2_REG (*((volatile uint32_t *)0xE000ED20))

#define portNVIC_PENDSV_PRI (((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 16UL)
#define portNVIC_SYSTICK_PRI (((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 24UL)

#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
#define xPortSVCHandler SVC_Handler

/**
 * @brief 任务的返回后前往执行的函数(因为我们现在写的任务都是无限循环，一般不会返回，所用用这个退出错误函数替代返回)
 * @return void
 */
static void prvTaskExitError(void);

/**
 * @brief 初始化任务栈，并更新栈顶指针
 * @param pxTopOfStack 栈顶地址
 * @param pxCode 任务函数
 * @param pvParameters 任务形参
 * @return 返回新的栈顶的地址
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack,
                                   TaskFunction_t pxCode,
                                   void *pvParameters);

/**
 * @brief 调度器
 */
BaseType_t xPortStartScheduler(void);

/**
 * @brief 更新MSP的值，并产生SVC系统调用
 */
void prvStartFirstTask(void);

/**
 * @brief SVC中断处理函数
 */
void vPortSVCHandler(void);