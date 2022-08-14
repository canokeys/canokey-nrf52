#include <board.h>
#include <device.h>
#include <globals.h>
#include <tusb.h>

#include <nrf_gpio.h>
#include <nrfx.h>
#include <nrfx_timer.h>
#include <nrfx_uarte.h>
#include <nrfx_rng.h>

#include <boards.h>

uint32_t device_get_tick() {
    return system_ticks;
}

void device_delay(int ms) {
    uint32_t start_ms = device_get_tick();
    while(device_get_tick() - start_ms < ms) {
        // Wait
        tud_task();
    }
}

void led_on() {
    bsp_board_led_on(BSP_BOARD_LED_0);
}

void led_off() {
    bsp_board_led_off(BSP_BOARD_LED_0);
}

static void (*tim_callback)(void);

// every 10ms
void timer_touch_handler(nrf_timer_event_t event_type, void *p_context) {
    static uint32_t touch_timeout = 1;
    device_update_led();
    if(has_touch) {
        if(bsp_board_button_state_get(BSP_BOARD_BUTTON_0)) {
            uint32_t tick = device_get_tick();
            if(tick < touch_timeout + 1000) {
            } else {
                set_touch_result(TOUCH_SHORT);
                touch_timeout = tick + 200;
            }
        } else if(device_get_tick() > touch_timeout)
            set_touch_result(TOUCH_NO);
    } else {
        set_touch_result(TOUCH_SHORT);
    }
}

void device_set_timeout(void (*callback)(void), uint16_t timeout) {
    if(timeout == 0) {
        nrfx_timer_disable(&m_timer_timeout);
        return;
    }

    tim_callback = callback;
    nrfx_timer_compare(&m_timer_timeout, 0, nrfx_timer_ms_to_ticks(&m_timer_timeout, timeout), true);
    nrfx_timer_enable(&m_timer_timeout);
}

void timer_timeout_handler(nrf_timer_event_t event_type, void *p_context) {
    if(tim_callback) {
        tim_callback();
    }
}

uint32_t random32(void) {
    nrfx_rng_start();

    // rng_count is incremented in irq handler
    while(rng_count <= 3);
    nrfx_rng_stop();

    uint32_t data = rng_data_word;
    rng_data_word = 0;
    rng_count = 0;

    return data;
}
