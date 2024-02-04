#include <task.h>

List_t pxReadyTasksLists[configMAX_PRIORITIES];
TCB_t *pxCurrentTCB = NULL;

extern TCB_t Task1TCB;

#if (configSUPPORT_STATIC_ALLOCATION == 1)

/* 任务就绪列表 */

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                               const char *const pcName,
                               const uint32_t ulStackDepth,
                               void *const pvParameters,
                               StackType_t *const puxStackBuffer,
                               TCB_t *const pxTaskBuffer)
{
    TCB_t *pxNewTCB;
    TaskHandle_t xReturn; /* 任务句柄，指向任务的TCB */

    if ((pxTaskBuffer != NULL) && (puxStackBuffer != NULL))
    {
        pxNewTCB = (TCB_t *)pxTaskBuffer;
        pxNewTCB->pxStack = (StackType_t *)puxStackBuffer;

        /* 创建新的任务 */
        prvInitialiseNewTask(pxTaskCode,   /* 任务入口 */
                             pcName,       /* 任务名称，字符串形式 */
                             ulStackDepth, /* 任务栈大小，单位为字 */
                             pvParameters, /* 任务形参 */
                             &xReturn,     /* 任务句柄 */
                             pxNewTCB);    /* 任务栈起始地址 */
    }
    else
    {
        xReturn = NULL;
    }

    /* 返回任务句柄，如果任务创建成功，此时xReturn应该指向任务控制块 */
    return xReturn;
}

static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,
                                 const char *const pcName,
                                 const uint32_t ulStackDepth,
                                 void *const pvParameters,
                                 TaskHandle_t *const pxCreatedTask,
                                 TCB_t *pxNewTCB)
{
    StackType_t *pxTopOfStack;
    UBaseType_t x;

    /* 获取栈顶地址 */
    pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - (uint32_t)1);
    /* ~0x0007是为了做8字节对齐 */
    pxTopOfStack = (StackType_t *)(((uint32_t)pxTopOfStack) & (~((uint32_t)0x0007)));

    /* 将任务名称存储在TCB中 */
    for (x = (UBaseType_t)0; x < (UBaseType_t)configMAX_TASK_NAME_LEN; x++)
    {
        pxNewTCB->pcTaskName[x] = pcName[x];

        if (pcName[x] == 0x00)
            break;
    }
    /* 任务名的长度不能超过configMAX_TASK_NAME_LEN */

    pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN - 1] = '\0';

    /* 初始化TCB中的xStateListItem节点 */
    listSET_LIST_ITEM_OWNER(&(pxNewTCB->xStateListItem), pxNewTCB);

    /* 初始化任务栈 */
    pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack, pxTaskCode, pvParameters);

    /* 让任务句柄指向任务控制块 */
    if ((void *)pxCreatedTask != NULL)
    {
        *pxCreatedTask = (TaskFunction_t)pxNewTCB; // 实际上就是tcb的首地址
    }
}

void prvInitialiseTaskLists(void)
{
    UBaseType_t uxPriority;

    for (uxPriority = (UBaseType_t)0U; uxPriority < (UBaseType_t)configMAX_PRIORITIES; uxPriority++)
    {
        vListInitialise(&(pxReadyTasksLists[uxPriority]));
    }
}

void vTaskStartScheduler(void)
{
    pxCurrentTCB = &Task1TCB;

    if (xPortStartScheduler() != pdFALSE)
        ;
}

#endif /* configSUPPORT_STATIC_ALLOCATION */