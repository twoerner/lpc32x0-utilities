// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registers.h"

static void
emc__sdramclkctrl (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[22]", "SDRAM_PIN_SPEED3");
	field = print_field(val, 22, 22);
	printf("%s slew rate\n", field == 1? "slow" : "fast");

	printf(ITEMFMT, "[21]", "SDRAM_PIN_SPEED2");
	field = print_field(val, 21, 21);
	printf("%s slew rate\n", field == 1? "slow" : "fast");

	printf(ITEMFMT, "[20]", "SDRAM_PIN_SPEED1");
	field = print_field(val, 20, 20);
	printf("%s slew rate\n", field == 1? "slow" : "fast");

	printf(ITEMFMT, "[19]", "SW_DDR_RESET");
	field = print_field(val, 19, 19);
	printf("%s EMC reset\n", field == 1? "active" : "no");

	printf(ITEMFMT, "[18:14]", "COMMAND_DELAY");
	field = print_field(val, 18, 14);
	printf("%u\n", field);

	printf(ITEMFMT, "[13]", "delay circuitry adder status");
	field = print_field(val, 13, 13);
	if (field == 1)
		printf("last calibration produced either an overflow or a negative number");
	else
		printf("no overflow or sign bit");
	printf("\n");

	printf(ITEMFMT, "[12:10]", "sensitivity factor for DDR SDRAM calibration");
	field = print_field(val, 12, 10);
	printf("shift left with %u\n", field);

	printf(ITEMFMT, "[9]", "calibrated delay");
	field = print_field(val, 9, 9);
	printf("use %scalibrated delay settings for DDR SDRAM\n", field == 1? "" : "un-");

	printf(ITEMFMT, "[8]", "software DDR calibration");
	field = print_field(val, 8, 8);
	printf("%s manual DDR delay calibration\n", field == 1? "perform" : "no");

	printf(ITEMFMT, "[7]", "RTC delay calibration");
	field = print_field(val, 7, 7);
	printf("%s automatic DDR delay calibration on each RTC TICK\n", field == 1? "enable" : "no");

	printf(ITEMFMT, "[6:2]", "DDR_DQSIN_DELAY");
	field = print_field(val, 6, 2);
	printf("%u\n", field);

	printf(ITEMFMT, "[1]", "DDR_SEL");
	field = print_field(val, 1, 1);
	printf("SDR SDRAM is used");
	if (field == 1)
		printf(" and DQS delay is enabled");
	printf("\n");

	printf(ITEMFMT, "[0]", "");
	field = print_field(val, 0, 0);
	if (field == 1)
		printf("all clocks to EMC block disabled");
	else
		printf("SDRAM HCLK and inverted HCLK enabled");
	printf("\n");
}

static void
emc__emccontrol (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[2]", "low-power mode");
	field = print_field(val, 2, 2);
	printf("%s mode\n", field == 1? "low-power" : "normal");

	printf(ITEMFMT, "[0]", "EMC enable");
	field = print_field(val, 0, 0);
	printf("%sabled\n", field == 1? "en" : "dis");
}

static void
emc__emcstatus (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[2]", "self-refresh acknowledge");
	field = print_field(val, 2, 2);
	printf("%s mode\n", field == 1? "self-refresh" : "normal");

	printf(ITEMFMT, "[0]", "busy");
	field = print_field(val, 0, 0);
	if (field == 1)
		printf("EMC is busy performing memory transactions, commands, auto-refresh cycles, or in self-refresh mode");
	else
		printf("EMC is idle");
	printf("\n");
}

static void
emc__emcconfig (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[0]", "endian mode");
	field = print_field(val, 0, 0);
	printf("%s-endian mode\n", field == 1? "big" : "little");
}

static void
emc__emcdynamiccontrol (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[13]", "power mode");
	field = print_field(val, 13, 13);
	if (field == 1)
		printf("enter deep power down mode");
	else
		printf("normal operation");
	printf("\n");

	printf(ITEMFMT, "[8:7]", "SDRAM initialization");
	field = print_field(val, 8, 7);
	switch (field) {
		case 0:
			printf("issue SDRAM NORMAL operation command");
			break;
		case 1:
			printf("issue SDRAM MODE command");
			break;
		case 2:
			printf("issue SDRAM PALL (precharge all) command");
			break;
		case 3:
			printf("issue SDRAM NOP command");
			break;
		default:
			printf("?");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[5]", "memory clock control (MCC)");
	field = print_field(val, 5, 5);
	printf("EMC_CLK %sabled\n", field == 1? "dis" : "en");

	printf(ITEMFMT, "[4]", "inverted memory clock control (IMCC)");
	field = print_field(val, 4, 4);
	printf("EMC_CLK_N %sabled\n", field == 1? "dis" : "en");

	printf(ITEMFMT, "[3]", "self-refresh memory clock control (SRMCC)");
	field = print_field(val, 3, 3);
	printf("EMC_CLK and EMC_CLK_N %s during self-refresh mode\n", field == 1? "are stopped" : "run continuously");

	printf(ITEMFMT, "[2]", "self-refresh request");
	field = print_field(val, 2, 2);
	printf("this bit must be 0 for correct operation; default is 1\n");

	printf(ITEMFMT, "[1]", "dynamic memory clock control");
	field = print_field(val, 1, 1);
	printf("EMC_CLK %s when all SDRAMs are idle and during self-refresh mode\n",
			field == 1? "runs continuously" : "stops");

	printf(ITEMFMT, "[0]", "dynamic memory clock enable");
	field = print_field(val, 0, 0);
	if (field == 1)
		printf("all clock enables are driven HIGH continuously");
	else
		printf("clock enable of idle devices are deasserted to save power");
	printf("\n");
}

static void
emc__emcdynamicrefresh (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[10:0]", "refresh timer");
	field = print_field(val, 10, 0);
	printf("%u\n", field);
}

static void
emc__emcdynamicreadconfig (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[12]", "DDR_DRP: DDR SDRAM read data capture polarity");
	field = print_field(val, 12, 12);
	printf("data captured on the %s edge of HCLK\n", field == 1? "positive" : "negative");

	printf(ITEMFMT, "[9:8]", "DDR_DRD: DDR SDRAM read data strategy");
	field = print_field(val, 9, 8);
	switch (field) {
		case 1:
			printf("command delayed by COMMAND_DELAY time");
			break;
		default:
			printf("(reserved)");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[4]", "SDR_SRP: SDR SDRAM read data capture polarity");
	field = print_field(val, 4, 4);
	printf("data captured on %s edge of HCLK\n", field == 1? "positive" : "negative");

	printf(ITEMFMT, "[1:0]", "SDR_SRD: SDR SDRAM read data strategy");
	field = print_field(val, 1, 0);
	switch (field) {
		case 1:
			printf("command delayed by COMMAND_DELAY time");
			break;
		default:
			printf("(reserved)");
			break;
	}
	printf("\n");
}

static void
emc__emcdynamictrp (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3:0]", "precharge command period (tRP)");
	field = print_field(val, 3, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcdynamictras (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3:0]", "active to precharge command period (tRAS)");
	field = print_field(val, 3, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcdynamictsrex (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[6:0]", "self-refresh exit time (tSREX)");
	field = print_field(val, 6, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcdynamictwr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3:0]", "write recovery time (tWR)");
	field = print_field(val, 3, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcdynamictrc (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[4:0]", "active to active command period (tRC)");
	field = print_field(val, 4, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcdynamictrfc (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[4:0]", "auto-refresh period and auto-refresh to active command period (tRFC)");
	field = print_field(val, 4, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcdynamictxsr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[7:0]", "exit self-refresh to active command time (tXSR)");
	field = print_field(val, 7, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcdynamictrrd (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3:0]", "active bank A to active bank B latency (tRRD)");
	field = print_field(val, 3, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcdynamictmrd (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3:0]", "load mode register to active command time (tMRD)");
	field = print_field(val, 3, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcdynamiccdlr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3:0]", "last data in to read command time (tCDLR)");
	field = print_field(val, 3, 0);
	printf("%u clock cycles\n", field+1);
}

static void
emc__emcstaticextendedwait (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[9:0]", "extended wait time out (in HCLK cycles)");
	field = print_field(val, 9, 0);
	printf("%u\n", field);
}

/*
 * Some of the values in the address mapping table are guessed since they
 * must be wrong in the "LPC32x0 User Manual" UM10326. I assume they're wrong
 * because duplicate values exist, how can the same value stand for two
 * different memory configurations?
 */
static void
emc__emcdynamicconfigN (uint32_t val)
{
#define EMCFMT "\t\t\t%-15s  %-5s  %-7s  %-7s"
	uint32_t field;

	printf(ITEMFMT, "[20]", "write protect");
	field = print_field(val, 20, 20);
	printf("writes%s protected\n", field == 1? "" : " not");

	printf(ITEMFMT, "[14:7]", "address mapping");
	field = print_field(val, 14, 7);
	printf("\n");
	printf(EMCFMT, "desc", "banks", "rowlen", "collen");
	printf("\n");
	switch (field) {
		/* 16-bit high-performance */
		case 0x00: printf(EMCFMT, "16Mb (2Mx8)",   "2", "11", "9"); break;
		case 0x01: printf(EMCFMT, "16Mb (1Mx16)",  "2", "11", "8"); break;
		case 0x04: printf(EMCFMT, "64Mb (8Mx8)",   "4", "12", "9"); break;
		case 0x05: printf(EMCFMT, "64Mb (4Mx16)",  "4", "12", "8"); break;
		case 0x08: printf(EMCFMT, "128Mb (16Mx8)", "4", "12", "10");break;
		case 0x09: printf(EMCFMT, "128Mb (8Mx16)", "4", "12", "9"); break;
		case 0x0c: printf(EMCFMT, "256Mb (32Mx8)", "4", "13", "10");break;
		case 0x0d: printf(EMCFMT, "256Mb (16Mx16)","4", "13", "9"); break;
		case 0x10: printf(EMCFMT, "512Mb (64Mx8)", "4", "13", "11");break;
		case 0x11: printf(EMCFMT, "512Mb (32Mx16)","4", "13", "10");break;

		/* 16-bit low-power */
		case 0x20: printf(EMCFMT, "16Mb (2Mx8)",   "2", "11", "9"); break;
		case 0x21: printf(EMCFMT, "16Mb (1Mx16)",  "2", "11", "8"); break;
		case 0x24: printf(EMCFMT, "64Mb (8Mx8)",   "4", "12", "9"); break;
		case 0x25: printf(EMCFMT, "64Mb (4Mx16)",  "4", "12", "8"); break;
		case 0x28: printf(EMCFMT, "128Mb (16Mx8)", "4", "12", "10");break;
		case 0x29: printf(EMCFMT, "128Mb (8Mx16)", "4", "12", "9"); break;
		case 0x2c: printf(EMCFMT, "256Mb (32Mx8)", "4", "13", "10");break;
		case 0x2d: printf(EMCFMT, "256Mb (16Mx16)","4", "13", "9"); break;
		case 0x30: printf(EMCFMT, "512Mb (64Mx8)", "4", "13", "11");break;
		case 0x31: printf(EMCFMT, "512Mb (32Mx16)","4", "13", "10");break;

		/* 32-bit high-performance */
		case 0x80: printf(EMCFMT, "16Mb (2Mx8)",   "2", "11", "9"); break;
		case 0x81: printf(EMCFMT, "16Mb (1Mx16)",  "2", "11", "8"); break;
		case 0x84: printf(EMCFMT, "64Mb (8Mx8)",   "4", "12", "9"); break;
		case 0x85: printf(EMCFMT, "64Mb (4Mx16)",  "4", "12", "8"); break;
		case 0x86: printf(EMCFMT, "64Mb (2Mx32)",  "4", "11", "8"); break;
		case 0x88: printf(EMCFMT, "128Mb (16Mx8)", "4", "12", "10");break;
		case 0x89: printf(EMCFMT, "128Mb (8Mx16)", "4", "12", "9"); break;
		case 0x8a: printf(EMCFMT, "128Mb (4Mx32)", "4", "12", "8"); break;
		case 0x8c: printf(EMCFMT, "256Mb (32Mx8)", "4", "13", "10");break;
		case 0x8d: printf(EMCFMT, "256Mb (16Mx16)","4", "13", "9"); break;
		case 0x8e: printf(EMCFMT, "256Mb (8Mx32)", "4", "13", "8"); break;
		case 0x8f: printf(EMCFMT, "256Mb (4Mx64)", "4", "12", "9"); break;
		case 0x90: printf(EMCFMT, "512Mb (64Mx8)", "4", "13", "11");break;
		case 0x91: printf(EMCFMT, "512Mb (32Mx16)","4", "13", "10");break;
		case 0x92: printf(EMCFMT, "512Mb (16Mx32)","4", "13", "9"); break;
		case 0x99: printf(EMCFMT, "1Gb (32Mx32)",  "4", "13", "10");break;

		/* 32-bit low-power */
		case 0xa0: printf(EMCFMT, "16Mb (2Mx8)",   "2", "11", "9"); break;
		case 0xa1: printf(EMCFMT, "16Mb (1Mx16)",  "2", "11", "8"); break;
		case 0xa4: printf(EMCFMT, "64Mb (8Mx8)",   "4", "12", "9"); break;
		case 0xa5: printf(EMCFMT, "64Mb (4Mx16)",  "4", "12", "8"); break;
		case 0xa6: printf(EMCFMT, "64Mb (2Mx32)",  "4", "11", "8"); break;
		case 0xa8: printf(EMCFMT, "128Mb (16Mx8)", "4", "12", "10");break;
		case 0xa9: printf(EMCFMT, "128Mb (8Mx16)", "4", "12", "9"); break;
		case 0xaa: printf(EMCFMT, "128Mb (4Mx32)", "4", "12", "8"); break;
		case 0xac: printf(EMCFMT, "256Mb (32Mx8)", "4", "13", "10");break;
		case 0xad: printf(EMCFMT, "256Mb (16Mx16)","4", "13", "9"); break;
		case 0xae: printf(EMCFMT, "256Mb (8Mx32)", "4", "13", "8"); break;
		case 0xaf: printf(EMCFMT, "256Mb (4Mx64)", "4", "12", "9"); break;
		case 0xb0: printf(EMCFMT, "512Mb (64Mx8)", "4", "13", "11");break;
		case 0xb1: printf(EMCFMT, "512Mb (32Mx16)","4", "13", "10");break;
		case 0xb2: printf(EMCFMT, "512Mb (16Mx32)","4", "13", "9"); break;
		case 0xb9: printf(EMCFMT, "1Gb (32Mx32)",  "4", "13", "10");break;

		default:
			printf("?");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[2:0]", "memory device");
	field = print_field(val, 2, 0);
	switch (field) {
		case 0:
			printf("SDR SDRAM");
			break;
		case 2:
			printf("low-power SDR SDRAM");
			break;
		case 4:
			printf("DDR SDRAM");
			break;
		case 6:
			printf("low-power DDR SDRAM");
			break;
		default:
			printf("(reserved)");
			break;
	}
}

static void
emc__emcdynamicrascasN (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[10:7]", "CAS latency");
	field = print_field(val, 10, 7);
	if (field == 0)
		printf("(reserved)\n");
	else
		printf("%u.%u clock cycle(s)\n", (field>>1), ((field&1)? 5 : 0));

	printf(ITEMFMT, "[3:0]", "RAS latency");
	field = print_field(val, 3, 0);
	if (field == 0)
		printf("(reserved)\n");
	else
		printf("%u clock cycles\n", field);
}

static void emc__emcdynamicconfig0 (uint32_t val) { emc__emcdynamicconfigN(val); }
static void emc__emcdynamicconfig1 (uint32_t val) { emc__emcdynamicconfigN(val); }
static void emc__emcdynamicrascas0 (uint32_t val) { emc__emcdynamicrascasN(val); }
static void emc__emcdynamicrascas1 (uint32_t val) { emc__emcdynamicrascasN(val); }

static void
emc__emcstaticconfigN (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[20]", "write protect");
	field = print_field(val, 20, 20);
	printf("writes%s protected\n", field == 1? "" : " not");

	printf(ITEMFMT, "[8]", "extended wait");
	field = print_field(val, 8, 8);
	printf("extended wait %sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[7]", "byte lane state");
	field = print_field(val, 7, 7);
	if (field == 1)
		printf("EMC_BLSn[3:0] strobes only active during writes");
	else
		printf("EMC_BLSn[3:0] strobes active during read and write access");
	printf("\n");

	printf(ITEMFMT, "[6]", "chip select polarity");
	field = print_field(val, 6, 6);
	printf("active %s chip select\n", field == 1? "HIGH" : "LOW");

	printf(ITEMFMT, "[3]", "page mode");
	field = print_field(val, 3, 3);
	printf("async page mode %sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[1:0]", "memory width");
	field = print_field(val, 1, 0);
	switch (field) {
		case 0:
			printf("8-bit");
			break;
		case 1:
			printf("16-bit");
			break;
		case 2:
			printf("32-bit");
			break;
		default:
			printf("(reserved)");
			break;
	}
	printf("\n");
}

static void
emc__emcstaticwaitwenN (uint32_t val)
{
	printf(ITEMFMT, "[3:0]", "wait write enable");
	print_field(val, 3, 0);
}

static void
emc__emcstaticwaitoenN (uint32_t val)
{
	printf(ITEMFMT, "[3:0]", "wait output enable");
	print_field(val, 3, 0);
}

static void
emc__emcstaticwaitrdN (uint32_t val)
{
	printf(ITEMFMT, "[4:0]", "non-page mode read wait states of async page mode readfirst access wait state");
	print_field(val, 4, 0);
}

static void
emc__emcstaticwaitpageN (uint32_t val)
{
	printf(ITEMFMT, "[4:0]", "async page mode read after the first read wait states");
	print_field(val, 4, 0);
}

static void
emc__emcstaticwaitwrN (uint32_t val)
{
	printf(ITEMFMT, "[4:0]", "write wait states");
	print_field(val, 4, 0);
}

static void
emc__emcstaticwaitturnN (uint32_t val)
{
	printf(ITEMFMT, "[3:0]", "bus turn-around cycles");
	print_field(val, 3, 0);
}

static void emc__emcstaticconfig0 (uint32_t val) { emc__emcstaticconfigN(val); }
static void emc__emcstaticwaitwen0 (uint32_t val) { emc__emcstaticwaitwenN(val); }
static void emc__emcstaticwaitoen0 (uint32_t val) { emc__emcstaticwaitoenN(val); }
static void emc__emcstaticwaitrd0 (uint32_t val) { emc__emcstaticwaitrdN(val); }
static void emc__emcstaticwaitpage0 (uint32_t val) { emc__emcstaticwaitpageN(val); }
static void emc__emcstaticwaitwr0 (uint32_t val) { emc__emcstaticwaitwrN(val); }
static void emc__emcstaticwaitturn0 (uint32_t val) { emc__emcstaticwaitturnN(val); }

static void emc__emcstaticconfig1 (uint32_t val) { emc__emcstaticconfigN(val); }
static void emc__emcstaticwaitwen1 (uint32_t val) { emc__emcstaticwaitwenN(val); }
static void emc__emcstaticwaitoen1 (uint32_t val) { emc__emcstaticwaitoenN(val); }
static void emc__emcstaticwaitrd1 (uint32_t val) { emc__emcstaticwaitrdN(val); }
static void emc__emcstaticwaitpage1 (uint32_t val) { emc__emcstaticwaitpageN(val); }
static void emc__emcstaticwaitwr1 (uint32_t val) { emc__emcstaticwaitwrN(val); }
static void emc__emcstaticwaitturn1 (uint32_t val) { emc__emcstaticwaitturnN(val); }

static void emc__emcstaticconfig2 (uint32_t val) { emc__emcstaticconfigN(val); }
static void emc__emcstaticwaitwen2 (uint32_t val) { emc__emcstaticwaitwenN(val); }
static void emc__emcstaticwaitoen2 (uint32_t val) { emc__emcstaticwaitoenN(val); }
static void emc__emcstaticwaitrd2 (uint32_t val) { emc__emcstaticwaitrdN(val); }
static void emc__emcstaticwaitpage2 (uint32_t val) { emc__emcstaticwaitpageN(val); }
static void emc__emcstaticwaitwr2 (uint32_t val) { emc__emcstaticwaitwrN(val); }
static void emc__emcstaticwaitturn2 (uint32_t val) { emc__emcstaticwaitturnN(val); }

static void emc__emcstaticconfig3 (uint32_t val) { emc__emcstaticconfigN(val); }
static void emc__emcstaticwaitwen3 (uint32_t val) { emc__emcstaticwaitwenN(val); }
static void emc__emcstaticwaitoen3 (uint32_t val) { emc__emcstaticwaitoenN(val); }
static void emc__emcstaticwaitrd3 (uint32_t val) { emc__emcstaticwaitrdN(val); }
static void emc__emcstaticwaitpage3 (uint32_t val) { emc__emcstaticwaitpageN(val); }
static void emc__emcstaticwaitwr3 (uint32_t val) { emc__emcstaticwaitwrN(val); }
static void emc__emcstaticwaitturn3 (uint32_t val) { emc__emcstaticwaitturnN(val); }

static void
emc__emcahbcontrolN (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[0]", "AHB port buffer enable");
	field = print_field(val, 0, 0);
	printf("%sable buffer\n", field == 1? "en" : "dis");
}

static void
emc__emcahbstatusN (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "AHB port buffer status");
	field = print_field(val, 1, 1);
	printf("buffer %s\n", field == 1? "contains data" : "empty");
}

static void
emc__emcahbtimeoutN (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[9:0]", "AHB timeout");
	field = print_field(val, 9, 0);
	if (field == 0)
		printf("timeout disabled");
	else
		printf("0x%x number of AHB timeout cycles\n", field);
}

static void emc__emcahbcontrol0 (uint32_t val) { emc__emcahbcontrolN(val); }
static void emc__emcahbstatus0 (uint32_t val) { emc__emcahbstatusN(val); }
static void emc__emcahbtimeout0 (uint32_t val) { emc__emcahbtimeoutN(val); }

static void emc__emcahbcontrol3 (uint32_t val) { emc__emcahbcontrolN(val); }
static void emc__emcahbstatus3 (uint32_t val) { emc__emcahbstatusN(val); }
static void emc__emcahbtimeout3 (uint32_t val) { emc__emcahbtimeoutN(val); }

static void emc__emcahbcontrol4 (uint32_t val) { emc__emcahbcontrolN(val); }
static void emc__emcahbstatus4 (uint32_t val) { emc__emcahbstatusN(val); }
static void emc__emcahbtimeout4 (uint32_t val) { emc__emcahbtimeoutN(val); }

static void
emc__ddrlapnom (uint32_t unused val)
{
	printf(ITEMFMT, "[31:0]", "nominal count, written by software, factoring process, voltage, and temperature");
}

static void
emc__ddrlapcount (uint32_t unused val)
{
	printf(ITEMFMT, "[31:0]", "DDR SDRAM ring oscillator counter");
}

static void
emc__ddrcaldelay (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[4:0]", "current calibrated delay setting");
	field = print_field(val, 4, 0);
	printf("0x%x\n", field);
}

RegisterDescription_t emc[] = {
	{0x40004068, 0x00000000, "SDRAMCLK_CTRL", "controls various SDRAM configuration details", accessRW, emc__sdramclkctrl},
	{0x31080000, 0x00000003, "EMCControl", "controls operation of the EMC", accessRW, emc__emccontrol},
	{0x31080004, 0x00000005, "EMCStatus", "provides EMC status information", accessRead, emc__emcstatus},
	{0x31080008, 0x00000000, "EMCConfig", "configures operation of the EMC", accessRW, emc__emcconfig},
	{0x31080020, 0x00000006, "EMCDynamicControl", "configures operation of the EMC", accessRW, emc__emcdynamiccontrol},
	{0x31080024, 0x00000000, "EMCDynamicRefresh", "controls dynamic memory operation", accessRW, emc__emcdynamicrefresh},
	{0x31080028, 0x00000000, "EMCDynamicReadConfig", "configures the dynamic memory read strategy", accessRW, emc__emcdynamicreadconfig},
	{0x31080030, 0x0000000f, "EMCDynamictRP", "selects the precharge command period", accessRW, emc__emcdynamictrp},
	{0x31080034, 0x0000000f, "EMCDynamictRAS", "selects the active to precharge command preiod", accessRW, emc__emcdynamictras},
	{0x31080038, 0x0000000f, "EMCDynamictSREX", "selects the self-refresh exit time", accessRW, emc__emcdynamictsrex},
	{0x31080044, 0x0000000f, "EMCDynamictWR", "selects the write recovery time", accessRW, emc__emcdynamictwr},
	{0x31080048, 0x0000001f, "EMCDynamictRC", "selects the active to active command period", accessRW, emc__emcdynamictrc},
	{0x3108004c, 0x0000001f, "EMCDynamictRFC", "selects the auto-refresh period", accessRW, emc__emcdynamictrfc},
	{0x31080050, 0x0000001f, "EMCDynamictXSR", "selects the exit self-refresh to active command time", accessRW, emc__emcdynamictxsr},
	{0x31080054, 0x0000000f, "EMCDynamictRRD", "selects the active bank A to active bank B latency", accessRW, emc__emcdynamictrrd},
	{0x31080058, 0x0000000f, "EMCDynamictMRD", "selects the load mode register to active command time", accessRW, emc__emcdynamictmrd},
	{0x3108005c, 0x0000000f, "EMCDynamictCDLR", "selects the last data in to read command time", accessRW, emc__emcdynamiccdlr},
	{0x31080080, 0x0000000f, "EMCStaticExtendedWait", "selects time for long static memory read and write tranxfers", accessRW, emc__emcstaticextendedwait},
	{0x31080100, 0,          "EMCDynamicConfig0", "selects the configuration information for the SDRAM", accessRW, emc__emcdynamicconfig0},
	{0x31080104, 0x00000303, "EMCDynamicRasCas0", "selects the RAS and CAS latencies for the SDRAM", accessRW, emc__emcdynamicrascas0},
	{0x31080120, 0,          "EMCDynamicConfig1", "selects the configuration information for the SDRAM", accessRW, emc__emcdynamicconfig1},
	{0x31080124, 0x00000303, "EMCDynamicRasCas1", "selects the RAS and CAS latencies for the SDRAM", accessRW, emc__emcdynamicrascas1},
	{0x31080200, 0x00000002, "EMCStaticConfig0", "selects the memory configuration for static chip select 0", accessRW, emc__emcstaticconfig0},
	{0x31080204, 0,          "EMCStaticWaitWen0", "selects the delay from chip select 0 to write enable", accessRW, emc__emcstaticwaitwen0},
	{0x31080208, 0,          "EMCStaticWaitOen0", "selects the delay from chip select 0 or address change whichever is later, to output enable", accessRW, emc__emcstaticwaitoen0},
	{0x3108020c, 0x0000001f, "EMCStaticWaitRd0", "selects the delay from chip select 0 to a read access", accessRW, emc__emcstaticwaitrd0},
	{0x31080210, 0x0000001f, "EMCStaticWaitPage0", "selects the delay from asynchronous page mode sequential accesses for chip select 0", accessRW, emc__emcstaticwaitpage0},
	{0x31080214, 0x0000001f, "EMCStaticWaitWr0", "selects the delay from chip select 0 to a write access", accessRW, emc__emcstaticwaitwr0},
	{0x31080218, 0x0000000f, "EMCStaticWaitTurn0", "selects the number of bus turnaround cycles for chip select 0", accessRW, emc__emcstaticwaitturn0},
	{0x31080220, 0x00000002, "EMCStaticConfig1", "selects the memory configuration for static chip select 1", accessRW, emc__emcstaticconfig1},
	{0x31080224, 0,          "EMCStaticWaitWen1", "selects the delay from chip select 1 to write enable", accessRW, emc__emcstaticwaitwen1},
	{0x31080228, 0,          "EMCStaticWaitOen1", "selects the delay from chip select 1 or address change, whichever is later, to output enable", accessRW, emc__emcstaticwaitoen1},
	{0x3108022c, 0x0000001f, "EMCStaticWaitRd1", "selects the delay from chip select 1 to a read access", accessRW, emc__emcstaticwaitrd1},
	{0x31080230, 0x0000001f, "EMCStaticWaitPage1", "selects the delay from asynchronous page mode sequential access for chip select 1", accessRW, emc__emcstaticwaitpage1},
	{0x31080234, 0x0000001f, "EMCStaticWaitWr1", "selects the delay from chip select 1 to a write access", accessRW, emc__emcstaticwaitwr1},
	{0x31080238, 0x0000000f, "EMCStaticWaitTurn1", "selects the number of bus turnaround cycles for chip select 1", accessRW, emc__emcstaticwaitturn1},
	{0x31080240, 0x00000002, "EMCStaticConfig2", "selects the memory configuration for static chip select 2", accessRW, emc__emcstaticconfig2},
	{0x31080244, 0,          "EMCStaticWaitWen2", "selects the delay from chip select 2 to write enable", accessRW, emc__emcstaticwaitwen2},
	{0x31080248, 0,          "EMCStaticWaitOen2", "selects the delay from chip select 2 or address change, whichever is later, to output enable", accessRW, emc__emcstaticwaitoen2},
	{0x3108024c, 0x0000001f, "EMCStaticWaitRd2", "selects the delay from chip select 2 to a read access", accessRW, emc__emcstaticwaitrd2},
	{0x31080250, 0x0000001f, "EMCStaticWaitPage2", "selects the delay for asynchronous page mode sequential accesses for chip select 2", accessRW, emc__emcstaticwaitpage2},
	{0x31080254, 0x0000001f, "EMCStaticWaitWr2", "selects the delay from chip select 2 to a write access", accessRW, emc__emcstaticwaitwr2},
	{0x31080258, 0x0000000f, "EMCStaticWaitTurn2", "selects the number of bus turnaround cycles for chip select 2", accessRW, emc__emcstaticwaitturn2},
	{0x31080260, 0x00000002, "EMCStaticConfig3", "selects the memory configuration for static chip select 3", accessRW, emc__emcstaticconfig3},
	{0x31080264, 0,          "EMCStaticWaitWen3", "selects the delay from chip select 3 to write enable", accessRW, emc__emcstaticwaitwen3},
	{0x31080268, 0,          "EMCStaticWaitOen3", "selects the delay from chip select3 or address change, whichever is later, to output enable", accessRW, emc__emcstaticwaitoen3},
	{0x3108026c, 0x0000001f, "EMCStaticWaitRd3", "selects the delay from chip select 3 to a read access", accessRW, emc__emcstaticwaitrd3},
	{0x31080270, 0x0000001f, "EMCStaticWaitPage3", "selects the delay for asynchronous page mode sequential accesses for chip select 3", accessRW, emc__emcstaticwaitpage3},
	{0x31080274, 0x0000001f, "EMCStaticWaitWr3", "selects the delay from chip select 3 to a write access", accessRW, emc__emcstaticwaitwr3},
	{0x31080278, 0x0000000f, "EMCStaticWaitTurn3", "selects the delay from chip select 3 to a write access", accessRW, emc__emcstaticwaitturn3},
	{0x31080400, 0,          "EMCAHBControl0", "control register for AHB port 0", accessRW, emc__emcahbcontrol0},
	{0x31080404, 0,          "EMCAHBStatus0", "status register for AHB port 0", accessRW, emc__emcahbstatus0},
	{0x31080408, 0,          "EMCAHBTimeOut0", "timeout register for AHB port 0", accessRW, emc__emcahbtimeout0},
	{0x31080460, 0,          "EMCAHBControl3", "control register for AHB port 3", accessRW, emc__emcahbcontrol3},
	{0x31080464, 0,          "EMCAHBStatus3", "status register for AHB port 3", accessRW, emc__emcahbstatus3},
	{0x31080468, 0,          "EMCAHBTimeOut3", "timeout register for AHB port 3", accessRW, emc__emcahbtimeout3},
	{0x31080480, 0,          "EMCAHBControl4", "control register for AHB port 4", accessRW, emc__emcahbcontrol4},
	{0x31080484, 0,          "EMCAHBStatus4", "status register for AHB port 4", accessRW, emc__emcahbstatus4},
	{0x31080488, 0,          "EMCAHBTimeOut4", "timeout register for AHB port 4", accessRW, emc__emcahbtimeout4},
	{0x4000406c, 0,          "DDR_LAP_NOM", "nominal value for DDR DQS input delay", accessRW, emc__ddrlapnom},
	{0x40004070, 0,          "DDR_LAP_COUNT", "DDR SDRAM ring oscillator counter", accessRead, emc__ddrlapcount},
	{0x40004074, 0,          "DDR_CAL_DELAY", "calibrated value of the DDR DQS input delay", accessRead, emc__ddrcaldelay},
};
size_t emcSZ = (sizeof(emc)/sizeof(emc[0]));
