#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_16_BIT_TICKS 0
#define configMAX_TASK_NAME_LEN 16 /* 任务名字最长长度 */
#define configSUPPORT_STATIC_ALLOCATION 1
#define configMAX_PRIORITIES 256 /* 优先级最大个数 */
#define configKERNEL_INTERRUPT_PRIORITY 1

#endif