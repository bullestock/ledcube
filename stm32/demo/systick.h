/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file libmaple/include/libmaple/systick.h
 * @brief System timer definitions
 */

#ifndef _LIBMAPLE_SYSTICK_H_
#define _LIBMAPLE_SYSTICK_H_

#include <stm32f10x.h>

#ifdef __cplusplus
extern "C"{
#endif

#define __io volatile
    
#include "util.h"

/** SysTick register map type */
typedef struct systick_reg_map {
    __io uint32_t CSR;            /**< Control and status register */
    __io uint32_t RVR;            /**< Reload value register */
    __io uint32_t CNT;            /**< Current value register ("count") */
    __io uint32_t CVR;            /**< Calibration value register */
} systick_reg_map;

/** SysTick register map base pointer */
#define SYSTICK_BASE                    ((struct systick_reg_map*)0xE000E010)

/*
 * Register bit definitions.
 */

/* Control and status register */

#define SYSTICK_CSR_COUNTFLAG           BIT(16)
#define SYSTICK_CSR_CLKSOURCE           BIT(2)
#define SYSTICK_CSR_CLKSOURCE_EXTERNAL  0
#define SYSTICK_CSR_CLKSOURCE_CORE      BIT(2)
#define SYSTICK_CSR_TICKINT             BIT(1)
#define SYSTICK_CSR_TICKINT_PEND        BIT(1)
#define SYSTICK_CSR_TICKINT_NO_PEND     0
#define SYSTICK_CSR_ENABLE              BIT(0)
#define SYSTICK_CSR_ENABLE_MULTISHOT    BIT(0)
#define SYSTICK_CSR_ENABLE_DISABLED     0

/* Calibration value register */

#define SYSTICK_CVR_NOREF               BIT(31)
#define SYSTICK_CVR_SKEW                BIT(30)
#define SYSTICK_CVR_TENMS               0xFFFFFF

/** System elapsed time, in milliseconds */
extern volatile uint32_t systick_uptime_millis;

/**
 * @brief Returns the system uptime, in milliseconds.
 */
static inline uint32_t systick_uptime(void) {
    return systick_uptime_millis;
}


void systick_init(uint32_t reload_val);
void systick_disable();
void systick_enable();

/**
 * @brief Returns the current value of the SysTick counter.
 */
static inline uint32_t systick_get_count(void) {
    return SYSTICK_BASE->CNT;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif
