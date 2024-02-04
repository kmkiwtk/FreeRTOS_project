#include <list.h>
#include <projdefs.h>
#include <port.h>

#define taskYIELD() portYIELD()

/**
 * @brief 任务控制块
 */
typedef struct tskTaskControlBlock
{
    volatile StackType_t *pxTopOfStack; /* 栈顶 */

    ListItem_t xStateListItem; /* 任务节点 */

    StackType_t *pxStack; /* 任务栈起始地址 */

    char pcTaskName[configMAX_TASK_NAME_LEN]; /* 任务名称，为字符串形式 */
} tskTCB;

typedef tskTCB TCB_t;

typedef void *TaskHandle_t;

#if (configSUPPORT_STATIC_ALLOCATION == 1)

/**
 * @brief 静态创建任务TCB
 * @param pxTaskCode 任务入口
 * @param pcName 任务名称
 * @param ulStackDepth 任务栈大小，单位为字
 * @param pvParameters 任务形参
 * @param puxStackBuffer 任务栈起始地址
 * @param pxTaskBuffer 任务控制块指针
 * @return 返回TCB的地址
 */
TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                               const char *const pcName,
                               const uint32_t ulStackDepth,
                               void *const pvParameters,
                               StackType_t *const puxStackBuffer,
                               TCB_t *const pxTaskBuffer);

/**
 * @brief 创建新任务，其实就是将任务函数放入任务控制块中，根据传入任务控制块的信息初始化栈空间，主体是TCB
 * @param pxTaskCode 任务入口，即任务函数的地址
 * @param pcName 任务名称
 * @param ulStackDeptha 任务栈大小，即缓存空间的大小，单位为字(32bit/4byte)
 * @param pvParameters 任务形参数
 * @param pxCreatedTask 任务句柄，即要使用的TCB的首地址
 * @param pxNewTCB 栈顶地址，即用来保存tcb的内存空间首地址
 * @return void
 */
static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,
                                 const char *const pcName,
                                 const uint32_t ulStackDepth,
                                 void *const pvParameters,
                                 TaskHandle_t *const pxCreatedTask,
                                 TCB_t *pxNewTCB);

/**
 * @brief 就绪列表初始化
 * @param void
 * @return void
 */
void prvInitialiseTaskLists(void);

/**
 * @brief 启动调度器
 * @param void
 * @return void
 */
void vTaskStartScheduler(void);

#endif /* configSUPPORT_STATIC_ALLOCATION */