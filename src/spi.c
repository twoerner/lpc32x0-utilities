// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include "registers.h"

static void
spi__global (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "rst");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("no action");
	else
		printf("SPI interface is reset");
	printf("\n");

	printf(ITEMFMT, "[0]", "enable");
	field = print_field(val, 0, 0);
	printf("SPIn interface is %sabled\n", field==0? "dis" : "en");
}

static void
spi__control (uint32_t val)
{
	int bit15;
	uint32_t field;

	printf(ITEMFMT, "[23]", "selects bidirectional or unidirectional usage of the SPIn_DATIO pin");
	field = print_field(val, 23, 23);
	printf("SPIn_DATIO pin is %sdirectional", field==0? "bi" : "uni");
	printf("\n");

	printf(ITEMFMT, "[22]", "busy halt, determines whether SPIn_BUSY affects SPI operation");
	field = print_field(val, 22, 22);
	if (field == 1)
		printf("data xfer is halted if SPIn_BUSY is active during master operation");
	else
		printf("the SPIn_BUSY pin is ignored during master operation");
	printf("\n");

	printf(ITEMFMT, "[21]", "busy polarity - controls the polarity of the SPIn_BUSY signal");
	field = print_field(val, 21, 21);
	printf("SPIn_BUSY is active %s\n", field==0? "LOW" : "HIGH");

	printf(ITEMFMT, "[20]", "(reserved)");
	print_field(val, 20, 20);
	printf("\n");

	printf(ITEMFMT, "[19]", "endian - controls the order in which the bits are transfered");
	field = print_field(val, 19, 19);
	printf("data is xfered %s-first\n", field==0? "MSB" : "LSB");

	printf(ITEMFMT, "[18]", "(reserved)");
	print_field(val, 18, 18);
	printf("\n");

	printf(ITEMFMT, "[17:16]", "SPI mode selection");
	field = print_field(val, 17, 16);
	switch (field) {
		case 0:
			printf("SPI mode 0: clock starts low, data is sampled at the clock rising edge");
			break;
		case 1:
			printf("SPI mode 1: clock starts low, data is sampled at the clock falling edge");
			break;
		case 2:
			printf("SPI mode 2: clock starts high, data is sampled at the clock falling edge");
			break;
		case 3:
			printf("SPI mode 3: clock starts high, data is sampled at the clock rising edge");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[15]", "rxtx - controls the direction of data transfer");
	field = print_field(val, 15, 15);
	if (field == 0)
		printf("data is shifted into the SPI (receive)");
	else
		printf("data is shifted out of the SPI (transmit)");
	printf("\n");

	bit15 = val & 0x00008000;
	printf(ITEMFMT, "[14]", "thr - controls the FIFO threshold, determines operation of interrupt flag");
	field = print_field(val, 14, 14);
	if (bit15) {
		if (field == 0)
			printf("the FIFO threshold is disabled");
		else
			printf("the FIFO threshold is enabled, threshold=8 entries in FIFO");
	}
	else {
		if (field == 0)
			printf("the FIFO threshold is disabled; threshold=1 entry in FIFO");
		else
			printf("the FIFO threshold is enabled; threshold=56 entries in FIFO");
	}
	printf("\n");

	printf(ITEMFMT, "[13]", "shift_off - controls generation of clock pulses on SPIn_CLK");
	field = print_field(val, 13, 13);
	printf("%sables the generation of clock pulses on SPIn_CLK\n", field==0? "en" : "dis");

	printf(ITEMFMT, "[12:9]", "bitnum - defines the number of bits to xmit or rcve in one block xfer");
	field = print_field(val, 12, 9);
	printf("%d bits\n", field+1);

	printf(ITEMFMT, "[8]", "(reserved)");
	print_field(val, 8, 8);
	printf("\n");

	printf(ITEMFMT, "[7]", "master");
	field = print_field(val, 7, 7);
	if (field == 0)
		printf("not supported");
	else
		printf("SPI is operating as master");
	printf("\n");

	printf(ITEMFMT, "[6:0]", "rate - SPI transfer rate - SPIn_CLK = HCLK / ((rate+1) x 2)");
	field = print_field(val, 6, 0);
	printf("value: %d", field);
}

static void
spi__framecount (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[15:0]", "spif - SPI frame count; the number of frames xfered");
	field = print_field(val, 15, 0);
	printf("%d frame(s)", field);
}

static void
spi__ier (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "inteot - end of transfer interrupt");
	field = print_field(val, 1, 1);
	printf("end of transfer interrupt is %sabled\n", field==0? "dis" : "en");

	printf(ITEMFMT, "[0]", "intthr - FIFO threshold interrupt enable");
	field = print_field(val, 0, 0);
	printf("the FIFO threshold interrupt is %sabled", field==0? "dis" : "en");
}

static void
spi__stat (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[8]", "intclr - SPI interrupt clear");
	print_field(val, 8, 8);
	printf("\n");

	printf(ITEMFMT, "[7]", "eot - end of transfer interrupt");
	print_field(val, 7, 7);
	printf("\n");

	printf(ITEMFMT, "[6]", "busylev - SPIn_BUSY level");
	print_field(val, 6, 6);
	printf("\n");

	printf(ITEMFMT, "[5:4]", "(reserved)");
	print_field(val, 5, 4);
	printf("\n");

	printf(ITEMFMT, "[3]", "shiftact - shift active; indicates when the SPI is transferring data");
	print_field(val, 3, 3);
	printf("\n");

	printf(ITEMFMT, "[2]", "bf - FIFO full interrupt flag");
	print_field(val, 2, 2);
	printf("\n");

	printf(ITEMFMT, "[1]", "thr - FIFO threshold interrupt flag");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("if rxtx==0: FIFO is below threshold; if rxtx==1: FIFO is above threshold");
	else
		printf("if rxtx==0: FIFO is at or above threshold; if rxtx==1: FIFO is at or below threshold");
	printf("\n");

	printf(ITEMFMT, "[0]", "be - FIFO empty interrupt flag");
	field = print_field(val, 0, 0);
	printf("FIFO is %sempty", field==0? "not " : "");
}

static void
spi__timctrl (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[2]", "tirqe - timed interrupt enable");
	field = print_field(val, 2, 2);
	printf("timed interrupt is %sabled\n", field==0? "dis" : "en");

	printf(ITEMFMT, "[1]", "pirqe - peripheral interrupt enable");
	field = print_field(val, 1, 1);
	printf("SPI status interrupt input %sabled\n", field==0? "dis" : "en");

	printf(ITEMFMT, "[0]", "mode - determines how the timer is used");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("timed interrupt mode");
	else
		printf("DMA time-out mode");
	printf("\n");
}

static void
spi__timcount (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[15:0]", "count");
	field = print_field(val, 15, 0);
	printf("%d", field);
}

static void
spi__timstat (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[15]", "tirqstat - timed interrupt status flag, write 1 to clear flag");
	field = print_field(val, 15, 15);
	printf("%stimed interrupt pending", field==0? "no " : "");
}

RegisterDescription_t spi[] = {
	{0x400040c4, 0,          "SPI_CTRL", "SPI1 and SPI2 clock and pin control", accessRW, NULL/*see clkpwr*/},
	{0x20088000, 0,          "SPI1_GLOBAL", "SPI1 global control", accessRW, spi__global},
	{0x20090000, 0,          "SPI2_GLOBAL", "SPI2 global control", accessRW, spi__global},
	{0x20088004, 0x00000e08, "SPI1_CON", "SPI1 control", accessRW, spi__control},
	{0x20090004, 0x00000e08, "SPI2_CON", "SPI2 control", accessRW, spi__control},
	{0x20088008, 0,          "SPI1_FRM", "SPI1 frame count", accessRW, spi__framecount},
	{0x20090008, 0,          "SPI2_FRM", "SPI2 frame count", accessRW, spi__framecount},
	{0x2008800c, 0,          "SPI1_IER", "SPI1 interrupt enable", accessRW, spi__ier},
	{0x2009000c, 0,          "SPI2_IER", "SPI2 interrupt enable", accessRW, spi__ier},
	{0x20088010, 0x00000001, "SPI1_STAT", "SPI1 status", accessRW, spi__stat},
	{0x20090010, 0x00000001, "SPI2_STAT", "SPI2 status", accessRW, spi__stat},
	{0x20088014, 0,          "SPI1_DAT", "SPI1 data", accessRW, NULL},
	{0x20090014, 0,          "SPI2_DAT", "SPI2 data", accessRW, NULL},
	{0x20088400, 0x00000002, "SPI1_TIM_CTRL", "SPI1 timer control", accessRW, spi__timctrl},
	{0x20090400, 0x00000002, "SPI2_TIM_CTRL", "SPI2 timer control", accessRW, spi__timctrl},
	{0x20088404, 0,          "SPI1_TIM_COUNT", "SPI1 timer count", accessRW, spi__timcount},
	{0x20090404, 0,          "SPI2_TIM_COUNT", "SPI2 timer count", accessRW, spi__timcount},
	{0x20088408, 0,          "SPI1_TIM_STAT", "SPI1 timer status", accessRW, spi__timstat},
	{0x20090408, 0,          "SPI2_TIM_STAT", "SPI2 timer status", accessRW, spi__timstat},
};
size_t spiSZ = sizeof(spi)/sizeof(spi[0]);
