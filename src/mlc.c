// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registers.h"

static void
mlc__mlccmd (uint32_t unused val)
{
	printf(ITEMFMT, "[7:0]", "command code");
}

static void
mlc__mlcaddr (uint32_t unused val)
{
	printf(ITEMFMT, "[7:0]", "address");
}

static void
mlc__mlceccencreg (uint32_t unused val)
{
	printf(ITEMFMT, "[7:0]", "writing to this register starts a data encode cycle");
}

static void
mlc__mlceccdecreg (uint32_t unused val)
{
	printf(ITEMFMT, "[7:0]", "writing to this register starts a data decode cycle");
}

static void
mlc__mlceccautoencreg (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[8]", "auto-program");
	field = print_field(val, 8, 8);
	printf("auto-program command %sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[7:0]", "auto-program command");
}

static void
mlc__mlceccautodecreg (uint32_t unused val)
{
	printf(ITEMFMT, "[7:0]", "writing any data to this register starts an automatic decode cycle");
}

static void
mlc__mlcrpr (uint32_t unused val)
{
	printf(ITEMFMT, "[7:0]", "writing any data to this register forces the controller to read 10-byte parity data from the NAND flash device");
}

static void
mlc__mlcwpr (uint32_t unused val)
{
	printf(ITEMFMT, "[7:0]", "writing any data to this register forces the controller to write 10-byte parity data to the NAND flash device");
}

static void
mlc__mlcrubp (uint32_t unused val)
{
	printf(ITEMFMT, "[7:0]", "writing any data to this register forces the serial data buffer pointer to the start of the user data region");
}

static void
mlc__mlcrobp (uint32_t unused val)
{
	printf(ITEMFMT, "[0]", "writing any data to this register forces the serial data buffer pointer to the start of the overhead data region");
}

static void
mlc__mlcswwpaddlow (uint32_t unused val)
{
	printf(ITEMFMT, "[23:0]", "the lower bound for the write-protected area");
}

static void
mlc__mlcswwpaddhig (uint32_t unused val)
{
	printf(ITEMFMT, "[23:0]", "the upper bound for the write-protected area");
}

static void
mlc__mlcicr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3]", "protection");
	field = print_field(val, 3, 3);
	printf("software write protection %sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[2]", "size");
	field = print_field(val, 2, 2);
	if (field == 1)
		printf("large block flash device (2k + 64 byte pages)");
	else
		printf("small block flash device (512 + 16 byte pages)");
	printf("\n");

	printf(ITEMFMT, "[1]", "address word count");
	field = print_field(val, 1, 1);
	if (field == 1)
		printf("small page:4  large page:5");
	else
		printf("small page:3  large page:4");
	printf("\n");

	printf(ITEMFMT, "[0]", "flash I/O bus width");
	field = print_field(val, 0, 0);
	printf("%u-bit\n", field == 1? 16 : 8);
}

static void
mlc__mlctimereg (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[25:24]", "TCEA_DELAY");
	field = print_field(val, 25, 24);
	printf("%u\n", field);

	printf(ITEMFMT, "[23:19]", "BUSY_DELAY");
	field = print_field(val, 23, 19);
	printf("%u\n", field);

	printf(ITEMFMT, "[18:16]", "NAND_TA");
	field = print_field(val, 18, 16);
	printf("%u\n", field);

	printf(ITEMFMT, "[15:12]", "RD_HIGH");
	field = print_field(val, 15, 12);
	printf("%u\n", field);

	printf(ITEMFMT, "[11:8]", "RD_LOW");
	field = print_field(val, 11, 8);
	printf("%u\n", field);

	printf(ITEMFMT, "[7:4]", "WR_HIGH");
	field = print_field(val, 7, 4);
	printf("%u\n", field);

	printf(ITEMFMT, "[3:0]", "WR_LOW");
	field = print_field(val, 3, 0);
	printf("%u\n", field);
}

static void
mlc__mlcirqmr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[5]", "NAND ready");
	field = print_field(val, 5, 5);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[4]", "controller ready");
	field = print_field(val, 4, 4);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[3]", "decode failure");
	field = print_field(val, 3, 3);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[2]", "decode error detected");
	field = print_field(val, 2, 2);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[1]", "ECC encode/decode ready");
	field = print_field(val, 1, 1);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[0]", "software write protection fault");
	field = print_field(val, 0, 0);
	printf("%sabled\n", field == 1? "en" : "dis");
}

static void
mlc__mlcirqsr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[5]", "NAND ready");
	field = print_field(val, 5, 5);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[4]", "controller ready");
	field = print_field(val, 4, 4);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[3]", "decode failure");
	field = print_field(val, 3, 3);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[2]", "decode error detected");
	field = print_field(val, 2, 2);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[1]", "ECC encode/decode ready");
	field = print_field(val, 1, 1);
	printf("%sabled\n", field == 1? "en" : "dis");

	printf(ITEMFMT, "[0]", "software write protection fault");
	field = print_field(val, 0, 0);
	printf("%sabled\n", field == 1? "en" : "dis");
}

static void
mlc__mlclockpr (uint32_t unused val)
{
	printf(ITEMFMT, "[15:0]", "writing 0xa25e to this register unlocks access to MLC_SW_WP_ADD_LOW, MLC_SW_WP_ADD_HIG, MLC_ICR, MLC_ICR_MLC_WP_REG, and MLC_TIME_REG");
}

static void
mlc__mlcisr (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[6]", "decoder failure");
	print_field(val, 6, 6);
	printf("\n");

	printf(ITEMFMT, "[5:4]", "number of R/S symbol errors");
	field = print_field(val, 5, 4);
	printf("%u\n", field);

	printf(ITEMFMT, "[3]", "errors detected");
	print_field(val, 3, 3);
	printf("\n");

	printf(ITEMFMT, "[2]", "ECC ready");
	print_field(val, 2, 2);
	printf("\n");

	printf(ITEMFMT, "[1]", "controller ready");
	print_field(val, 1, 1);
	printf("\n");

	printf(ITEMFMT, "[0]", "NAND ready");
	print_field(val, 0, 0);
	printf("\n");
}

static void
mlc__mlcceh (uint32_t val)
{
	uint32_t field;

	field = print_field(val, 0, 0);
	if (field == 1)
		printf("force nCE assert");
	else
		printf("normal nCE operation (controlled by controller)");
	printf("\n");
}

RegisterDescription_t mlc[] = {
	{0x200a8000, 0, "MLC_BUFF", "MLC NAND data buffer", accessRW, NULL},
	{0x200b0000, 0, "MLC_DATA", "start of MLC data buffer", accessRW, NULL},
	{0x200b8000, 0, "MLC_CMD", "MLC NAND flash command register", accessWrite, mlc__mlccmd},
	{0x200b8004, 0, "MLC_ADDR", "MLC NAND flash address register", accessWrite, mlc__mlcaddr},
	{0x200b8008, 0, "MLC_ECC_ENC_REG", "MLC NAND ECC encode register", accessWrite, mlc__mlceccencreg},
	{0x200b800c, 0, "MLC_ECC_DEC_REG", "MLC NAND ECC decode register", accessWrite, mlc__mlceccdecreg},
	{0x200b8010, 0, "MLC_ECC_AUTO_ENC_REG", "MLC NAND ECC auto encode register", accessWrite, mlc__mlceccautoencreg},
	{0x200b8014, 0, "MLC_ECC_AUTO_DEC_REG", "MLC NAND ECC auto decode register", accessWrite, mlc__mlceccautodecreg},
	{0x200b8018, 0, "MLC_RPR", "MLC NAND read parity register", accessWrite, mlc__mlcrpr},
	{0x200b801c, 0, "MLC_WPR", "MLC NAND write parity register", accessWrite, mlc__mlcwpr},
	{0x200b8020, 0, "MLC_RUBP", "MLC NAND reset user buffer pointer register", accessWrite, mlc__mlcrubp},
	{0x200b8024, 0, "MLC_ROBP", "MLC NAND reset overhead buffer pointer register", accessWrite, mlc__mlcrobp},
	{0x200b8028, 0, "MLC_SW_WP_ADD_LOW", "MLC NAND software write protection address low register", accessWrite, mlc__mlcswwpaddlow},
	{0x200b802c, 0, "MLC_SW_WP_ADD_HIG", "MLC NAND software write protection address high register", accessWrite, mlc__mlcswwpaddhig},
	{0x200b8030, 0, "MLC_ICR", "MLC NAND controller configuration register", accessWrite, mlc__mlcicr},
	{0x200b8034, 0x37, "MLC_TIME_REG", "MLC NAND timing register", accessWrite, mlc__mlctimereg},
	{0x200b8038, 0,    "MLC_IRQ_MR", "MLC NAND interrupt mask register", accessWrite, mlc__mlcirqmr},
	{0x200b803c, 0,    "MLC_IRQ_SR", "MLC NAND interrupt status register", accessRead, mlc__mlcirqsr},
	{0x200b8044, 0,    "MLC_LOCK_PR", "MLC NAND lock protection register", accessWrite, mlc__mlclockpr},
	{0x200b8048, 0,    "MLC_ISR", "MLC NAND status register", accessRead, mlc__mlcisr},
	{0x200b804c, 0,    "MLC_CEH", "MLC NAND chip-enable host control register", accessWrite, mlc__mlcceh},
};
size_t mlcSZ = (sizeof(mlc)/sizeof(mlc[0]));
