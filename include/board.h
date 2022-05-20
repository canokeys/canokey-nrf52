#ifndef __BOARD_H
#define __BOARD_H

#include <stdint.h>
#include <stdbool.h>

void board_init();

void board_led_write(bool val);
bool board_button_read();

uint32_t board_millis();

#endif /* __BOARD_H */