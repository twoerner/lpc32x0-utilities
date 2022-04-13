// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registers.h"

static void
lcd__lcdtimh (uint32_t val)
{
	printf(ITEMFMT, "[31:24]", "HBP - horizontal back porch");
	printf("\t\t\t0x%x\n", get_field(val, 31, 24));

	printf(ITEMFMT, "[23:16]", "HFP - horizontal front porch");
	printf("\t\t\t0x%x\n", get_field(val, 23, 16));

	printf(ITEMFMT, "[15:8]", "HSW - horizontal synchronization pulse width");
	printf("\t\t\t0x%x\n", get_field(val, 15, 8));

	printf(ITEMFMT, "[7:2]", "PPL - pixels per line");
	printf("\t\t\t0x%x\n", get_field(val, 7, 2));
}

static void
lcd__lcdtimv (uint32_t val)
{
	printf(ITEMFMT, "[31:24]", "VBP - vertial back porch");
	printf("\t\t\t0x%x\n", get_field(val, 31, 24));

	printf(ITEMFMT, "[23:16]", "VFP - vertical front porch");
	printf("\t\t\t0x%x\n", get_field(val, 23, 16));

	printf(ITEMFMT, "[15:10]", "VSW - vertical synchronization pulse width");
	printf("\t\t\t0x%x\n", get_field(val, 15, 0));

	printf(ITEMFMT, "[9:0]", "LLP - lines per panel");
	printf("\t\t\t0x%x\n", get_field(val, 9, 0));
}

static void
lcd__lcdpol (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:27]", "PCD_HI - panel clock divider, upper five bits");
	printf("\t\t\t0x%x\n", get_field(val, 31, 27));

	printf(ITEMFMT, "[26]", "BCD - bypass pixel clock divider");
	field = print_field(val, 26, 26);
	printf("%sbypass pixel clock divider logic\n", field == 0? "don't " : "");

	printf(ITEMFMT, "[25:16]", "CPL - clocks per line");
	printf("\t\t\t0x%x\n", get_field(val, 25, 16));

	printf(ITEMFMT, "[14]", "IOE - invert output enable");
	field = print_field(val, 14, 14);
	printf("LCDENAB output pin is active %s in TFT mode\n", field == 0? "HIGH" : "LOW");

	printf(ITEMFMT, "[13]", "IPC - invert panel clock");
	field = print_field(val, 13, 13);
	printf("data is driven on the LCD data lines on the %s edge of LCDDCLK\n", field == 0? "rising" : "falling");

	printf(ITEMFMT, "[12]", "IHS - invert horizontal synchronization");
	field = print_field(val, 12, 12);
	printf("LCDLP pin is active %s and inactive %s\n",
			field == 0? "HIGH" : "LOW",
			field == 0? "LOW" : "HIGH");

	printf(ITEMFMT, "[11]", "IVS - invert vertical synchronization");
	field = print_field(val, 11, 11);
	printf("LCDFP pin is active %s and inactive %s\n",
			field == 0? "HIGH" : "LOW",
			field == 0? "LOW" : "HIGH");

	printf(ITEMFMT, "[10:6]", "ACB - AC bian pin frequency");
	printf("\t\t\t0x%x\n", get_field(val, 10, 6));

	printf(ITEMFMT, "[5]", "CLKSEL");
	field = print_field(val, 5, 5);
	printf("the clock source for the LCD block is %s\n", field == 0? "HCLK" : "LCDCLKIN");

	printf(ITEMFMT, "[4:0]", "PCD_LO - panel clock divider, lower five bits");
	printf("\t\t\t0x%x\n", get_field(val, 4, 0));
}

static void
lcd__lcdle (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[16]", "LEE - LCD line end enable");
	field = print_field(val, 16, 16);
	if (field == 0)
		printf("LCDLE disabled (held LOW)");
	else
		printf("LCDLE signal active");
	printf("\n");

	printf(ITEMFMT, "[6:0]", "LED - line end delay");
	printf("\t\t\t0x%x\n", get_field(val, 6, 0));
}

static void
lcd__lcdupbase (uint32_t val)
{
	printf(ITEMFMT, "[31:3]", "LCDUPBASE - LCD upper panel base address");
	printf("\t\t\t0x%08x\n", get_field(val, 31, 3));
}

static void
lcd__lcdlpbase (uint32_t val)
{
	printf(ITEMFMT, "[31:3]", "LCDLPBASE - LCD lower panel base address");
	printf("\t\t\t0x%08x\n", get_field(val, 31, 3));
}

static void
lcd__lcdctrl (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[16]", "WATERMARK - LCD DMA FIFO watermark level");
	field = print_field(val, 16, 16);
	printf("LCD DMA request generated with %u or more empty locations\n", field == 0? 4 : 8);

	printf(ITEMFMT, "[13:12]", "LcdVComp - LCD vertical compare interrupt, generate VComp interrupt at...");
	field = print_field(val, 13, 12);
	switch (field) {
		case 0:
			printf("start of vertical synchronization");
			break;
		case 1:
			printf("start of back porch");
			break;
		case 2:
			printf("start of active video");
			break;
		case 3:
			printf("start of front porch");
			break;
		default:
			printf("?");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[11]", "LcdPwr - LCD power enable");
	field = print_field(val, 11, 11);
	if (field == 0)
		printf("power not gated through LCD, LCDV[23:0] disabled");
	else
		printf("power gated through LCD, LCDV[23:0] enabled");
	printf("\n");

	printf(ITEMFMT, "[10]", "BEPO - big-endian pixel ordering within a byte");
	field = print_field(val, 10, 10);
	if (field == 0)
		printf("little-endian pixel ordering");
	else
		printf("big-endian pixel ordering");
	printf("\n");

	printf(ITEMFMT, "[9]", "BEBO - big-endian byte order in memory");
	field = print_field(val, 9, 9);
	if (field == 0)
		printf("little-endian byte order");
	else
		printf("big-endian byte order");
	printf("\n");

	printf(ITEMFMT, "[8]", "BGR - colour format selection");
	field = print_field(val, 8, 8);
	if (field == 0)
		printf("RGB: normal output");
	else
		printf("BGR: red and blue are swapped");
	printf("\n");

	printf(ITEMFMT, "[7]", "LcdDual - single or dual LCD panel selection");
	field = print_field(val, 7, 7);
	printf("%s-panel\n", field == 0? "single" : "dual");

	printf(ITEMFMT, "[6]", "LcdMono8 - monochrome LCD interface width");
	field = print_field(val, 6, 6);
	printf("monochrome LCD uses %s-bit interface\n", field == 0? "4" : "8");

	printf(ITEMFMT, "[5]", "LcdTFT - LCD panel TFT type selection");
	field = print_field(val, 5, 5);
	if (field == 0)
		printf("LCD is an STN display, use gray scaler");
	else
		printf("LCD is a TFT display, do not use gray scaler");
	printf("\n");

	printf(ITEMFMT, "[4]", "LcdBW - STN LCD monochrome/colour selection");
	field = print_field(val, 4, 4);
	printf("STN LCD is %s\n", field == 0? "colour" : "monochrome");

	printf(ITEMFMT, "[3:1]", "LcdBpp - LCD bits per pixel");
	field = print_field(val, 3, 1);
	switch (field) {
		case 0:
			printf("1 bpp");
			break;
		case 1:
			printf("2 bpp");
			break;
		case 2:
			printf("4 bpp");
			break;
		case 3:
			printf("8 bpp");
			break;
		case 4:
			printf("16 bpp");
			break;
		case 5:
			printf("24 bpp (TFT panel only)");
			break;
		case 6:
			printf("16 bpp, 5:6:5 mode");
			break;
		case 7:
			printf("12 bpp, 4:4:4 mode");
			break;
		default:
			printf("?");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[0]", "LcdEn - LCD enable control bit");
	field = print_field(val, 0, 0);
	printf("LCD %sabled\n", field == 0? "dis" : "en");
}

static void
generic_int (uint32_t val)
{
	printf("\t\t\t%u - AHB master bus error\n", get_field(val, 4, 4));
	printf("\t\t\t%u - vertical compare interrupt\n", get_field(val, 3, 3));
	printf("\t\t\t%u - LCD next address base update raw interrupt\n", get_field(val, 2, 2));
	printf("\t\t\t%u - FIFO underflow interrupt\n", get_field(val, 1, 1));
}

static void lcd__lcdintmsk (uint32_t val) { generic_int(val); }
static void lcd__lcdintraw (uint32_t val) { generic_int(val); }
static void lcd__lcdintstat (uint32_t val) { generic_int(val); }
static void lcd__lcdintclr (uint32_t val) { generic_int(val); }

static void
lcd__lcdupcurr (uint32_t val)
{
	printf(ITEMFMT, "[31:0]", "LCDUPCURR - LCD upper panel current address");
	printf("\t\t\t0x%08x\n", val);
}

static void
lcd__lcdlpcurr (uint32_t val)
{
	printf(ITEMFMT, "[31:0]", "LCDLPCURR - LCD lower pannel current address");
	printf("\t\t\t0x%08x\n", val);
}

static void
lcd__crsrctrl (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[5:4]", "");
	print_field(val, 5, 4);
	printf("cursor number\n");

	printf(ITEMFMT, "[0]", "cursor enable");
	field = print_field(val, 0, 0);
	printf("cursor is%s displayed\n", field == 0? " not" : "");
}

static void
lcd__crsrcfg (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "frame sync");
	field = print_field(val, 1, 1);
	printf("cursor coordinates are %s to the frame sync pulse\n", field == 0? "asynchronous" : "synchronized");

	printf(ITEMFMT, "[0]", "cursor size");
	field = print_field(val, 0, 0);
	printf("%s pixel cursor\n", field == 0? "32x32" : "64x64");
}

static void
pal_config (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[23:16]", "blue");
	field = print_field(val, 23, 16);
	printf("0x%x\n", field);

	printf(ITEMFMT, "[15:8]", "green");
	field = print_field(val, 15, 8);
	printf("0x%x\n", field);

	printf(ITEMFMT, "[7:0]", "red");
	field = print_field(val, 7, 0);
	printf("0x%x\n", field);
}

static void lcd__crsrpal0 (uint32_t val) { pal_config(val); }
static void lcd__crsrpal1 (uint32_t val) { pal_config(val); }

static void
lcd__crsrxy (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[25:16]", "Y");
	field = print_field(val, 25, 16);
	printf("0x%x\n", field);

	printf(ITEMFMT, "[9:0]", "X");
	field = print_field(val, 9, 0);
	printf("0x%x\n", field);
}

static void
lcd__crsrclip (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[13:8]", "cursor clip position for Y direction");
	field = print_field(val, 13, 8);
	printf("0x%x\n", field);

	printf(ITEMFMT, "[5:0]", "cursor clip position for X direction");
	field = print_field(val, 5, 0);
	printf("0x%x\n", field);
}

static void
crsr_int (uint32_t val)
{
	printf("\t\t\t%u - cursor interrupt\n", get_field(val, 0, 0));
}

static void lcd__crsrintmsk (uint32_t val) { crsr_int(val); }
static void lcd__crsrintclr (uint32_t val) { crsr_int(val); }
static void lcd__crsrintraw (uint32_t val) { crsr_int(val); }
static void lcd__crsrintstat (uint32_t val) { crsr_int(val); }

RegisterDescription_t lcd[] = {
	{0x40004054, 0, "LCD_CFG", "LCD configuration and clocking control", accessRW, NULL},
	{0x31040000, 0, "LCD_TIMH", "horizontal timing control register", accessRW, lcd__lcdtimh},
	{0x31040004, 0, "LCD_TIMV", "vertical timing control register", accessRW, lcd__lcdtimv},
	{0x31040008, 0, "LCD_POL", "clock and signal polarity control register", accessRW, lcd__lcdpol},
	{0x3104000C, 0, "LCD_LE", "line end control register", accessRW, lcd__lcdle},
	{0x31040010, 0, "LCD_UPBASE", "upper panel frame base address register", accessRW, lcd__lcdupbase},
	{0x31040014, 0, "LCD_LPBASE", "lower panel frame base address register", accessRW, lcd__lcdlpbase},
	{0x31040018, 0, "LCD_CTRL", "LCD control register", accessRW, lcd__lcdctrl},
	{0x3104001c, 0, "LCD_INTMSK", "interrupt mask register", accessRW, lcd__lcdintmsk},
	{0x31040020, 0, "LCD_INTRAW", "raw interrupt status register", accessRead, lcd__lcdintraw},
	{0x31040024, 0, "LCD_INTSTAT", "masked interrupt status register", accessRead, lcd__lcdintstat},
	{0x31040028, 0, "LCD_INTCLR", "interrupt clear register", accessWrite, lcd__lcdintclr},
	{0x3104002c, 0, "LCD_UPCURR", "upper panel current address value register", accessRead, lcd__lcdupcurr},
	{0x31040030, 0, "LCD_LPCURR", "lower panel current address value register", accessRead, lcd__lcdlpcurr},
	{0x31040c00, 0, "CRSR_CTRL", "cursor control register", accessRW, lcd__crsrctrl},
	{0x31040c04, 0, "CRSR_CFG", "cursor configuration register", accessRW, lcd__crsrcfg},
	{0x31040c08, 0, "CRSR_PAL0", "cursor palette register 0", accessRW, lcd__crsrpal0},
	{0x31040c0c, 0, "CRSR_PAL1", "cursor palette register 1", accessRW, lcd__crsrpal1},
	{0x31040c10, 0, "CRSR_XY", "cursor XY position register", accessRW, lcd__crsrxy},
	{0x31040c14, 0, "CRSR_CLIP", "cursor clip position register", accessRW, lcd__crsrclip},
	{0x31040c20, 0, "CRSR_INTMSK", "cursor interrupt mask register", accessRW, lcd__crsrintmsk},
	{0x31040c24, 0, "CRSR_INTCLR", "cursor interrupt clear register", accessWrite, lcd__crsrintclr},
	{0x31040c28, 0, "CRSR_INTRAW", "cursor raw interrupt status register", accessRead, lcd__crsrintraw},
	{0x31040c2c, 0, "CRSR_INTSTAT", "cursor masked interrupt status register", accessRead, lcd__crsrintstat},
};
size_t lcdSZ = (sizeof(lcd)/sizeof(lcd[0]));
