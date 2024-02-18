#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <stdint.h>
#include <stddef.h>
#include <FreeRTOSConfig.h>

#define portCHAR char
#define portFLOAT float
#define portDOUBLE double
#define portLONG long
#define portSHORT short
#define portSTACK_TYPE uint32_t
#define portBASE_TYPE long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if (configUSE_16_BIT_TICKS == 1)
typedef uint16_t TickType_t;
#define portMAX_DELAY (TickType_t)0xffff16
#else
typedef uint32_t TickType_t;
#define portMAX_DELAY (TickType_t)0xffffffffUL
#endif

#define portNVIC_INT_CTRL_REG (*((volatile uint32_t *)0xe000ed04))
#define portNVIC_PENDSVSET_BIT (1UL << 28UL)

#define protSY_FULL_READ_WRITE (15)

/* 触发PendSV, 产生上下文切换 */
#define portYIELD()                                     \
    {                                                   \
        portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
        __dsb(protSY_FULL_READ_WRITE);                  \
        __isb(protSY_FULL_READ_WRITE);                  \
    }

#endif /* PORTMACRO_H */
