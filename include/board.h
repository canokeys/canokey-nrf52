/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

/** \ingroup group_demo
 * \defgroup group_board Boards Abstraction Layer
 *  @{ */

#ifndef _BSP_BOARD_H_
#define _BSP_BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "nrfx_timer.h"

#define CFG_BOARD_UART_BAUDRATE 115200

// Initialize on-board peripherals : led, button, uart and USB
void board_init(void);

// Write data to UART
int board_uart_write(void const *buf, int len);

// Ticks
extern volatile uint32_t system_ticks;

// Timers
extern nrfx_timer_t m_timer_touch;
extern nrfx_timer_t m_timer_timeout;

// Timer handlers
void timer_touch_handler(nrf_timer_event_t event_type, void * p_context);
void timer_timeout_handler(nrf_timer_event_t event_type, void * p_context);

// RNG event handler
extern volatile uint32_t rng_data_word, rng_count;
void rng_event_handler(uint8_t rng_data);

#ifdef __cplusplus
}
#endif

#endif /* _BSP_BOARD_H_ */

/** @} */
