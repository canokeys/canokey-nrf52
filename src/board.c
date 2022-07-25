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

#include <board.h>
#include <common.h>

#include "nrf_gpio.h"
#include "nrfx.h"
#include "nrfx_power.h"
#include "nrfx_timer.h"
#include "nrfx_uarte.h"
#include "nrfx_rng.h"

uint32_t rng_count = 0;
uint32_t rng_data_word = 0;

//--------------------------------------------------------------------+
// Forward USB interrupt events to TinyUSB IRQ Handler
//--------------------------------------------------------------------+
void USBD_IRQHandler(void) {
    tud_int_handler(0);
}

/*------------------------------------------------------------------*/
/* MACRO TYPEDEF CONSTANT ENUM
 *------------------------------------------------------------------*/
static nrfx_uarte_t _uart_id = NRFX_UARTE_INSTANCE(0);
nrfx_timer_t m_timer_touch = NRFX_TIMER_INSTANCE(0);
nrfx_timer_t m_timer_timeout = NRFX_TIMER_INSTANCE(1);

// tinyusb function that handles power event (detected, ready, removed)
// We must call it within SD's SOC event handler, or set it as power event handler if SD is not enabled.
extern void tusb_hal_nrf_power_event(uint32_t event);

// nrf power callback, could be unused if SD is enabled or usb is disabled (board_test example)
TU_ATTR_UNUSED static void power_event_handler(nrfx_power_usb_evt_t event) {
    tusb_hal_nrf_power_event((uint32_t)event);
}

void board_init(void) {
    // stop LF clock just in case we jump from application without reset
    NRF_CLOCK->TASKS_LFCLKSTOP = 1UL;

    // Use Internal OSC to compatible with all boards
    NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_RC;
    NRF_CLOCK->TASKS_LFCLKSTART = 1UL;

    // LED
    nrf_gpio_cfg_output(BSP_LED_0);
    board_led_write(false);

    // Button
    nrf_gpio_cfg_input(BSP_BUTTON_0, BUTTON_PULL);

    // 1ms tick timer
    SysTick_Config(SystemCoreClock / 1000);

    // UART
    nrfx_uarte_config_t uart_cfg =
        {
            .pseltxd = TX_PIN_NUMBER,
            .pselrxd = RX_PIN_NUMBER,
            .pselcts = CTS_PIN_NUMBER,
            .pselrts = RTS_PIN_NUMBER,
            .p_context = NULL,
            .baudrate = NRF_UARTE_BAUDRATE_115200, // CFG_BOARD_UART_BAUDRATE
            .interrupt_priority = 7,
            .hwfc = HWFC ? NRF_UARTE_HWFC_ENABLED : NRF_UARTE_HWFC_DISABLED,
            .parity = NRF_UARTE_PARITY_EXCLUDED,
        };

    nrfx_uarte_init(&_uart_id, &uart_cfg, NULL); // uart_handler);

    // Timer
    nrfx_timer_config_t timer_cfg = NRFX_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;

    // Touch timer is executed every 10ms
    nrfx_err_t ret;
    ret = nrfx_timer_init(&m_timer_touch, &timer_cfg, timer_touch_handler);
    if(ret != NRFX_SUCCESS) ERR_MSG("touch nrfx_timer_init failed: %d\r\n", ret);

    nrfx_timer_extended_compare(&m_timer_touch, NRF_TIMER_CC_CHANNEL0, nrfx_timer_ms_to_ticks(&m_timer_touch, 10), NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrfx_timer_enable(&m_timer_touch);

    // Timeout timer oneshot, configured in device_set_timeout()
    ret = nrfx_timer_init(&m_timer_timeout, &timer_cfg, timer_timeout_handler);
    if(ret != NRFX_SUCCESS) ERR_MSG("timeout nrfx_timer_init failed: %d\r\n", ret);

    //------------- USB -------------//
    // Priorities 0, 1, 4 (nRF52) are reserved for SoftDevice
    // 2 is highest for application
    NVIC_SetPriority(USBD_IRQn, 2);

    // USB power may already be ready at this time -> no event generated
    // We need to invoke the handler based on the status initially
    uint32_t usb_reg;

    // Power module init
    const nrfx_power_config_t pwr_cfg = {0};
    nrfx_power_init(&pwr_cfg);

    // Register tusb function as USB power handler
    // cause cast-function-type warning
    const nrfx_power_usbevt_config_t pwr_evt_cfg = {.handler = power_event_handler};
    nrfx_power_usbevt_init(&pwr_evt_cfg);

    nrfx_power_usbevt_enable();

    usb_reg = NRF_POWER->USBREGSTATUS;

    if(usb_reg & POWER_USBREGSTATUS_VBUSDETECT_Msk) tusb_hal_nrf_power_event(NRFX_POWER_USB_EVT_DETECTED);
    if(usb_reg & POWER_USBREGSTATUS_OUTPUTRDY_Msk) tusb_hal_nrf_power_event(NRFX_POWER_USB_EVT_READY);

    // RNG module init
    nrfx_rng_config_t rng_cfg = NRFX_RNG_DEFAULT_CONFIG;
    nrfx_rng_init(&rng_cfg, rng_event_handler);

    rng_count = 0;
}

// RNG handler
void rng_event_handler(uint8_t rng_data) {
    if(rng_count <= 3) {
        rng_data_word <<= 8;
        rng_data_word |= rng_data;
        rng_count++;
    }
}

//--------------------------------------------------------------------+
// Board porting API
//--------------------------------------------------------------------+

void board_led_write(bool state) {
    nrf_gpio_pin_write(BSP_LED_0, state ? LEDS_ACTIVE_STATE : 1 - LEDS_ACTIVE_STATE);
}

bool board_button_read(void) {
    return nrf_gpio_pin_read(BSP_BUTTON_0) == BUTTONS_ACTIVE_STATE;
}

volatile uint32_t system_ticks = 0;
void SysTick_Handler(void) {
    system_ticks++;
}

int board_uart_write(void const *buf, int len) {
    return (NRFX_SUCCESS == nrfx_uarte_tx(&_uart_id, (uint8_t const *)buf, (size_t)len)) ? len : 0;
}

// printf to UART
int _write(int fd, char *str, int len) {
    board_uart_write(str, len);
    return len;
}

int device_atomic_compare_and_swap(volatile uint32_t *var, uint32_t expect, uint32_t update) {
    if(*var == expect) {
        *var = update;
        return 0;
    } else {
        return -1;
    }
}

int device_spinlock_lock(volatile uint32_t *lock, uint32_t blocking) {
    // Not really working, for test only
    while(*lock) {
        if(!blocking) return -1;
    }
    *lock = 1;
    return 0;
}
void device_spinlock_unlock(volatile uint32_t *lock) { *lock = 0; }
