#include "memory/mmu/cache.h"
#include "memory/memory.h"

#ifdef CACHE_ENABLED
struct CacheLine cache[CACHE_LINE_NUM];
// init the cache
void init_cache()
{
	// implement me in PA 3-1
	printf("init cache\n");
	for (int i = 0; i < CACHE_LINE_NUM; i++) {
		cache[i].vaild_bit = false;
		cache[i].tag = 0;
		memset(cache[i].data, 0, CACHE_BLOCK_SIZE);
	}
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
	
	// implement me in PA 3-1
	// 根据paddr找到标记、组号、与块内地址
	uint32_t tag = paddr >> 13;
	uint8_t offset = paddr & 0x3f;
	uint8_t set_num = (paddr >> 6) & 0x7f;
	
	bool hit = false;
	int line_num = -1;
	
	for(int i = 0; i < CACHE_SET_LINE_NUM; i++){
		if(cache[set_num*CACHE_SET_LINE_NUM+i].tag == tag && cache[set_num*CACHE_SET_LINE_NUM+i].vaild_bit == true)
		{
			hit = true;
			line_num = set_num*CACHE_SET_LINE_NUM+i;
			break;
		}
	}

    // 若未命中则直接写主存，非写分配法中写缺失不导致Cache装载	
	// 若命中，根据全写法写Cache同时写主存	
	if(hit == true)
	{
		// 若数据不跨两个Cache行则直接写入Cache 
		if(offset + len <= CACHE_BLOCK_SIZE)
		{
			memcpy(cache[line_num].data + offset, &data, len); 
		} 
		else
		{ 
			memcpy(cache[line_num].data + offset, &data, CACHE_BLOCK_SIZE - offset);
			cache_write(paddr + (CACHE_BLOCK_SIZE - offset), len - (CACHE_BLOCK_SIZE - offset), data >> (8 * (CACHE_BLOCK_SIZE - offset)));
		}
	
	}

	memcpy(hw_mem + paddr, &data, len);
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
	
	// implement me in PA 3-1
	uint32_t result = 0;

	uint32_t tag = paddr >> 13;
	uint8_t offset = paddr & 0x3f;
	uint8_t set_num = (paddr >> 6) & 0x7f;

	bool hit = false;
	int line_num = -1;
	//缓存命中
	for(int i = 0; i < CACHE_SET_LINE_NUM; i++){
		if(cache[set_num*CACHE_SET_LINE_NUM+i].tag == tag && cache[set_num*CACHE_SET_LINE_NUM+i].vaild_bit == true)
		{
			hit = true;
			line_num = set_num*CACHE_SET_LINE_NUM + i;
			break;
		}
	}
	//缓存未命中
	if(hit == false)
	{
		for(int i = 0; i < CACHE_SET_LINE_NUM; i++){
			if(cache[set_num*CACHE_SET_LINE_NUM+i].vaild_bit == false){
				line_num = set_num*CACHE_SET_LINE_NUM+i;
				break;
			}
		}

		if(line_num == -1)
		{
			srand((unsigned)time(0));
			line_num = set_num * CACHE_SET_LINE_NUM + (rand() % CACHE_SET_LINE_NUM);
			cache[line_num].vaild_bit = false;
		}

		memcpy(cache[line_num].data, hw_mem + paddr - offset, CACHE_BLOCK_SIZE);
		cache[line_num].tag = tag;
		cache[line_num].vaild_bit = true;
	
	}

	// 按照Cache Line内地址读cache
	// 若数据不跨两个Cache行则直接读Cache	
	if(offset + len <= CACHE_BLOCK_SIZE)
	{
		memcpy(&result, cache[line_num].data + offset, len);

	}
	// 若数据跨两个Cache行，则分两次读两个Cache行后，将结果按小端方式拼起
	else
	{	
		uint32_t temp1 = 0,temp2 = 0;
		memcpy(&temp1, cache[line_num].data + offset, CACHE_BLOCK_SIZE - offset);
		temp2 = cache_read(paddr + (CACHE_BLOCK_SIZE - offset), len - (CACHE_BLOCK_SIZE - offset));
		result = temp1 | (temp2 << 8 * (CACHE_BLOCK_SIZE - offset));
	}
	return result;
}

#endif


