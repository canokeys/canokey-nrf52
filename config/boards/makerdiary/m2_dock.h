/**
 * Copyright (c) 2016 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef M2DOCK_H
#define M2DOCK_H

#ifdef __cplusplus
extern "C" {
#endif

// LEDs definitions for M2DOCK
#define LEDS_NUMBER    1

#define LED_1          NRF_GPIO_PIN_MAP(1,7)
#define LED_START      LED_1
#define LED_STOP       LED_1

#define LEDS_ACTIVE_STATE 0

#define LEDS_LIST { LED_1 }

#define LEDS_INV_MASK  LEDS_MASK

#define BSP_LED_0      LED_1

#define BUTTONS_NUMBER 1

#define BUTTON_1       NRF_GPIO_PIN_MAP(0,19)
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST { BUTTON_1 }

#define BSP_BUTTON_0   BUTTON_1

#define RX_PIN_NUMBER  NRF_GPIO_PIN_MAP(0,15)
#define TX_PIN_NUMBER  NRF_GPIO_PIN_MAP(0,16)
#define CTS_PIN_NUMBER NRF_GPIO_PIN_MAP(0,13)
#define RTS_PIN_NUMBER NRF_GPIO_PIN_MAP(0,14)
#define HWFC           false

#define BSP_QSPI_SCK_PIN   NRF_GPIO_PIN_MAP(1,11)
#define BSP_QSPI_CSN_PIN   NRF_GPIO_PIN_MAP(1,13)
#define BSP_QSPI_IO0_PIN   NRF_GPIO_PIN_MAP(1,10)
#define BSP_QSPI_IO1_PIN   NRF_GPIO_PIN_MAP(1,14)
#define BSP_QSPI_IO2_PIN   NRF_GPIO_PIN_MAP(1,15)
#define BSP_QSPI_IO3_PIN   NRF_GPIO_PIN_MAP(1,12)


// Arduino board mappings
#define ARDUINO_SCL_PIN             NRF_GPIO_PIN_MAP(1, 6)    // SCL signal pin
#define ARDUINO_SDA_PIN             NRF_GPIO_PIN_MAP(1, 5)    // SDA signal pin
// #define ARDUINO_AREF_PIN            2     // Aref pin

#define ARDUINO_13_PIN              NRF_GPIO_PIN_MAP(1, 7)  // Digital pin 13
#define ARDUINO_12_PIN              NRF_GPIO_PIN_MAP(1, 4)  // Digital pin 12
#define ARDUINO_11_PIN              NRF_GPIO_PIN_MAP(1, 3)  // Digital pin 11
#define ARDUINO_10_PIN              NRF_GPIO_PIN_MAP(1, 2)  // Digital pin 10
#define ARDUINO_9_PIN               NRF_GPIO_PIN_MAP(1, 1)  // Digital pin 9
#define ARDUINO_8_PIN               NRF_GPIO_PIN_MAP(1, 0)  // Digital pin 8

#define ARDUINO_7_PIN               NRF_GPIO_PIN_MAP(0, 24) // Digital pin 7
#define ARDUINO_6_PIN               NRF_GPIO_PIN_MAP(0, 23) // Digital pin 6
#define ARDUINO_5_PIN               NRF_GPIO_PIN_MAP(0, 22) // Digital pin 5
#define ARDUINO_4_PIN               NRF_GPIO_PIN_MAP(0, 21) // Digital pin 4
#define ARDUINO_3_PIN               NRF_GPIO_PIN_MAP(0, 20) // Digital pin 3
#define ARDUINO_2_PIN               NRF_GPIO_PIN_MAP(0, 19) // Digital pin 2
#define ARDUINO_1_PIN               NRF_GPIO_PIN_MAP(0, 16) // Digital pin 1
#define ARDUINO_0_PIN               NRF_GPIO_PIN_MAP(0, 15) // Digital pin 0

#define ARDUINO_A0_PIN              NRF_GPIO_PIN_MAP(0, 2)  // Analog channel 0
#define ARDUINO_A1_PIN              NRF_GPIO_PIN_MAP(0, 3)  // Analog channel 1
#define ARDUINO_A2_PIN              NRF_GPIO_PIN_MAP(0, 28) // Analog channel 2
#define ARDUINO_A3_PIN              NRF_GPIO_PIN_MAP(0, 27) // Analog channel 3
#define ARDUINO_A4_PIN              NRF_GPIO_PIN_MAP(0, 26) // Analog channel 4
#define ARDUINO_A5_PIN              NRF_GPIO_PIN_MAP(0, 4)  // Analog channel 5


#ifdef __cplusplus
}
#endif

#endif // M2DOCK_H
