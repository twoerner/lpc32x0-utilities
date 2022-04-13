// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include "registers.h"

static void
ssp__cr0 (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[15:8]", "SCR - serial clock rate = HCLK / (CPSDVSR x (SCR+1))");
	field = print_field(val, 15, 8);
	printf("%d\n", field);

	printf(ITEMFMT, "[7]", "CPHA - clock out phase - this bit is only used in SPI mode");
	field = print_field(val, 7, 7);
	if (field == 0)
		printf("SSP captures serial data on the first clock transition of the frame");
	else
		printf("SSP captures serial data on the second clock transition of the frame");
	printf("\n");

	printf(ITEMFMT, "[6]", "CPOL - clock out polarity - this bit is only used in SPI mode");
	field = print_field(val, 6, 6);
	if (field == 0)
		printf("SSP controller maintains the bus clock low between frames");
	else
		printf("SSP controller maintains the bus clock high between frames");
	printf("\n");

	printf(ITEMFMT, "[5:4]", "FRF - frame format");
	field = print_field(val, 5, 4);
	switch (field) {
		case 0:
			printf("SPI");
			break;
		case 1:
			printf("TI");
			break;
		case 2:
			printf("Microwire");
			break;
		default:
			printf("- not supported -");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[3:0]", "DSS - data size select - number of bits transferred in each frame");
	field = print_field(val, 3, 0);
	if (field < 3)
		printf("- not supported -");
	else
		printf("%d bit transfer", field+1);
	printf("\n");
}

static void
ssp__cr1 (uint32_t val)
{
	int bit2;
	uint32_t field;

	bit2 = val & 0x4;
	printf(ITEMFMT, "[3]", "SOD - slave output disable");
	field = print_field(val, 3, 3);
	if (bit2) {
		if (field == 1)
			printf("block the SSP controller from driving MISO");
		else
			printf("allow SSP to drive MISO");
	}
	else
		printf("only relevant when bit 2 (MS) == 1");
	printf("\n");

	printf(ITEMFMT, "[2]", "MS - master/slave mode (can only be written when SSE == 0)");
	field = print_field(val, 2, 2);
	if (field == 0)
		printf("SSP is master; drives SCLK, MOSI, and SSEL, receives on MISO");
	else
		printf("SSP is slave; drives MISO, and receives on SCLK, MOSI, and SSEL");
	printf("\n");

	printf(ITEMFMT, "[1]", "SSE - SSP enable");
	field = print_field(val, 1, 1);
	printf("SSP is %sabled\n", field==0? "dis" : "en");

	printf(ITEMFMT, "[0]", "LBM - loop back mode");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("normal operation");
	else
		printf("loop-back mode");
	printf("\n");
}

static void
ssp__status (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[4]", "BSY");
	field = print_field(val, 4, 4);
	printf("%sbusy\n", field==0? "not " : "");

	printf(ITEMFMT, "[3]", "RFF");
	field = print_field(val, 3, 3);
	printf("receive FIFO %sfull\n", field==0? "not " : "");

	printf(ITEMFMT, "[2]", "RNE");
	field = print_field(val, 2, 2);
	printf("receive FIFO %sempty\n", field==1? "not " : "");

	printf(ITEMFMT, "[1]", "TNF");
	field = print_field(val, 1, 1);
	printf("transmit FIFO %sfull\n", field==1? "not " : "");

	printf(ITEMFMT, "[0]", "TFE");
	field = print_field(val, 0, 0);
	printf("transmit FIFO %sempty", field==0? "not " : "");
}

static void
ssp__cpsr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[7:0]", "clock prescale");
	field = print_field(val, 7, 0);
	printf("%d", field);
}

static void
ssp__imsc (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3]", "TXIM - interrupt when the Tx FIFO is at least half empty");
	field = print_field(val, 3, 3);
	printf("%sabled\n", field==1? "en" : "dis");

	printf(ITEMFMT, "[2]", "RXIM - interrupt when the Rx FIFO is at least half full");
	field = print_field(val, 2, 2);
	printf("%sabled\n", field==1? "en" : "dis");

	printf(ITEMFMT, "[1]", "RTIM - receive timeout interrupt");
	field = print_field(val, 1, 1);
	printf("%sabled\n", field==1? "en" : "dis");

	printf(ITEMFMT, "[0]", "RORIM - receive overrun interrupt");
	field = print_field(val, 0, 0);
	printf("%sabled\n", field==1? "en" : "dis");
}

static void
ssp__ris (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3]", "TXRIS");
	field = print_field(val, 3, 3);
	printf("Tx FIFO is %sat least half empty\n", field==1? "" : "not ");

	printf(ITEMFMT, "[2]", "RXRIS");
	field = print_field(val, 2, 2);
	printf("Rx FIFO is %sat least half full\n", field==1? "" : "not ");

	printf(ITEMFMT, "[1]", "RTRIS");
	field = print_field(val, 1, 1);
	printf("Rx FIFO is %sempty and has not been read for a timeout period\n", field==1? "not " : "");

	printf(ITEMFMT, "[0]", "RORRIS");
	field = print_field(val, 0, 0);
	printf("another frame was %scompletely received while the Rx FIFO was full", field==1? "" : "not ");
}

static void
ssp__mis (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3]", "TXMIS");
	field = print_field(val, 3, 3);
	printf("the Tx FIFO is %sat least half empty\n", field==0? "not " : "");

	printf(ITEMFMT, "[2]", "RXMIS");
	field = print_field(val, 2, 2);
	printf("the Rx FIFO is %sat least half full\n", field==0? "not " : "");

	printf(ITEMFMT, "[1]", "RTMIS");
	field = print_field(val, 1, 1);
	printf("the Rx FIFO is %sempty\n", field==1? "not " : "");

	printf(ITEMFMT, "[0]", "RORMIS");
	field = print_field(val, 0, 0);
	printf("another frame was %scompletely received while the Rx FIFO was full", field==1? "" : "not ");
}

static void
ssp__dmacr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "TXDMAE");
	field = print_field(val, 1, 1);
	printf("DMA for the transmit FIFO is %sabled\n", field==1? "en" : "dis");

	printf(ITEMFMT, "[0]", "RXDMAE");
	field = print_field(val, 0, 0);
	printf("DMA for the receive FIFO is %sabled", field==1? "en" : "dis");
}

RegisterDescription_t ssp[] = {
	{0x40004078, 0, "SSP_CTRL", "SSP0/1 clock control", accessRW, NULL/*see clkpwr.c*/},
	{0x20084000, 0, "SSP0CR0", "SSP0 control 0", accessRW, ssp__cr0},
	{0x2008c000, 0, "SSP1CR0", "SSP1 control 0", accessRW, ssp__cr0},
	{0x20084004, 0, "SSP0CR1", "SSP0 control 1", accessRW, ssp__cr1},
	{0x2008c004, 0, "SSP1CR1", "SSP1 control 1", accessRW, ssp__cr1},
	{0x20084008, 0, "SSP0DR", "SSP0 data", accessWrite, NULL},
	{0x2008c008, 0, "SSP1DR", "SSP1 data", accessWrite, NULL},
	{0x2008400c, 0, "SSP0SR", "SSP0 status", accessRead, ssp__status},
	{0x2008c00c, 0, "SSP1SR", "SSP1 status", accessRead, ssp__status},
	{0x20084010, 0, "SSP0CPSR", "SSP0 clock prescale", accessRW, ssp__cpsr},
	{0x2008c010, 0, "SSP1CPSR", "SSP1 clock prescale", accessRW, ssp__cpsr},
	{0x20084014, 0, "SSP0IMSC", "SSP0 interrupt mask set and clear", accessRW, ssp__imsc},
	{0x2008c014, 0, "SSP1IMSC", "SSP1 interrupt mask set and clear", accessRW, ssp__imsc},
	{0x20084018, 0, "SSP0RIS", "SSP0 raw interrupt status", accessRead, ssp__ris},
	{0x2008c018, 0, "SSP1RIS", "SSP1 raw interrupt status", accessRead, ssp__ris},
	{0x2008401c, 0, "SSP0MIS", "SSP0 masked interrupt status", accessRead, ssp__mis},
	{0x2008c01c, 0, "SSP1MIS", "SSP1 masked interrupt status", accessRead, ssp__mis},
	{0x20084020, 0, "SSP0ICR", "SSP0 interrupt clear", accessWrite, NULL},
	{0x2008c020, 0, "SSP1ICR", "SSP1 interrupt clear", accessWrite, NULL},
	{0x20084024, 0, "SSP0DMACR", "SSP0 DMA control", accessRW, ssp__dmacr},
	{0x2008c024, 0, "SSP1DMACR", "SSP1 DMA control", accessRW, ssp__dmacr},
};
size_t sspSZ = sizeof(ssp)/sizeof(ssp[0]);
