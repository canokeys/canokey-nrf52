#ifndef M2_DOCK_H__
#define M2_DOCK_H__

#include "nrf_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

// LED
#define LED_PIN 39
#define LED_STATE_ON 0

// Button
#define BUTTON_PIN 19
#define BUTTON_STATE_ACTIVE 0
#define BUTTON_PULL NRF_GPIO_PIN_PULLUP

// UART
#define UART_RX_PIN 15
#define UART_TX_PIN 16

#ifdef __cplusplus
}
#endif

#endif /* M2_DOCK_H__ */