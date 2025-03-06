#include "cpu/cpu.h"

static bool cal_pf(uint32_t result)
{
	result &= 0xff;
	result ^= result >>4;
	result ^= result >>2;
	result ^= result >>1;
	return (result &0x01) == 0;
}

static bool cal_zf(uint32_t result)
{
	return result == 0;
}

/** Attention: Unsigned number overflow and signed number overflow**/
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
       	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
		       
	}
	src &= mask;
	dest &= mask;
	uint64_t res_full = (uint64_t)src + dest;//src should be uint64_t,or overflow will happen.
	uint32_t res = res_full & mask;
	
	
	cpu.eflags.CF = (res_full > mask);
	cpu.eflags.OF = ((dest >> (data_size-1)) == (src >> (data_size -1))) && ((res >> (data_size -1)) != (dest >> (data_size -1)));
      	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	
	return res;
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;
	uint64_t res_full = (uint64_t)src + dest + cpu.eflags.CF;
	uint32_t res = res_full & mask;

	cpu.eflags.CF = (res_full > mask);
	cpu.eflags.OF = ((dest >> (data_size-1)) == (src >> (data_size -1))) && ((res >> (data_size -1)) != (dest >> (data_size -1)));
      	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	
	return res;
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;

	uint32_t res = dest - src;
	res &= mask;
	
	cpu.eflags.CF = (src > dest);
	cpu.eflags.OF = ((dest >> (data_size-1)) != (src >> (data_size -1))) && ((res >> (data_size -1)) != (dest >> (data_size -1)));
	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	
	return res;
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;

	uint32_t cf = cpu.eflags.CF;
	uint32_t res = dest - src - cf;
	res &= mask;

	uint64_t extend_src = (uint64_t)src + cf;

	cpu.eflags.CF = (extend_src > dest);
	cpu.eflags.OF = ((dest >> (data_size-1)) != (src >> (data_size -1))) && ((res >> (data_size -1)) != (dest >> (data_size -1)));
	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	
	return res;
#endif
}

/* 无符号整数乘法 */
uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;	
	
	uint64_t res = (uint64_t)src * dest;

	cpu.eflags.CF = ((res >> data_size) != 0);
	cpu.eflags.OF = ((res >> data_size) != 0);

	return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}

	uint32_t res = dest / src;
	res &= mask; 

	return res;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);

	uint32_t res = dest % src;

	return res;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;	 
	
	uint32_t res = src & dest;
	res &= mask;

	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	
	return res;
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;	 
	
	uint32_t res = src ^ dest;
	res &= mask;

	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	
	return res;
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;	

	uint32_t res = src | dest;
	res &= mask;

	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	
	return res;
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;	
	bool orignal_sign = dest >> (data_size - 1);

	uint32_t res = dest << src;
	res &= mask;

	bool new_sign = res >> (data_size - 1);
	if(src ==1 && orignal_sign != new_sign )
	{
		cpu.eflags.OF = 1;
	}
	cpu.eflags.CF = (src > 0) ? ((dest >> (data_size - src)) & 0x1) : 0;
	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	return res;
#endif
}

/* 逻辑右移：低位丢弃，高位补0 */
uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;	

	uint32_t res = dest >> src;
	res &= mask;

	cpu.eflags.CF = (src > 0) ? ((dest >> (src-1)) & 0x1) : 0;
	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	return res;
#endif
}

/*
算数右移:​低位丢弃，高位补符号位。
正数高位补0，与逻辑右移结果相同
负数高位补1，与逻辑右移结果不同
*/
uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;	

	int32_t sign_res = (int32_t)(dest << (32 - data_size)) >> (32 - data_size);
	sign_res = sign_res >> src;
	uint32_t res = sign_res & mask;
	

	cpu.eflags.CF = (src > 0) ? ((dest >> (src-1)) & 0x1) : 0;
	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	return res;
#endif
}

/* 算数左移和逻辑左移效果一样，都是低位补0*/
uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	// fflush(stdout);
	// assert(0);
	uint32_t mask = 0;
	switch(data_size)
	{
		case 8:mask = 0xff;break;
		case 16:mask = 0xffff;break;
		case 32:mask = 0xffffffff;break;
		default:break;
	}
	src &= mask;
	dest &= mask;	
	bool orignal_sign = dest >> (data_size - 1);

	uint32_t res = dest << src;
	res &= mask;

	bool new_sign = res >> (data_size - 1);
	if(src ==1 && orignal_sign != new_sign )
	{
		cpu.eflags.OF = 1;
	}
	cpu.eflags.CF = (src > 0) ? ((dest >> (data_size - src)) & 0x1) : 0;
	cpu.eflags.ZF = cal_zf(res);
	cpu.eflags.SF = (res >> (data_size - 1) == 0x01);
	cpu.eflags.PF = cal_pf(res);	
	return res;
#endif
}
