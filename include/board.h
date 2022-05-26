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

#include "tusb.h"

#define CFG_BOARD_UART_BAUDRATE 115200

//--------------------------------------------------------------------+
// Board selection
// Select your board in CMakeLists.txt
//--------------------------------------------------------------------+

#define OPT_BOARD_MAKERDIARY_M2_DOCK    1

#if CFG_BOARD == OPT_BOARD_MAKERDIARY_M2_DOCK
    #include "boards/makerdiary/m2_dock.h"
#else
    #error "No board selected, please select your board in CMakeLists.txt"
#endif

//--------------------------------------------------------------------+
// CanoKey Porting API
//--------------------------------------------------------------------+

/**
 * Get a spinlock.
 *
 * @param lock      The lock handler, which should be pointed to a uint32_t variable.
 * @param blocking  If we should wait the lock to be released.
 *
 * @return 0 for locking successfully, -1 for failure.
 */
int device_spinlock_lock(volatile uint32_t *lock, uint32_t blocking);

/**
 * Unlock the specific handler.
 *
 * @param lock  The lock handler.
 */
void device_spinlock_unlock(volatile uint32_t *lock);

/**
 * Update the value of a variable atomically.
 *
 * @param var    The address of variable to update.
 * @param expect The current value of variable.
 * @param var    The new value of variable.
 */
int device_atomic_compare_and_swap(volatile uint32_t *var, uint32_t expect, uint32_t update);

void led_on(void);
void led_off(void);
void device_set_timeout(void (*callback)(void), uint16_t timeout);

//--------------------------------------------------------------------+
// Board Porting API
// For simplicity, only one LED and one Button are used
//--------------------------------------------------------------------+

// Initialize on-board peripherals : led, button, uart and USB
void board_init(void);

// Turn LED on or off
void board_led_write(bool state);

// Control led pattern using phase duration in ms.
// For each phase, LED is toggle then repeated, board_led_task() is required to be called
// void board_led_pattern(uint32_t const phase_ms[], uint8_t count);

// Get the current state of button
// a '1' means active (pressed), a '0' means inactive.
uint32_t board_button_read(void);

// Get characters from UART
int board_uart_read(uint8_t *buf, int len);

// Send characters to UART
int board_uart_write(void const *buf, int len);

// Get current milliseconds, must be implemented when no RTOS is used
uint32_t board_millis(void);

//--------------------------------------------------------------------+
// Helper functions
//--------------------------------------------------------------------+
static inline void board_led_on(void) {
    board_led_write(true);
}

static inline void board_led_off(void) {
    board_led_write(false);
}

// TODO remove
static inline void board_delay(uint32_t ms) {
    uint32_t start_ms = board_millis();
    while(board_millis() - start_ms < ms) {
#if CFG_TUD_ENABLED
        // take chance to run usb background
        tud_task();
#endif
    }
}

static inline int board_uart_getchar(void) {
    uint8_t c;
    return board_uart_read(&c, 1) ? (int)c : (-1);
}

static inline int board_uart_putchar(uint8_t c) {
    return board_uart_write(&c, 1);
}

#ifdef __cplusplus
}
#endif

#endif /* _BSP_BOARD_H_ */

/** @} */
