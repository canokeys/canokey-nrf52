cmake_minimum_required(VERSION 3.18)

message(STATUS "NRF5_SDK_DIR: ${NRF5_SDK_DIR}")

set(NRF5_SDK_STARTUP_SOURCES
    ${NRF5_SDK_DIR}/modules/nrfx/mdk/gcc_startup_nrf52840.S
    ${NRF5_SDK_DIR}/modules/nrfx/mdk/system_nrf52840.c
)

set(NRF5_SDK_LIBRARY_SOURCES
    ${NRF5_SDK_DIR}/components/libraries/atomic/nrf_atomic.c
    ${NRF5_SDK_DIR}/components/libraries/balloc/nrf_balloc.c
    ${NRF5_SDK_DIR}/components/libraries/fifo/app_fifo.c
    ${NRF5_SDK_DIR}/components/libraries/hardfault/hardfault_implementation.c
    ${NRF5_SDK_DIR}/components/libraries/hardfault/nrf52/handler/hardfault_handler_gcc.c
    ${NRF5_SDK_DIR}/components/libraries/log/src/nrf_log_backend_rtt.c
    ${NRF5_SDK_DIR}/components/libraries/log/src/nrf_log_backend_serial.c
    ${NRF5_SDK_DIR}/components/libraries/log/src/nrf_log_backend_uart.c
    ${NRF5_SDK_DIR}/components/libraries/log/src/nrf_log_default_backends.c
    ${NRF5_SDK_DIR}/components/libraries/log/src/nrf_log_frontend.c
    ${NRF5_SDK_DIR}/components/libraries/log/src/nrf_log_str_formatter.c
    ${NRF5_SDK_DIR}/components/libraries/memobj/nrf_memobj.c
    ${NRF5_SDK_DIR}/components/libraries/ringbuf/nrf_ringbuf.c
    ${NRF5_SDK_DIR}/components/libraries/strerror/nrf_strerror.c
    ${NRF5_SDK_DIR}/components/libraries/uart/app_uart_fifo.c
    ${NRF5_SDK_DIR}/components/libraries/util/app_error_handler_gcc.c
    ${NRF5_SDK_DIR}/components/libraries/util/app_error_weak.c
    ${NRF5_SDK_DIR}/components/libraries/util/app_error.c
    ${NRF5_SDK_DIR}/components/libraries/util/app_util_platform.c
    ${NRF5_SDK_DIR}/components/libraries/util/nrf_assert.c

    ${NRF5_SDK_DIR}/external/fprintf/nrf_fprintf.c
    ${NRF5_SDK_DIR}/external/fprintf/nrf_fprintf_format.c
)

set(NRF5_SDK_DRIVER_SOURCES
    ${NRF5_SDK_DIR}/components/boards/boards.c
    ${NRF5_SDK_DIR}/components/drivers_nrf/nrf_soc_nosd/nrf_nvic.c
    ${NRF5_SDK_DIR}/components/drivers_nrf/nrf_soc_nosd/nrf_soc.c

    ${NRF5_SDK_DIR}/integration/nrfx/legacy/nrf_drv_clock.c
    ${NRF5_SDK_DIR}/integration/nrfx/legacy/nrf_drv_uart.c

    ${NRF5_SDK_DIR}/modules/nrfx/hal/nrf_nvmc.c

    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src/nrfx_clock.c
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src/nrfx_nfct.c
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src/nrfx_power.c
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src/nrfx_timer.c
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src/nrfx_uart.c
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src/nrfx_uarte.c
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src/nrfx_rng.c
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src/nrfx_nvmc.c
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src/prs/nrfx_prs.c
    ${NRF5_SDK_DIR}/modules/nrfx/soc/nrfx_atomic.c
)

set(NRF5_SDK_OTHER_SOURCES
    ${NRF5_SDK_DIR}/components/nfc/platform/nfc_platform.c

    ${NRF5_SDK_DIR}/components/nfc/ndef/generic/message/nfc_ndef_msg.c
    ${NRF5_SDK_DIR}/components/nfc/ndef/generic/record/nfc_ndef_record.c
    ${NRF5_SDK_DIR}/components/nfc/ndef/text/nfc_text_rec.c
)

set(NRF5_SDK_SOURCES
    ${NRF5_SDK_STARTUP_SOURCES}
    ${NRF5_SDK_LIBRARY_SOURCES}
    ${NRF5_SDK_DRIVER_SOURCES}
    ${NRF5_SDK_OTHER_SOURCES}
)

set(NRF5_SDK_NRFX_INCLUDES
    ${NRF5_SDK_DIR}/integration/nrfx
    ${NRF5_SDK_DIR}/integration/nrfx/legacy

    ${NRF5_SDK_DIR}/modules/nrfx
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/include
    ${NRF5_SDK_DIR}/modules/nrfx/drivers/src
    ${NRF5_SDK_DIR}/modules/nrfx/hal
    ${NRF5_SDK_DIR}/modules/nrfx/mdk
)

set(NRF5_SDK_INCLUDES
    ${NRF5_SDK_DIR}/components
    ${NRF5_SDK_DIR}/components/boards
    ${NRF5_SDK_DIR}/components/drivers_nrf/nrf_soc_nosd
    ${NRF5_SDK_DIR}/components/libraries/atomic
    ${NRF5_SDK_DIR}/components/libraries/balloc
    ${NRF5_SDK_DIR}/components/libraries/bsp
    ${NRF5_SDK_DIR}/components/libraries/delay
    ${NRF5_SDK_DIR}/components/libraries/experimental_section_vars
    ${NRF5_SDK_DIR}/components/libraries/fifo
    ${NRF5_SDK_DIR}/components/libraries/hardfault
    ${NRF5_SDK_DIR}/components/libraries/hardfault/nrf52
    ${NRF5_SDK_DIR}/components/libraries/log
    ${NRF5_SDK_DIR}/components/libraries/log/src
    ${NRF5_SDK_DIR}/components/libraries/memobj
    ${NRF5_SDK_DIR}/components/libraries/ringbuf
    ${NRF5_SDK_DIR}/components/libraries/strerror
    ${NRF5_SDK_DIR}/components/libraries/uart
    ${NRF5_SDK_DIR}/components/libraries/util
    ${NRF5_SDK_DIR}/components/nfc/ndef/generic/message
    ${NRF5_SDK_DIR}/components/nfc/ndef/generic/record
    ${NRF5_SDK_DIR}/components/nfc/ndef/text
    ${NRF5_SDK_DIR}/components/nfc/platform
    ${NRF5_SDK_DIR}/components/nfc/t2t_lib
    ${NRF5_SDK_DIR}/components/toolchain/cmsis/include

    ${NRF5_SDK_DIR}/external/fprintf
    ${NRF5_SDK_DIR}/external/segger_rtt

    ${NRF5_SDK_NRFX_INCLUDES}
)
