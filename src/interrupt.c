// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "registers.h"

typedef enum {
	ER, RSR, SR, APR, ATR, ITR, MAX_REG
} IntRegTypes_e;

typedef struct {
	bool set;
	uint32_t addr;
	uint32_t val;
} RegRecord_t;

static RegRecord_t MicBlock_G[MAX_REG];
static RegRecord_t Sic1Block_G[MAX_REG];
static RegRecord_t Sic2Block_G[MAX_REG];

static char
get_bit (uint32_t reg, unsigned bitPos)
{
	uint32_t mask;

	/* preconds */
	if (bitPos > 31)
		return '?';

	mask = 1 << bitPos;
	if (reg & mask)
		return '1';
	return '0';
}

static char *MicBlockNames[] = {
	"sub1IRQn",	/* 00 */
	"sub2IRQn",
	"(reserved)",
	"Timer4/MCPWM",
	"Timer5",
	"HSTIMER_INT",	/* 05 */
	"WATCH_INT",
	"UART3",
	"UART4",
	"UART5",
	"UART6",	/* 10 */
	"FLASH_INT",
	"(reserved)",
	"SD1_INT",
	"LCD_INT",
	"SD0_INT",	/* 15 */
	"Timer0",
	"Timer1",
	"Timer2",
	"Timer3",
	"SSP0",		/* 20 */
	"SSP1",
	"I2S0",
	"I2S1",
	"UART7",
	"UART2",	/* 25 */
	"UART1",
	"MSTIMER_INT",
	"DMA_INT",
	"ethernet",
	"sub1FIQn",	/* 30 */
	"sub2FIQn"
};

static char *Sic1BlockNames[] = {
	"(reserved)",	/* 00 */
	"JTAG_COMM_TX",
	"JTAG_COMM_RX",
	"(reserved)",
	"GPI_28",
	"(reserved)",	/* 05 */
	"TS_P",
	"TS_IRQ(ADC_INT)",
	"TS_AUX",
	"(reserved)",
	"(reserved)",	/* 10 */
	"(reserved)",
	"SPI2_INT",
	"PLLUSB_INT",
	"PLLHCLK_INT",
	"(reserved)",	/* 15 */
	"(reserved)",
	"PLL397_INT",
	"I2C2_INT",
	"I2C1_INT",
	"RTC_INT",	/* 20 */
	"(reserved)",
	"KEY_IRQ",
	"SPI1_INT",
	"SW_INT",
	"USB_otg_timer_int", /* 25 */
	"USB_otg_atx_int_n",
	"USB_host_int",
	"USB_dev_dma_int",
	"USB_dev_lp_int",
	"USB_dev_hp_int",/* 30 */
	"USB_i2c_int"
};

static char *Sic2BlockNames[] = {
	"GPIO_0",	/* 00 */
	"GPIO_1",
	"GPIO_2",
	"GPIO_3",
	"GPIO_4",
	"GPIO_5",	/* 05 */
	"SPI2_DATIN",
	"UART2_HCTS",
	"Pn_GPIO",
	"GPI_8",
	"GPI_9",	/* 10 */
	"GPI_19",
	"UART7_HCTS",
	"(reserved)",
	"(reserved)",
	"GPI_7",	/* 15 */
	"(reserved)",
	"(reserved)",
	"SDIO_INT_N",
	"UART5_RX",
	"SPI1_DATIN",	/* 20 */
	"(reserved)",
	"GPI_0",
	"GPI_1",
	"GPI_2",
	"GPI_3",	/* 25 */
	"GPI_4",
	"GPI_5",
	"GPI_6",
	"(reserved)",
	"(reserved)",	/* 30 */
	"SYSCLK mux"
};

static void
print_block_row(char *name_p, RegRecord_t *blk_p, unsigned bit)
{
	/* preconds */
	if (name_p == NULL)
		return;
	if (blk_p == NULL)
		return;
	if (bit > 31)
		return;

	if (bit == 31) {
		printf("    0x%08x: 0x%08x       +----------------------------------------------- enable register\n", blk_p[ER].addr, blk_p[ER].val);
		printf("    0x%08x: 0x%08x       |        +-------------------------------------- raw status register\n", blk_p[RSR].addr, blk_p[RSR].val);
		printf("    0x%08x: 0x%08x       |        |        +----------------------------- status register\n", blk_p[SR].addr, blk_p[SR].val);
		printf("    0x%08x: 0x%08x       |        |        |        +-------------------- activation polarity register (0:active-low 1:active-high)\n", blk_p[APR].addr, blk_p[APR].val);
		printf("    0x%08x: 0x%08x       |        |        |        |        +----------- activation type register (0:level 1:edge)\n", blk_p[ATR].addr, blk_p[ATR].val);
		printf("    0x%08x: 0x%08x       |        |        |        |        |        +-- interrupt type register (0:IRQ 1:FIQ)\n", blk_p[ITR].addr, blk_p[ITR].val);
	}

	if (strncmp(name_p, "(reserved)", strlen("(reserved)")) == 0)
		printf("        %17s\n", name_p);
	else
		printf("        %17s........%c........%c........%c........%c........%c........%c\n", name_p,
				get_bit(blk_p[ER].val,  bit), get_bit(blk_p[RSR].val, bit), get_bit(blk_p[SR].val,  bit),
				get_bit(blk_p[APR].val, bit), get_bit(blk_p[ATR].val, bit), get_bit(blk_p[ITR].val, bit));
}

static void
process_MIC_block (void)
{
	int i;

	printf("MIC block\n");
	for (i=31; i>-1; --i)
		print_block_row(MicBlockNames[i], MicBlock_G, i);
	printf("\n");
}

static void
process_SIC1_block (void)
{
	int i;

	printf("SIC1 block\n");
	for (i=31; i>-1; --i)
		print_block_row(Sic1BlockNames[i], Sic1Block_G, i);
	printf("\n");
}

static void
process_SIC2_block (void)
{
	int i;

	printf("SIC2 block\n");
	for (i=31; i>-1; --i)
		print_block_row(Sic2BlockNames[i], Sic2Block_G, i);
	printf("\n");
}

void
interrupt_decode_reg (uint32_t addr, uint32_t val)
{
	size_t i;
	size_t cnt;
	static bool init = true;
	RegRecord_t *reg_p = NULL;

	if (init) {
		init = false;
		memset(MicBlock_G, 0, sizeof(MicBlock_G));
		memset(Sic1Block_G, 0, sizeof(Sic1Block_G));
		memset(Sic2Block_G, 0, sizeof(Sic2Block_G));
	}

	switch (addr) {
		case 0x40008000:
			reg_p = &MicBlock_G[ER];
			break;
		case 0x40008004:
			reg_p = &MicBlock_G[RSR];
			break;
		case 0x40008008:
			reg_p = &MicBlock_G[SR];
			break;
		case 0x4000800c:
			reg_p = &MicBlock_G[APR];
			break;
		case 0x40008010:
			reg_p = &MicBlock_G[ATR];
			break;
		case 0x40008014:
			reg_p = &MicBlock_G[ITR];
			break;

		case 0x4000c000:
			reg_p = &Sic1Block_G[ER];
			break;
		case 0x4000c004:
			reg_p = &Sic1Block_G[RSR];
			break;
		case 0x4000c008:
			reg_p = &Sic1Block_G[SR];
			break;
		case 0x4000c00c:
			reg_p = &Sic1Block_G[APR];
			break;
		case 0x4000c010:
			reg_p = &Sic1Block_G[ATR];
			break;
		case 0x4000c014:
			reg_p = &Sic1Block_G[ITR];
			break;

		case 0x40010000:
			reg_p = &Sic2Block_G[ER];
			break;
		case 0x40010004:
			reg_p = &Sic2Block_G[RSR];
			break;
		case 0x40010008:
			reg_p = &Sic2Block_G[SR];
			break;
		case 0x4001000c:
			reg_p = &Sic2Block_G[APR];
			break;
		case 0x40010010:
			reg_p = &Sic2Block_G[ATR];
			break;
		case 0x40010014:
			reg_p = &Sic2Block_G[ITR];
			break;

		default:
			return;
	}
	if (reg_p != NULL) {
		reg_p->set = true;
		reg_p->addr = addr;
		reg_p->val = val;
	}

	/* MIC */
	cnt = 0;
	for (i=0; i<MAX_REG; ++i) {
		if (MicBlock_G[i].set)
			++cnt;
	}
	if (cnt == MAX_REG) {
		process_MIC_block();
		memset(MicBlock_G, 0, sizeof(MicBlock_G));
	}

	/* SIC1 */
	cnt = 0;
	for (i=0; i<MAX_REG; ++i) {
		if (Sic1Block_G[i].set)
			++cnt;
	}
	if (cnt == MAX_REG) {
		process_SIC1_block();
		memset(Sic1Block_G, 0, sizeof(Sic1Block_G));
	}

	/* SIC2 */
	cnt = 0;
	for (i=0; i<MAX_REG; ++i) {
		if (Sic2Block_G[i].set)
			++cnt;
	}
	if (cnt == MAX_REG) {
		process_SIC2_block();
		memset(Sic2Block_G, 0, sizeof(Sic2Block_G));
	}
}
