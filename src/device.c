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

const uint32_t TOUCH_GAP_TIME = 800; /* Gap period (in ms) between two consecutive touch events */
const uint32_t MIN_LONG_TOUCH_TIME = 500;
const uint32_t MIN_TOUCH_TIME = 20;


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

inline static bool btn_down() {
    return bsp_board_button_state_get(BSP_BOARD_BUTTON_0);
}

static void (*tim_callback)(void);

// every 10ms
void timer_touch_handler(nrf_timer_event_t event_type, void *p_context) {
  enum {
    TOUCH_STATE_IDLE,
    TOUCH_STATE_DOWN,
    TOUCH_STATE_ASSERT,
    TOUCH_STATE_DEASSERT,
  };
  static uint32_t event_tick, fsm = TOUCH_STATE_IDLE;
  uint32_t tick = device_get_tick();
  switch (fsm)
  {
  case TOUCH_STATE_IDLE:
#ifdef DEBUG_OUTPUT
    // Emulate touch events with UART input
    // if (LL_USART_IsActiveFlag_RXNE(DBG_UART.Instance)) {
    //   int data = LL_USART_ReceiveData8(DBG_UART.Instance);
    //   DBG_MSG("UART: %x\n", data);
    //   if ('T' == data || 'L' == data) {
    //     set_touch_result('T' == data ? TOUCH_SHORT : TOUCH_LONG);
    //     fsm = TOUCH_STATE_ASSERT;
    //     event_tick = tick;
    //   }
    // }
#endif
    if(has_touch && btn_down()) {
      fsm = TOUCH_STATE_DOWN;
      event_tick = tick;
    }
    break;
  case TOUCH_STATE_DOWN:
    if(!btn_down() || tick - event_tick > MIN_LONG_TOUCH_TIME) {
      if (tick - event_tick > MIN_TOUCH_TIME) {
        set_touch_result(tick - event_tick > MIN_LONG_TOUCH_TIME ? TOUCH_LONG : TOUCH_SHORT);
        fsm = TOUCH_STATE_ASSERT;
        event_tick = tick;
      } else
        fsm = TOUCH_STATE_IDLE;
    }
    break;
  case TOUCH_STATE_ASSERT:
    if (tick - event_tick > TOUCH_GAP_TIME) {
      set_touch_result(TOUCH_NO);
      fsm = TOUCH_STATE_DEASSERT;
      DBG_MSG("touch deasserted\r\n");
    }
    break;
  case TOUCH_STATE_DEASSERT:
    if(!btn_down()) {
      fsm = TOUCH_STATE_IDLE;
    }
    break;
  default:
    break;
  }
  device_update_led();
}

void device_set_timeout(void (*callback)(void), uint16_t timeout) {
    // DBG_MSG("CB=%p Time=%hu\n", callback, timeout);
    nrfx_timer_disable(&m_timer_timeout);
    if(timeout == 0) {
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
