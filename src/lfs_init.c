#include "main.h"
#include <common.h>
#include <fs.h>
#include <memzero.h>
#include <nrf_nvmc.h>
#include <nrfx_nvmc.h>
#include <stdalign.h>
#include <string.h>

#define FLASH_PAGE_SIZE (nrfx_nvmc_flash_page_size_get())
#define LOOKAHEAD_SIZE 16
#define CACHE_SIZE 128
#define WRITE_SIZE 4
#define READ_SIZE 1
#define FS_BASE (&_lfs_begin)
#define FS_END (&_lfs_end)
#define FLASH_ADDR(b, o) (FS_BASE + (b)*FLASH_PAGE_SIZE + (o))
// #define FLASH_ADDR2BLOCK(a) (((a) & ~0x8000000u) / FLASH_PAGE_SIZE)

static struct lfs_config config;
static uint8_t read_buffer[CACHE_SIZE];
static uint8_t prog_buffer[CACHE_SIZE];
static alignas(4) uint8_t lookahead_buffer[LOOKAHEAD_SIZE];

// Provided by the linker script
extern uint8_t _lfs_begin;
extern uint8_t _lfs_end;

int block_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    (void)c;

    // DBG_MSG("blk %d @ %p len %u buf %p\r\n", block, (void *)FLASH_ADDR(block, off), size, buffer);
    memcpy(buffer, (const void *)FLASH_ADDR(block, off), size);
    return 0;
}

int block_prog(const struct lfs_config *c, lfs_block_t block,
               lfs_off_t off, const void *buffer, lfs_size_t size) {
    (void)c;
    // DBG_MSG("blk %d @ %p len %u buf %p\r\n", block, (void *)FLASH_ADDR(block, off), size, buffer);

    LFS_ASSERT(size % 4 == 0);

    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_WRITE);

    // Program 32 bit words
    uint32_t *src = (uint32_t *)buffer;
    uint32_t *dst = (uint32_t *)FLASH_ADDR(block, off);
    memcpy(dst, src, size);

    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_READONLY);

    return 0;
}

int block_erase(const struct lfs_config *c, lfs_block_t block) {
    (void)c;
    DBG_MSG("blk %d\r\n", block);

    nrfx_err_t err = nrfx_nvmc_page_erase((uint32_t)FLASH_ADDR(block, 0));
    if(err != NRFX_SUCCESS) {
        ERR_MSG("erase block %d failed status = %d\r\n", block, err);
        return LFS_ERR_IO;
    }

    return LFS_ERR_OK;
}

int block_sync(const struct lfs_config *c) {
    (void)c;
    return 0;
}

void littlefs_init() {
    memzero(&config, sizeof(config));
    config.read = block_read;
    config.prog = block_prog;
    config.erase = block_erase;
    config.sync = block_sync;

    config.read_size = READ_SIZE;
    config.prog_size = WRITE_SIZE;
    config.block_size = FLASH_PAGE_SIZE;
    config.block_count = (FS_END - FS_BASE) / FLASH_PAGE_SIZE;

    config.block_cycles = 100000;
    config.cache_size = CACHE_SIZE;
    config.lookahead_size = LOOKAHEAD_SIZE;
    config.read_buffer = read_buffer;
    config.prog_buffer = prog_buffer;
    config.lookahead_buffer = lookahead_buffer;

    DBG_MSG("Flash base %p, end %p, %u blocks (%u bytes)\r\n", FS_BASE, FS_END, config.block_count, FLASH_PAGE_SIZE);

    int err;
    for(int retry = 0; retry < 3; retry++) {
        err = fs_mount(&config);
        if(!err) {
            DBG_MSG("Mounted littlefs\r\n");
            return;
        }
    }

    // should happen for the first boot
    DBG_MSG("Formating data area...\r\n");
    fs_format(&config);
    err = fs_mount(&config);
    if(err) {
        ERR_MSG("Failed to mount FS after formating\r\n");
        for(;;)
            ;
    }
}