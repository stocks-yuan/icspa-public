#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"

#ifdef CACHE_ENABLED

#define CACHE_BLOCK_SIZE 64
#define CACHE_SET_NUM 128
#define CACHE_LINE_NUM 1024
#define CACHE_SET_LINE_NUM (CACHE_LINE_NUM/CACHE_SET_NUM)

struct CacheLine {
    bool vaild_bit;
    uint32_t tag;
    uint8_t data[CACHE_BLOCK_SIZE];
};

// init the cache
void init_cache();

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data);

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len);

#endif

#endif


