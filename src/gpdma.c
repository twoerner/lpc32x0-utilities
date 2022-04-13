// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registers.h"

static void
per_channel (uint32_t val)
{
	print_field(val, 7, 0);
	printf("[7:0] each bit represents one channel\n");
}

static void
gpdma__dmacenbldchns (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[7:0]", "enabled channels");
	field = print_field(val, 7, 7);
	printf("channel 7 %s\n", (field == 1? "enabled" : "disabled"));
	field = print_field(val, 6, 6);
	printf("channel 6 %s\n", (field == 1? "enabled" : "disabled"));
	field = print_field(val, 5, 5);
	printf("channel 5 %s\n", (field == 1? "enabled" : "disabled"));
	field = print_field(val, 4, 4);
	printf("channel 4 %s\n", (field == 1? "enabled" : "disabled"));
	field = print_field(val, 3, 3);
	printf("channel 3 %s\n", (field == 1? "enabled" : "disabled"));
	field = print_field(val, 2, 2);
	printf("channel 2 %s\n", (field == 1? "enabled" : "disabled"));
	field = print_field(val, 1, 1);
	printf("channel 1 %s\n", (field == 1? "enabled" : "disabled"));
	field = print_field(val, 0, 0);
	printf("channel 0 %s\n", (field == 1? "enabled" : "disabled"));
}

static void
dma_slaves (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[15:0]", "DMA source");
	field = print_field(val, 15, 15);
	if (field == 1)
		printf("SSP0 transmit\n");
	else
		printf("(reserved)\n");
	field = print_field(val, 14, 14);
	if (field == 1)
		printf("SSP0 receive\n");
	else
		printf("(reserved)\n");
	print_field(val, 13, 13); printf("I2S0 DMA1\n");
	print_field(val, 12, 12); printf("NAND flash\n");
	field = print_field(val, 11, 11);
	printf("SSP1 %s\n", (field == 1? "transmit" : "receive"));
	field = print_field(val, 10, 10);
	if (field == 1)
		printf("I2S1 DMA1\n");
	else
		printf("14-clock UART7 recieve\n");
	print_field(val, 9, 9); printf("14-clock UART7 transmit\n");
	print_field(val, 8, 8); printf("14-clock UART2 receive\n");
	print_field(val, 7, 7); printf("14-clock UART2 transmit\n");
	print_field(val, 6, 6); printf("14-clock UART1 receive\n");
	print_field(val, 5, 5); printf("14-clock UART1 transmit\n");
	print_field(val, 4, 4); printf("SD receive and transmit\n");
	field = print_field(val, 3, 3);
	if (field == 1)
		printf("SSP1 receive\n");
	else
		printf("SPI2 receive and transmit\n");
	print_field(val, 2, 2); printf("I2S1 DMA0\n");
	print_field(val, 1, 1); printf("NAND flash\n");
	print_field(val, 0, 0); printf("I2S0 DMA0\n");
}

static void
dma_source_address (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:0]", "DMA source address");
	field = print_field(val, 31, 0);
	printf("0x%08x\n", field);
}

static void
dma_dest_address (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:0]", "DMA destination address");
	field = print_field(val, 31, 0);
	printf("0x%08x\n", field);
}

static void
dma_linked_list (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:2]", "address of next linked-list item");
	field = print_field(val,31, 2);
	printf("0x%08x\n", field);

	printf(ITEMFMT, "[0]", "AHB master");
	field = print_field(val, 0, 0);
	printf("%u\n", field);
}

static void
dma_channel_control (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31]", "terminal count interrupt enable bit");
	field = print_field(val, 31, 31);
	printf("%s\n", field == 1? "enabled" : "disabled");

	printf(ITEMFMT, "[27]", "destination increment");
	field = print_field(val, 27, 27);
	printf("is%s incremented after each xfer\n", field == 1? "" : " not");

	printf(ITEMFMT, "[26]", "source increment");
	field = print_field(val, 26, 26);
	printf("is%s incremented after each xfer\n", field == 1? "" : " not");

	printf(ITEMFMT, "[25]", "AHB master selected for destination xfer");
	field = print_field(val, 25, 25);
	printf("%u\n", field);

	printf(ITEMFMT, "[24]", "AHB master selected for source xfer");
	field = print_field(val, 24, 24);
	printf("%u\n", field);

	printf(ITEMFMT, "[23:21]", "destination xfer width");
	field = print_field(val, 23, 21);
	switch (field) {
		case 0:
			printf("byte (8-bit)");
			break;
		case 1:
			printf("halfword (16-bit)");
			break;
		case 2:
			printf("word (32-bit)");
			break;
		default:
			printf("(reserved)");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[20:18]", "source xfer width");
	field = print_field(val, 20, 18);
	switch (field) {
		case 0:
			printf("byte (8-bit)");
			break;
		case 1:
			printf("halfword (16-bit)");
			break;
		case 2:
			printf("word (32-bit)");
			break;
		default:
			printf("(reserved)");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[17:15]", "destination burst size");
	field = print_field(val, 17, 15);
	if (field == 0)
		printf("1");
	else
		printf("%u", (1 << (field+1)));
	printf("\n");

	printf(ITEMFMT, "[14:12]", "source burst size");
	field = print_field(val, 14, 12);
	if (field == 0)
		printf("1");
	else
		printf("%u", (1 << (field+1)));
	printf("\n");

	printf(ITEMFMT, "[11:0]", "write=xfer-size read=xfers-completed");
	field = print_field(val, 11, 0);
	printf("%u\n", field);
}

static void
dma_channel_config (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[18]", "halt");
	field = print_field(val, 18, 18);
	if (field == 1)
		printf("ignore further source DMA requests");
	else
		printf("enable DMA requests");
	printf("\n");

	printf(ITEMFMT, "[17]", "active");
	field = print_field(val, 17, 17);
	printf("the FIFO of the channel %sdata\n",
			(field == 1? "has " : "doesn't have "));

	printf(ITEMFMT, "[16]", "lock");
	field = print_field(val, 16, 16);
	printf("%s\n", (field == 1? "enabled" : "disabled"));

	printf(ITEMFMT, "[15]", "terminal count interrupt mask");
	field = print_field(val, 15, 15);
	printf("%u\n", field);

	printf(ITEMFMT, "[14]", "interrupt error mask");
	field = print_field(val, 14, 14);
	printf("%u\n", field);

	printf(ITEMFMT, "[13:11]", "flow control and *transfer type*");
	field = print_field(val, 13, 11);
	switch (field) {
		case 0:
			printf("*DMA* memory to memory");
			break;
		case 1:
			printf("*DMA* memory to peripheral");
			break;
		case 2:
			printf("*DMA* peripheral to memory");
			break;
		case 3:
			printf("*DMA* source peripheral to destination peripheral");
			break;
		case 4:
			printf("source peripheral to *destination peripheral*");
			break;
		case 5:
			printf("memory to *peripheral*");
			break;
		case 6:
			printf("*peripheral* to memory");
			break;
		case 7:
			printf("*source peripheral* to destination peripheral");
			break;
		default:
			printf("?");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[10:6]", "destination peripheral");
	field = print_field(val, 10, 6);
	printf("\n");

	printf(ITEMFMT, "[5:1]", "source peripheral");
	field = print_field(val, 5, 1);
	printf("\n");

	printf(ITEMFMT, "[0]", "channel enable");
	field = print_field(val, 0, 0);
	printf("%s\n", field == 1? "enabled" : "disabled");
}

static void gpdma__dmacc0srcaddr (uint32_t val) { dma_source_address(val); }
static void gpdma__dmacc1srcaddr (uint32_t val) { dma_source_address(val); }
static void gpdma__dmacc2srcaddr (uint32_t val) { dma_source_address(val); }
static void gpdma__dmacc3srcaddr (uint32_t val) { dma_source_address(val); }
static void gpdma__dmacc4srcaddr (uint32_t val) { dma_source_address(val); }
static void gpdma__dmacc5srcaddr (uint32_t val) { dma_source_address(val); }
static void gpdma__dmacc6srcaddr (uint32_t val) { dma_source_address(val); }
static void gpdma__dmacc7srcaddr (uint32_t val) { dma_source_address(val); }

static void gpdma__dmacc0destaddr (uint32_t val) { dma_dest_address(val); }
static void gpdma__dmacc1destaddr (uint32_t val) { dma_dest_address(val); }
static void gpdma__dmacc2destaddr (uint32_t val) { dma_dest_address(val); }
static void gpdma__dmacc3destaddr (uint32_t val) { dma_dest_address(val); }
static void gpdma__dmacc4destaddr (uint32_t val) { dma_dest_address(val); }
static void gpdma__dmacc5destaddr (uint32_t val) { dma_dest_address(val); }
static void gpdma__dmacc6destaddr (uint32_t val) { dma_dest_address(val); }
static void gpdma__dmacc7destaddr (uint32_t val) { dma_dest_address(val); }

static void gpdma__dmacc0lli (uint32_t val) { dma_linked_list(val); }
static void gpdma__dmacc1lli (uint32_t val) { dma_linked_list(val); }
static void gpdma__dmacc2lli (uint32_t val) { dma_linked_list(val); }
static void gpdma__dmacc3lli (uint32_t val) { dma_linked_list(val); }
static void gpdma__dmacc4lli (uint32_t val) { dma_linked_list(val); }
static void gpdma__dmacc5lli (uint32_t val) { dma_linked_list(val); }
static void gpdma__dmacc6lli (uint32_t val) { dma_linked_list(val); }
static void gpdma__dmacc7lli (uint32_t val) { dma_linked_list(val); }

static void gpdma__dmacc0control (uint32_t val) { dma_channel_control(val); }
static void gpdma__dmacc1control (uint32_t val) { dma_channel_control(val); }
static void gpdma__dmacc2control (uint32_t val) { dma_channel_control(val); }
static void gpdma__dmacc3control (uint32_t val) { dma_channel_control(val); }
static void gpdma__dmacc4control (uint32_t val) { dma_channel_control(val); }
static void gpdma__dmacc5control (uint32_t val) { dma_channel_control(val); }
static void gpdma__dmacc6control (uint32_t val) { dma_channel_control(val); }
static void gpdma__dmacc7control (uint32_t val) { dma_channel_control(val); }

static void gpdma__dmacc0config (uint32_t val) { dma_channel_config(val); }
static void gpdma__dmacc1config (uint32_t val) { dma_channel_config(val); }
static void gpdma__dmacc2config (uint32_t val) { dma_channel_config(val); }
static void gpdma__dmacc3config (uint32_t val) { dma_channel_config(val); }
static void gpdma__dmacc4config (uint32_t val) { dma_channel_config(val); }
static void gpdma__dmacc5config (uint32_t val) { dma_channel_config(val); }
static void gpdma__dmacc6config (uint32_t val) { dma_channel_config(val); }
static void gpdma__dmacc7config (uint32_t val) { dma_channel_config(val); }

RegisterDescription_t gpdma[] = {
	{0x31000000, 0, "DMACIntStat", "DMA interrupt status register", accessRW, per_channel},
	{0x31000004, 0, "DMACIntTCStat", "DMA interrupt terminal count request status register", accessRead, per_channel},
	{0x31000008, 0, "DMACIntTCClear", "DMA interrupt terminal count request clear register", accessWrite, per_channel},
	{0x3100000c, 0, "DMACIntErrStat", "DMA interrupt error status register", accessRead, per_channel},
	{0x31000010, 0, "DMACIntErrClr", "DMA interrupt error clear", accessWrite, per_channel},
	{0x31000014, 0, "DMACRawIntTCStat", "DMA raw interrupt terminal count status", accessRead, per_channel},
	{0x31000018, 0, "DMACRawIntErrStat", "DMA raw error interrupt status", accessRead, per_channel},
	{0x3100001c, 0, "DMACEnbldChns", "DMA enabled channel", accessRead, gpdma__dmacenbldchns},
	{0x31000020, 0, "DMACSoftBReq", "DMA software burst request", accessRW, dma_slaves},
	{0x31000024, 0, "DMACSoftSReq", "DMA software single request", accessRW, dma_slaves},
	{0x31000028, 0, "DMACSoftLBReq", "DMA software last burst request", accessRW, dma_slaves},
	{0x3100002c, 0, "DMACSoftLSReq", "DMA software last single request", accessRW, dma_slaves},
	{0x31000030, 0, "DMACConfig", "DMA configuration", accessRW, dma_slaves},
	/* channel 0 */
	{0x31000100, 0, "DMACC0SrcAddr", "DMA channel 0 source address", accessRW, gpdma__dmacc0srcaddr},
	{0x31000104, 0, "DMACC0DestAddr", "DMA channel 0 destination address", accessRW, gpdma__dmacc0destaddr},
	{0x31000108, 0, "DMACC0LLI", "DMA channel 0 linked list item", accessRW, gpdma__dmacc0lli},
	{0x3100010c, 0, "DMACC0Control", "DMA channel 0 control", accessRW, gpdma__dmacc0control},
	{0x31000110, 0, "DMACC0Config", "DMA channel 0 configuration", accessRW, gpdma__dmacc0config},
	/* channel 1 */
	{0x31000120, 0, "DMACC1SrcAddr", "DMA channel 1 source address", accessRW, gpdma__dmacc1srcaddr},
	{0x31000124, 0, "DMACC1DestAddr", "DMA channel 1 destination address", accessRW, gpdma__dmacc1destaddr},
	{0x31000128, 0, "DMACC1LLI", "DMA channel 1 linked list item", accessRW, gpdma__dmacc1lli},
	{0x3100012c, 0, "DMACC1Control", "DMA channel 1 control", accessRW, gpdma__dmacc1control},
	{0x31000130, 0, "DMACC1Config", "DMA channel 1 configuration", accessRW, gpdma__dmacc1config},
	/* channel 2 */
	{0x31000140, 0, "DMACC2SrcAddr", "DMA channel 2 source address", accessRW, gpdma__dmacc2srcaddr},
	{0x31000144, 0, "DMACC2DestAddr", "DMA channel 2 destination address", accessRW, gpdma__dmacc2destaddr},
	{0x31000148, 0, "DMACC2LLI", "DMA channel 2 linked list item", accessRW, gpdma__dmacc2lli},
	{0x3100014c, 0, "DMACC2Control", "DMA channel 2 control", accessRW, gpdma__dmacc2control},
	{0x31000150, 0, "DMACC2Config", "DMA channel 2 configuration", accessRW, gpdma__dmacc2config},
	/* channel 3 */
	{0x31000160, 0, "DMACC3SrcAddr", "DMA channel 3 source address", accessRW, gpdma__dmacc3srcaddr},
	{0x31000164, 0, "DMACC3DestAddr", "DMA channel 3 destination address", accessRW, gpdma__dmacc3destaddr},
	{0x31000168, 0, "DMACC3LLI", "DMA channel 3 linked list item", accessRW, gpdma__dmacc3lli},
	{0x3100016c, 0, "DMACC3Control", "DMA channel 3 control", accessRW, gpdma__dmacc3control},
	{0x31000170, 0, "DMACC3Config", "DMA channel 3 configuration", accessRW, gpdma__dmacc3config},
	/* channel 4 */
	{0x31000180, 0, "DMACC4SrcAddr", "DMA channel 4 source address", accessRW, gpdma__dmacc4srcaddr},
	{0x31000184, 0, "DMACC4DestAddr", "DMA channel 4 destination address", accessRW, gpdma__dmacc4destaddr},
	{0x31000188, 0, "DMACC4LLI", "DMA channel 4 linked list item", accessRW, gpdma__dmacc4lli},
	{0x3100018c, 0, "DMACC4Control", "DMA channel 4 control", accessRW, gpdma__dmacc4control},
	{0x31000190, 0, "DMACC4Config", "DMA channel 4 configuration", accessRW, gpdma__dmacc4config},
	/* channel 5 */
	{0x310001a0, 0, "DMACC5SrcAddr", "DMA channel 5 source address", accessRW, gpdma__dmacc5srcaddr},
	{0x310001a4, 0, "DMACC5DestAddr", "DMA channel 5 destination address", accessRW, gpdma__dmacc5destaddr},
	{0x310001a8, 0, "DMACC5LLI", "DMA channel 5 linked list item", accessRW, gpdma__dmacc5lli},
	{0x310001ac, 0, "DMACC5Control", "DMA channel 5 control", accessRW, gpdma__dmacc5control},
	{0x310001b0, 0, "DMACC5Config", "DMA channel 5 configuration", accessRW, gpdma__dmacc5config},
	/* channel 6 */
	{0x310001c0, 0, "DMACC6SrcAddr", "DMA channel 6 source address", accessRW, gpdma__dmacc6srcaddr},
	{0x310001c4, 0, "DMACC6DestAddr", "DMA channel 6 destination address", accessRW, gpdma__dmacc6destaddr},
	{0x310001c8, 0, "DMACC6LLI", "DMA channel 6 linked list item", accessRW, gpdma__dmacc6lli},
	{0x310001cc, 0, "DMACC6Control", "DMA channel 6 control", accessRW, gpdma__dmacc6control},
	{0x310001d0, 0, "DMACC6Config", "DMA channel 6 configuration", accessRW, gpdma__dmacc6config},
	/* channel 7 */
	{0x310001e0, 0, "DMACC7SrcAddr", "DMA channel 7 source address", accessRW, gpdma__dmacc7srcaddr},
	{0x310001e4, 0, "DMACC7DestAddr", "DMA channel 7 destination address", accessRW, gpdma__dmacc7destaddr},
	{0x310001e8, 0, "DMACC7LLI", "DMA channel 7 linked list item", accessRW, gpdma__dmacc7lli},
	{0x310001ec, 0, "DMACC7Control", "DMA channel 7 control", accessRW, gpdma__dmacc7control},
	{0x310001f0, 0, "DMACC7Config", "DMA channel 7 configuration", accessRW, gpdma__dmacc7config},
};
size_t gpdmaSZ = (sizeof(gpdma)/sizeof(gpdma[0]));
