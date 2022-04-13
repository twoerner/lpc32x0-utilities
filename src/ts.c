// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registers.h"

static void
ts__adclkctrl (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[0]", "ADC clock control");
	field = print_field(val, 0, 0);
	printf("%sable 32kHz clock to ADC block\n", field == 0? "dis" : "en");
}

static void
ts__adclkctrl1 (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[8]", "ADCCLK_SEL: ADC clock select");
	field = print_field(val, 8, 8);
	printf("clock ADC and TS from %s clock\n", field == 0? "RTC" : "PERIPH_CLK");

	printf(ITEMFMT, "[7:0]", "ADC_FREQ: controls clock divider when bit [8]=1");
	field = print_field(val, 7, 0);
	printf("this is one less than divider\n");
}

static void
ts__adcstat (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[8]", "TS FIFO_OVERRUN");
	field = print_field(val, 8, 8);
	printf("FIFO %sover run\n", field == 0? "not " : "");

	printf(ITEMFMT, "[7]", "TS FIFO_EMPTY");
	field = print_field(val, 7, 7);
	printf("FIFO %sempty\n", field == 0? "not " : "");
}

static void
ts__adcselect (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[9:8]", "TS_Ref- selects A/D negative reference voltage");
	field = print_field(val, 9, 8);
	switch (field) {
		case 0:
			printf("TS_XM");
			break;
		case 1:
			printf("TS_YM");
			break;
		case 2:
			printf("Vss");
			break;
		default:
			printf("not used");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[7:6]", "TS_Ref+ selects A/D positive reference voltage");
	field = print_field(val, 7, 6);
	switch (field) {
		case 0:
			printf("TS_XP");
			break;
		case 1:
			printf("TS_YP");
			break;
		case 2:
			printf("VDDTS");
			break;
		default:
			printf("not used");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[5:4]", "TS_IN selects the input TS ADC");
	field = print_field(val, 5, 4);
	switch (field) {
		case 0:
			printf("TS_YM");
			break;
		case 1:
			printf("TS_XM");
			break;
		case 2:
			printf("TS_AUX");
			break;
		default:
			printf("not used");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[3]", "TS_YMC YM control");
	field = print_field(val, 3, 3);
	printf("the TS_YM signal to ground is %sconnected\n", field == 0? "dis" : "");

	printf(ITEMFMT, "[2]", "TS_YPC YP control");
	field = print_field(val, 2, 2);
	printf("the TS_YP signal to VddTS is %sconnected\n", field == 0? "" : "dis");

	printf(ITEMFMT, "[1]", "TS_XMC XM control");
	field = print_field(val, 1, 1);
	printf("the TS_XM sitnal to GND is %sconnected\n", field == 0? "dis" : "");

	printf(ITEMFMT, "[0]", "TS_XPC XP control");
	field = print_field(val, 0, 0);
	printf("the TS_XP signal to VddTS is %sconnected\n", field == 0? "dis" : "");
}

static void
ts__adcctrl (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[12:11]", "TS_FIFO_CTRL");
	field = print_field(val, 12, 11);
	printf("send TSC_IRQ on FIFO level ");
	switch (field) {
		case 0:
			printf("1");
			break;
		case 1:
			printf("4");
			break;
		case 2:
			printf("8");
			break;
		default:
			printf("16");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[10]", "TS_AUX_EN");
	field = print_field(val, 10, 10);
	printf("the AUX measured controller is %sabled\n", field == 0? "dis" : "");

	printf(ITEMFMT, "[9:7]", "TS_X_ACC");
	field = print_field(val, 9, 7);
	printf("ADC delivers %u bits\n", 10-field);

	printf(ITEMFMT, "[6:4]", "TS_Y_ACC");
	field = print_field(val, 6, 4);
	printf("ADC delivers %u bits\n", 10-field);

	printf(ITEMFMT, "[3]", "TS_POS_DET");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("normal auto mode");
	else
		printf("auto mode including position detect");
	printf("\n");

	printf(ITEMFMT, "[2]", "TS_ADC_PDC_CTRL");
	field = print_field(val, 2, 2);
	if (field == 0)
		printf("the ADC is in power down");
	else
		printf("the ADC is powered up and reset");
	printf("\n");

	printf(ITEMFMT, "[1]", "TS_ADC_STROBE");
	field = print_field(val, 1, 1);
	printf("writing 1 starts an AD conversion\n");

	printf(ITEMFMT, "[0]", "TS_AUTO_EN");
	field = print_field(val, 0, 0);
	printf("the touch screen controller is %sabled\n", field == 0? "dis" : "en");
}

static void
ts__tscsamplefifo (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31]", "TSC_P_LEVEL");
	field = print_field(val, 31, 31);
	printf("the touch screen is %spressed\n", field == 0? "" : "not ");

	printf(ITEMFMT, "[30]", "FIFO_EMPTY");
	field = print_field(val, 30, 30);
	printf("FIFO %sempty\n", field == 0? "not " : "");

	printf(ITEMFMT, "[29]", "FIFO_OVERRUN");
	field = print_field(val, 29, 29);
	printf("FIFO %sover run\n", field == 0? "not " : "");

	printf(ITEMFMT, "[25:16]", "TS_X_VALUE");
	print_field(val, 25, 16);
	printf("ADC value of the X coordinate\n");

	printf(ITEMFMT, "[9:0]", "TS_Y_VALUE");
	print_field(val, 9, 0);
	printf("ADC value of the Y coordinate\n");
}

static void
ts__tscdtr (uint32_t val)
{
	printf(ITEMFMT, "[19:0]", "TSC_DTR");
	print_field(val, 19, 0);
	printf("TS controller delay time register\n");
}

static void
ts__tscrtr (uint32_t val)
{
	printf(ITEMFMT, "[19:0]", "TSC_RTR");
	print_field(val, 19, 0);
	printf("touch screen controller rise time\n");
}

static void
ts__tscutr (uint32_t val)
{
	printf(ITEMFMT, "[19:0]", "TSC_UTR");
	print_field(val, 19, 0);
	printf("touch screen controller update time\n");
}

static void
ts__tscttr (uint32_t val)
{
	printf(ITEMFMT, "[19:0]", "TSC_TTR");
	print_field(val, 19, 0);
	printf("touch screen controller touch time\n");
}

static void
ts__tscdxp (uint32_t val)
{
	printf(ITEMFMT, "[19:0]", "TSC_DXP");
	print_field(val, 19, 0);
	printf("touch screen controller drain x plate time\n");
}

static void
ts__tscminx (uint32_t val)
{
	printf(ITEMFMT, "[9:0]", "TSC_MIN_X");
	print_field(val, 9, 0);
	printf("touch screen controller minimum X value\n");
}

static void
ts__tscmaxx (uint32_t val)
{
	printf(ITEMFMT, "[9:0]", "TSC_MAX_X");
	print_field(val, 9, 0);
	printf("touch screen controller maximum X value\n");
}

static void
ts__tscminy (uint32_t val)
{
	printf(ITEMFMT, "[9:0]", "TSC_MIN_Y");
	print_field(val, 9, 0);
	printf("touch screen controller minimum Y value\n");
}

static void
ts__tscmaxy (uint32_t val)
{
	printf(ITEMFMT, "[9:0]", "TSC_MAX_Y");
	print_field(val, 9, 0);
	printf("touch screen controller maximum Y value\n");
}

static void
ts__tscauxutr (uint32_t val)
{
	printf(ITEMFMT, "[31:0]", "TSC_AUX_UTR");
	print_field(val, 31, 0);
	printf("touch screen controller AUX update time\n");
}

static void
ts__tscauxmin (uint32_t val)
{
	printf(ITEMFMT, "[9:0]", "TSC_AUX_MIN");
	print_field(val, 9, 0);
	printf("touch screen controller AUX minimum value\n");
}

static void
ts__tscauxmax (uint32_t val)
{
	printf(ITEMFMT, "[9:0]", "TSC_AUX_MAX");
	print_field(val, 9, 0);
	printf("touch screen controller AUX maximum value\n");
}

static void
ts__tscauxvalue (uint32_t val)
{
	printf(ITEMFMT, "[9:0]", "TSC_AUX_VALUE");
	print_field(val, 9, 0);
	printf("touch screen controller AUX value\n");
}

static void
ts__tscadcvalue (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[9:0]", "ADC_VALUE");
	print_field(val, 9, 0);
	printf("ADC value of last conversion\n");

	printf(ITEMFMT, "[10]", "TSC_P_LEVEL");
	field = print_field(val, 10, 10);
	if (field == 0)
		printf("touch screen is pressed\n");
	else
		printf("touch is not pressed\n");
}

RegisterDescription_t ts[] = {
	{0x400040b4, 0,          "ADCLK_CTRL", "ADC clock control", accessRW, ts__adclkctrl},
	{0x40004060, 0,          "ADCLK_CTRL1", "second ADC clock control", accessRW, ts__adclkctrl1},
	{0x40048000, 0x00000080, "ADC_STAT", "A/D status", accessRead, ts__adcstat},
	{0x40048004, 0x00000004, "ADC_SELECT", "A/D/ select state", accessRW, ts__adcselect},
	{0x40048008, 0,          "ADC_CTRL", "A/D control", accessRW, ts__adcctrl},
	{0x4004800c, 0,          "TSC_SAMPLE_FIFO", "touchscreen sample FIFO", accessRead, ts__tscsamplefifo},
	{0x40048010, 0,          "TSC_DTR", "touchscreen delay time", accessRW, ts__tscdtr},
	{0x40048014, 0,          "TSC_RTR", "touchscreen rise time", accessRW, ts__tscrtr},
	{0x40048018, 0,          "TSC_UTR", "touchscreen update time", accessRW, ts__tscutr},
	{0x4004801c, 0,          "TSC_TTR", "touchscreen touch time", accessRW, ts__tscttr},
	{0x40048020, 0,          "TSC_DXP", "touchscreen drain X plate time", accessRW, ts__tscdxp},
	{0x40048024, 0,          "TSC_MIN_X", "touchscreen minimum X value", accessRW, ts__tscminx},
	{0x40048028, 0,          "TSC_MAX_X", "touchscreen maximum X value", accessRW, ts__tscmaxx},
	{0x4004802c, 0,          "TSC_MIN_Y", "touchscreen minimum Y value", accessRW, ts__tscminy},
	{0x40048030, 0,          "TSC_MAX_Y", "touchscreen maximum Y value", accessRW, ts__tscmaxy},
	{0x40048034, 0,          "TSC_AUX_UTR", "touchscreen AUX update time", accessRW, ts__tscauxutr},
	{0x40048038, 0,          "TSC_AUX_MIN", "touchscreen AUX minimum value", accessRW, ts__tscauxmin},
	{0x4004803c, 0,          "TSC_AUX_MAX", "touchscreen AUX maximum value", accessRW, ts__tscauxmax},
	{0x40048044, 0,          "TSC_AUX_VALUE", "touchscreen value", accessRead, ts__tscauxvalue},
	{0x40048048, 0,          "ADC_VALUE", "A/D data register", accessRead, ts__tscadcvalue},
};
size_t tsSZ = (sizeof(ts)/sizeof(ts[0]));
