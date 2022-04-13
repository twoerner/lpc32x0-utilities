// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "registers.h"

void
print_access (Access_e access)
{
	if (access & accessRead)
		printf("R");
	if (access & accessWrite)
		printf("W");
}

/*
 * 0-based [31:0]
 */
uint32_t
get_field (uint32_t val, unsigned high, unsigned low)
{
	uint32_t bits, mask;

	// preconds
	if (high < low)
		return 0;
	if (high > 31)
		return 0;

	bits = (high - low) + 1;
	if (bits >= 32)
		mask = 0xffffffff;
	else
		mask = ((1 << bits) - 1) << low;
	return ((val & mask) >> low);
}

uint32_t
print_field (uint32_t val, unsigned high, unsigned low)
{
	uint32_t bits;
	uint32_t i;
	uint32_t mask1bit;
	char buf[40];
	size_t pos;

	// preconds
	if (high < low)
		return 0;
	if (high > 31)
		return 0;

	bits = (high - low) + 1;
	val = get_field(val, high, low);

	memset(buf, '.', sizeof(buf));
	buf[0] = '0';
	buf[1] = 'b';
	pos = 2;
	for (i=0,mask1bit=(1<<(bits-1)); i<bits; ++i,mask1bit>>=1) {
		if (val&mask1bit)
			buf[pos++] = '1';
		else
			buf[pos++] = '0';
	}
	buf[35] = 0;

	printf("\t\t\t%s", buf);
	return val;
}

extern RegisterDescription_t clkpower[];
extern size_t clkpowerSZ;
extern RegisterDescription_t gpdma[];
extern size_t gpdmaSZ;
extern RegisterDescription_t emc[];
extern size_t emcSZ;
extern RegisterDescription_t lcd[];
extern size_t lcdSZ;
extern RegisterDescription_t mlc[];
extern size_t mlcSZ;
extern RegisterDescription_t slc[];
extern size_t slcSZ;
extern RegisterDescription_t ts[];
extern size_t tsSZ;
extern RegisterDescription_t gpio[];
extern size_t gpioSZ;
extern RegisterDescription_t pinmux[];
extern size_t pinmuxSZ;
extern RegisterDescription_t spi[];
extern size_t spiSZ;
extern RegisterDescription_t ssp[];
extern size_t sspSZ;

AllRegisters_t AllRegisters_G[] = {
	{"clkpwr", &clkpowerSZ, clkpower},
	/* interrupt */
	{"gpdma", &gpdmaSZ, gpdma},
	{"emc", &emcSZ, emc},
	{"mlc", &mlcSZ, mlc},
	{"slc", &slcSZ, slc},
	{"lcd", &lcdSZ, lcd},
	{"ts", &tsSZ, ts},
	/* adc */
	/* kbd */
	/* mac */
	/* usb-d */
	/* usb-h */
	/* usb-otg */
	/* sdcard */
	/* uart */
	/* uart-14 */
	{"spi", &spiSZ, spi},
	{"ssp", &sspSZ, ssp},
	/* i2c */
	/* i2s */
	/* timer */
	/* timer-hs */
	/* timer-ms */
	/* rtc */
	/* wdt */
	/* pwm-mc */
	/* pwm */
	{"gpio", &gpioSZ, gpio},
	{"pinmux", &pinmuxSZ, pinmux},
};
size_t AllRegistersSZ = sizeof(AllRegisters_G)/sizeof(AllRegisters_G[0]);

// let's reuse these between invocations
// this is not thread safe!
static int memFd_G = -1;
static void *map_pG = NULL;
static uint32_t curBase_G = 0;

static bool
open_dev_mem (void)
{
	if (memFd_G == -1) {
		memFd_G = open("/dev/mem", O_RDWR | O_SYNC);
		if (memFd_G == -1) {
			perror("open(/dev/mem)");
			return false;
		}
	}
	return true;
}

static bool
set_mapping (uint32_t addr)
{
	uint32_t newBase;

	// set/reset base mapping
	newBase = addr & 0xfffff000;
	if (curBase_G != newBase) {
		curBase_G = newBase;
		if (map_pG != NULL)
			munmap(map_pG, 0x00001000);
		map_pG = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, memFd_G, curBase_G);
		if (map_pG == MAP_FAILED) {
			perror("mmap()");
			return false;
		}
	}
	return true;
}

bool
lpc32x0__get_reg (uint32_t addr, uint32_t *regRet_p)
{
	size_t i, idx;
	uint32_t offset;

	if (!open_dev_mem())
		return false;

	*regRet_p = 0xffffffff;
	for (idx=0; idx<AllRegistersSZ; ++idx) {
		for (i=0; i<*(AllRegisters_G[idx].sz_p); ++i) {
			if (AllRegisters_G[idx].reg_p[i].addr == addr) {
				// check for readability
				if (!(AllRegisters_G[idx].reg_p[i].access & accessRead)) {
					printf("  %s (%08x) - skip non-readable\n", AllRegisters_G[idx].reg_p[i].name_p, addr);
					continue;
				}

				if (!set_mapping(addr))
					return false;
				offset = addr & 0x00000fff;
				*regRet_p = *(uint32_t*)((unsigned long)(map_pG) | (unsigned long)(offset));
				return true;
			}
		}
	}
	return false;
}

bool
lpc32x0__set_reg (uint32_t addr, uint32_t val)
{
	size_t i, idx;
	uint32_t offset;

	if (!open_dev_mem())
		return false;

	for (idx=0; idx<AllRegistersSZ; ++idx) {
		for (i=0; i<*(AllRegisters_G[idx].sz_p); ++i) {
			if (AllRegisters_G[idx].reg_p[i].addr == addr) {
				// check for writeability
				if (!(AllRegisters_G[idx].reg_p[i].access & accessWrite)) {
					printf("  %s (%08x) - skip non-writeable\n", AllRegisters_G[idx].reg_p[i].name_p, addr);
					continue;
				}

				if (!set_mapping(addr))
					return false;
				offset = addr & 0x00000fff;
				*(uint32_t*)((unsigned long)(map_pG) | (unsigned long)(offset)) = val;
				return true;
			}
		}
	}
	return false;
}

void
lpc32x0__cleanup (void)
{
	if (memFd_G != -1) {
		close(memFd_G);
		memFd_G = -1;
	}
	if (map_pG != NULL) {
		munmap(map_pG, 0x00001000);
		map_pG = NULL;
	}
}

bool
lpc32x0__print_reg (uint32_t addr, uint32_t val, bool verbose)
{
	size_t i, idx;

	for (idx=0; idx<AllRegistersSZ; ++idx) {
		for (i=0; i<*(AllRegisters_G[idx].sz_p); ++i) {
			if (AllRegisters_G[idx].reg_p[i].addr == addr) {
				if (verbose) {
					printf("0x%08x %-15s %s\n",
							AllRegisters_G[idx].reg_p[i].addr,
							AllRegisters_G[idx].reg_p[i].name_p,
							AllRegisters_G[idx].reg_p[i].desc_p);
					printf("\ttype:  "); print_access(AllRegisters_G[idx].reg_p[i].access); printf("\n");
					printf("\treset: 0x%08x\n", AllRegisters_G[idx].reg_p[i].resetState);
					printf("\tvalue: 0x%08x\n", val);
					if (AllRegisters_G[idx].reg_p[i].field_fp != NULL) {
						(*AllRegisters_G[idx].reg_p[i].field_fp)(val);
						printf("\n");
					}
					printf("\n");
				}
				else {
					printf("  %s (%08x) %*s 0x%08x\n",
							AllRegisters_G[idx].reg_p[i].name_p,
							AllRegisters_G[idx].reg_p[i].addr,
							25-(int)strlen(AllRegisters_G[idx].reg_p[i].name_p), " ",
							val);
				}
				return true;
			}
		}
	}
	return false;
}

bool
lpc32x0__get_and_print_reg_set_by_name (char *regSetName_p, bool verbose)
{
	uint32_t val;
	size_t i, idx;

	for (idx=0; idx<AllRegistersSZ; ++idx) {
		if (strcmp(AllRegisters_G[idx].name_p, regSetName_p) != 0)
			continue;
		printf("%s:\n", regSetName_p);
		for (i=0; i<*(AllRegisters_G[idx].sz_p); ++i) {
			if (lpc32x0__get_reg(AllRegisters_G[idx].reg_p[i].addr, &val))
				lpc32x0__print_reg(AllRegisters_G[idx].reg_p[i].addr, val, verbose);
		}
		return true;
	}
	return false;
}

bool
lpc32x0__get_and_print_all_regs (bool verbose)
{
	bool rtn = true;
	size_t idx;

	for (idx=0; idx<AllRegistersSZ; ++idx)
		if (!lpc32x0__get_and_print_reg_set_by_name(AllRegisters_G[idx].name_p, verbose))
			rtn = false;
	return rtn;
}
