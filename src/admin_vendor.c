// SPDX-License-Identifier: Apache-2.0
#include <board.h>
#include <device.h>
#include <globals.h>
#include <tusb.h>
#include <admin.h>
#include <git-rev.h>

#include <nrf_gpio.h>
#include <nrf_nvmc.h>
#include <nrfx.h>
#include <nrfx_timer.h>
#include <nrfx_uarte.h>

#include <boards.h>


#define VENDOR_NFC_SET 0x01
#define VENDOR_NFC_GET 0x02
#define VENDOR_STACK_TEST 0x03
#define VENDOR_RDP 0x55
#define VENDOR_DFU 0x22

void EnableRDP(void);
void EnterDFUBootloader(void);

// override functions defined in admin.c

int admin_vendor_hw_variant(const CAPDU *capdu, RAPDU *rapdu) {
  UNUSED(capdu);

  const char *s;
  static const char *const hw_variant_str[] = {
      [0] = "CanoKey ES (NRF52)",
  };

  s = hw_variant_str[0];

  size_t len = strlen(s);
  memcpy(RDATA, s, len);
  LL = len;
  if (LL > LE) LL = LE;

  return 0;
}

int admin_vendor_version(const CAPDU *capdu, RAPDU *rapdu) {
  UNUSED(capdu);

  size_t len = strlen(GIT_REV);
  memcpy(RDATA, GIT_REV, len);
  memcpy(RDATA + len, "-O", 2);
  LL = len + 2;
  if (LL > LE) LL = LE;

  return 0;
}

int admin_vendor_specific(const CAPDU *capdu, RAPDU *rapdu) {

  switch (P1) {
  case VENDOR_RDP:
    DBG_MSG("Enable RDP level %d\n", (int)P2);
    if (P2 == 1)
      EnableRDP();
    // else if (P2 == 2)
    //   EnableRDP(OB_RDP_LEVEL_2);
    else
      EXCEPT(SW_WRONG_P1P2);
    break;

  case VENDOR_DFU:
    if (P2 != VENDOR_DFU) EXCEPT(SW_WRONG_P1P2);
    DBG_MSG("Entering DFU\n");
    EnterDFUBootloader();
    ERR_MSG("Failed to enter DFU\n");
    for (;;)
      ;

  default:
    EXCEPT(SW_WRONG_P1P2);
  }

  return 0;
}


void EnableRDP() {
  if (NRF_UICR->APPROTECT != 0x0)
  {
    nrf_nvmc_write_word((uint32_t)&NRF_UICR->APPROTECT, 0x0);
  }
}

void EnterDFUBootloader() {
  nrf_gpio_cfg_output(BSP_SELF_PINRESET_PIN);
  nrf_gpio_pin_clear(BSP_SELF_PINRESET_PIN);
}

