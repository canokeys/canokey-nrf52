#ifndef _NFC_DUMMY_H_
#define _NFC_DUMMY_H_

#include <stdint.h>

void fm_nss_low(void);
void fm_nss_high(void);
void fm_transmit(uint8_t *buf, uint8_t len);
void fm_receive(uint8_t *buf, uint8_t len);


#endif /* _NFC_DUMMY_H_ */