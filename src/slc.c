// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registers.h"

static void
slc__slcdata (uint32_t val)
{
	print_field(val, 7, 0);
	printf("[7:0] NAND flash read or write data\n");
}

static void
slc__slcaddr (uint32_t val)
{
	print_field(val, 7, 0);
	printf("[7:0] NAND flash read or write address\n");
}

static void
slc__slccmd (uint32_t val)
{
	print_field(val, 7, 0);
	printf("[7:0] NAND flash command\n");
}

static void
slc__slcstop (uint32_t val)
{
	print_field(val, 7, 0);
	printf("[7:0] a write to this register causes the SLC flash controller to suspend all command/address sequences\n");
}

static void
slc__slcctrl (uint32_t val)
{
	print_field(val, 2, 2);
	printf("[2] writing 1 to this bit causes a reset of the SLC NAND flash controller\n");

	print_field(val, 1, 1);
	printf("[1] writing 1 to this bit clears ECC parity bits and reset the counter for ECC accumulation\n");

	print_field(val, 0, 0);
	printf("[0] writing 1 starts DMA data channel\n");
}

static void
slc__slccfg (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[5]", "CE_LOW");
	print_field(val, 5, 5);
	printf("writing 1 forces CEn always low, otherwise CEn is low only when SLC is accessed\n");

	printf(ITEMFMT, "[4]", "DMA_ECC");
	field = print_field(val, 4, 4);
	printf("channel %sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[3]", "ECC_EN");
	field = print_field(val, 3, 3);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[2]", "DMA_BURST");
	field = print_field(val, 2, 2);
	if (field == 1)
		printf("burst enabled, data channel use DMA_BREQ signal");
	else
		printf("burst disabled, use dmasreq0 signal only");
	printf("\n");

	printf(ITEMFMT, "[1]", "DMA_DIR");
	field = print_field(val, 1, 1);
	printf("DMA %s SLC\n", field == 1? "read from" : "write to");

	printf(ITEMFMT, "[0]", "external device width select");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("8-bit device");
	else
		printf("not used");
	printf("\n");
}

static void
slc__slcstat (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[2]", "DMA_ACTIVE: DMA_FIFO status");
	field = print_field(val, 2, 2);
	if (field == 1)
		printf("the DMA_FIFO contains data");
	else
		printf("no data in the DMA_FIFO");
	printf("\n");

	printf(ITEMFMT, "[1]", "SLC_ACTIVE: SLC_FIFO status");
	field = print_field(val, 1, 1);
	if (field == 1)
		printf("the SLC_FIFO contains data");
	else
		printf("no data in the SLC_FIFO");
	printf("\n");

	printf(ITEMFMT, "[0]", "READY: NAND flash device ready signal");
	field = print_field(val, 0, 0);
	printf("device %s\n", field == 1? "ready" : "busy");
}

static void
slc__slcintstat (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "INT_TC_STAT: terminal count interrupt status");
	field = print_field(val, 1, 1);
	printf("interrupt is%s pending (after mask)\n", field == 1? "" : " not");

	printf(ITEMFMT, "[0]", "INT_RDY_STAT: device ready interrupt status");
	field = print_field(val, 0, 0);
	printf("interrupt is%s pending (after mask)\n", field == 1? "" : " not");
}

static void
slc__slcien (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "INT_TC_EN");
	field = print_field(val, 1, 1);
	if (field == 1)
		printf("enable interrupt when TC has reached 0");
	else
		printf("disable TC interrupt");
	printf("\n");

	printf(ITEMFMT, "[0]", "INT_RDY_EN");
	field = print_field(val, 0, 0);
	if (field == 1)
		printf("enable interrupt when RDY asserted");
	else
		printf("disable RDY interrupt");
	printf("\n");
}

static void
slc__slcisr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "INT_TC_SET");
	field = print_field(val, 1, 1);
	if (field == 1)
		printf("writing 1 sets the TC interrupt");
	else
		printf("writing 0 has no effect");
	printf("\n");

	printf(ITEMFMT, "[0]", "INT_RDY_SET");
	field = print_field(val, 0, 0);
	if (field == 1)
		printf("writing 1 sets the RDY interrupt");
	else
		printf("writing 0 has no effect");
	printf("\n");
}

static void
slc__slcicr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "INT_TC_CLR");
	field = print_field(val, 1, 1);
	if (field == 1)
		printf("writing 1 clears the TC interrupt");
	else
		printf("writing 0 has no effect");
	printf("\n");

	printf(ITEMFMT, "[0]", "INT_RDY_CLR");
	field = print_field(val, 0, 0);
	if (field == 1)
		printf("writing 1 clears the RDY interrupt");
	else
		printf("writing 0 has no effect");
	printf("\n");
}

static void
slc__slctac (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:28]", "W_RDY");
	field = print_field(val, 31, 28);
	printf("%u\n", field);

	printf(ITEMFMT, "[27:24]", "W_WIDTH");
	field = print_field(val, 27, 24);
	printf("%u\n", field);

	printf(ITEMFMT, "[23:20]", "W_HOLD");
	field = print_field(val, 23, 20);
	printf("%u\n", field);

	printf(ITEMFMT, "[19:16]", "W_SETUP");
	field = print_field(val, 19, 16);
	printf("%u\n", field);

	printf(ITEMFMT, "[15:12]", "R_RDY");
	field = print_field(val, 15, 12);
	printf("%u\n", field);

	printf(ITEMFMT, "[11:8]", "R_WIDTH");
	field = print_field(val, 11, 8);
	printf("%u\n", field);

	printf(ITEMFMT, "[7:4]", "R_HOLD");
	field = print_field(val, 7, 4);
	printf("%u\n", field);

	printf(ITEMFMT, "[3:0]", "R_SETUP");
	field = print_field(val, 3, 0);
	printf("%u\n", field);
}

static void
slc__slctc (uint32_t val)
{
	print_field(val, 15, 0);
	printf("number of remaining bytes to be xfered to/from NAND during DMA\n");
}

static void
slc__slcecc (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[21:6]", "line parity");
	field = print_field(val, 21, 6);
	printf("%u\n", field);

	printf(ITEMFMT, "[5:0]", "column parity");
	field = print_field(val, 5, 0);
	printf("%u\n", field);
}

static void
slc__slcdmadata (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:24]", "last byte (upper address)");
	field = print_field(val, 31, 24);
	printf("0x%x\n", field);

	printf(ITEMFMT, "[23:16]", "third byte");
	field = print_field(val, 23, 16);
	printf("0x%x\n", field);

	printf(ITEMFMT, "[15:8]", "second byte");
	field = print_field(val, 15, 8);
	printf("0x%x\n", field);

	printf(ITEMFMT, "[7:0]", "first byte (lower address)");
	field = print_field(val, 7, 0);
	printf("0x%x\n", field);
}

RegisterDescription_t slc[] = {
	{0x20020000, 0, "SLC_DATA", "SLC NAND flash data register", accessRW, slc__slcdata},
	{0x20020004, 0, "SLC_ADDR", "SLC NAND flash address register", accessWrite, slc__slcaddr},
	{0x20020008, 0, "SLC_CMD", "SLC NAND flash command register", accessWrite, slc__slccmd},
	{0x2002000c, 0, "SLC_STOP", "SLC NAND flash stop register", accessWrite, slc__slcstop},
	{0x20020010, 0, "SLC_CTRL", "SLC NAND flash control register", accessRW, slc__slcctrl},
	{0x20020014, 0, "SLC_CFG", "SLC NAND flash configuration register", accessRW, slc__slccfg},
	{0x20020018, 0, "SLC_STAT", "SLC NAND flash status register", accessRead, slc__slcstat},
	{0x2002001c, 0, "SLC_INT_STAT", "SLC NAND flash interrupt status register", accessRead, slc__slcintstat},
	{0x20020020, 0, "SLC_IEN", "SLC NAND flash interrupt enable register", accessRW, slc__slcien},
	{0x20020024, 0, "SLC_ISR", "SLC NAND flash interrupt set register", accessWrite, slc__slcisr},
	{0x20020028, 0, "SLC_ICR", "SLC NAND flash interrupt clear register", accessWrite, slc__slcicr},
	{0x2002002c, 0, "SLC_TAC", "SLC NAND flash read timing arcs configuration register", accessRW, slc__slctac},
	{0x20020030, 0, "SLC_TC", "SLC NAND flash transfer count register", accessRW, slc__slctc},
	{0x20020034, 0, "SLC_ECC", "SLC NAND flash parity bits", accessRead, slc__slcecc},
	{0x20020038, 0, "SLC_DMA_DATA", "SLC NAND flash DMA data", accessWrite, slc__slcdmadata},
};
size_t slcSZ = (sizeof(slc)/sizeof(slc[0]));
