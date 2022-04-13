// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

#include "registers.h"

static void spi_init (void);
static void spi_reset (void);
static void spi_deinit (void);
static bool bootstick_present (void);
static void cs_high (void);
static void cs_low (void);
static void spi_tx (uint8_t *data_p, uint32_t len);
static void spi_rx (uint8_t *data_p, uint32_t len);
static void spi_readflash (uint8_t *data_p, uint32_t addr, uint32_t len);
static void spi_getid (void);
static void print_buf (uint32_t len);

static uint8_t buf_G[256];
static bool bootstick_G = false;

int
main (int argc, char *argv[])
{
	int c;
	struct option longOpts[] = {
		{"bootstick", no_argument, NULL, 'b'},
		{NULL, 0, NULL, 0},
	};

	while (1) {
		c = getopt_long(argc, argv, "b", longOpts, NULL);
		if (c == -1)
			break;
		switch (c) {
			case 'b':
				if (!bootstick_present()) {
					fprintf(stderr, "no bootstick present\n");
					lpc32x0__cleanup();
					return 1;
				}
				bootstick_G = true;
				break;
		}
	}

	spi_init();
	spi_getid();
	spi_reset();
	spi_readflash(buf_G, 0, sizeof(buf_G));
	spi_deinit();

	lpc32x0__cleanup();
	return 0;
}

static void
print_buf (uint32_t len)
{
	uint32_t i;

	for (i=0; i<len; ++i) {
		printf("%02x", buf_G[i]);
		if (((i+1) % 16) == 0)
			printf("\n");
		else
			printf(" ");
	}
	printf("\n");
}

static void
spi_init (void)
{
	// P2_MUX_CLR - set GPIO_05/SSEL0 as GPIO_05
	//            - set GPIO_04/SSEL1 as GPIO_04
	lpc32x0__set_reg(P2_MUX_CLR, 0x30);

	// P2_DIR_SET - enable GPIO_05 and GPIO_04 as outputs
	lpc32x0__set_reg(P2_DIR_SET, 0x60000000);

	cs_high();

	// SPI control - enable SPI1 and let it drive the SPI1_DATIO and
	//               SPI1_CLK pins
	lpc32x0__set_reg(SPI_CTRL, 0x03);

	// SPI1 global - enable SPI1
	lpc32x0__set_reg(SPI1_GLOBAL, 0x01);

	// SPI1 global - reset
	lpc32x0__set_reg(SPI1_GLOBAL, 0x03);
	lpc32x0__set_reg(SPI1_GLOBAL, 0x01);
}

static void
spi_reset (void)
{
	lpc32x0__get_and_print_reg_set_by_name("clkpwr", false);
	lpc32x0__get_and_print_reg_set_by_name("spi", false);
	lpc32x0__get_and_print_reg_set_by_name("ssp", false);

	// SPI1 global - reset
	lpc32x0__set_reg(SPI1_GLOBAL, 0x03);
	lpc32x0__set_reg(SPI1_GLOBAL, 0x01);

	lpc32x0__get_and_print_reg_set_by_name("clkpwr", false);
	lpc32x0__get_and_print_reg_set_by_name("spi", false);
	lpc32x0__get_and_print_reg_set_by_name("ssp", false);
}

static void
spi_deinit (void)
{
}

/*
 * on my board an optional "bootstick" can be plugged into a header on the board
 * the bootstick is basically a spi-nor on a breakout board
 * the device has circuitry to detect whether or not the bootstick is present
 * and, if so, switch around the chip selects so that the bootstick is
 * selected rather than the on-board spi-nor using the same chip select line
 *
 * if the bootstick is present:
 * 	chip select for on-board spi-nor:  GPIO_4
 * 	chip select for bootstick spi-nor: GPIO_5
 * else
 * 	chip select for on-board spi-nor:  GPIO_5
 */
static bool
bootstick_present (void)
{
	uint32_t val;

	lpc32x0__get_reg(P3_INP_STATE, &val);
	if (val & 0x08)
		return true;
	return false;
}

static void
cs_high (void)
{
	if (bootstick_present() && !bootstick_G)
		lpc32x0__set_reg(P3_OUTP_SET, 0x20000000);
	else
		lpc32x0__set_reg(P3_OUTP_SET, 0x40000000);
}

static void
cs_low (void)
{
	if (bootstick_present() && !bootstick_G)
		lpc32x0__set_reg(P3_OUTP_CLR, 0x20000000);
	else
		lpc32x0__set_reg(P3_OUTP_CLR, 0x40000000);
}

static void
spi_tx (uint8_t *data_p, uint32_t len)
{
	uint32_t val, frameLen;

	if (len == 0)
		return;

	// SPI1_CON - xmit, shift enabled
	lpc32x0__set_reg(SPI1_CON, 0x808e83);

	while (len) {
		if (len > 65535)
			frameLen = 65535;
		else
			frameLen = len;
		// SPI1_FRM
		lpc32x0__set_reg(SPI1_FRM, frameLen);
		len -= frameLen;

		while (frameLen--) {
			lpc32x0__get_reg(SPI1_STAT, &val);
			while (val & 0x04)
				lpc32x0__get_reg(SPI1_STAT, &val);
			lpc32x0__set_reg(SPI1_DAT, *data_p++);
		}

		lpc32x0__get_reg(SPI1_STAT, &val);
		while ((val & 0x01) == 0)
			lpc32x0__get_reg(SPI1_STAT, &val);
		lpc32x0__set_reg(SPI1_STAT, 0x100);
	}
}

static void
spi_rx (uint8_t *data_p, uint32_t len)
{
	uint32_t val, frameLen;

	if (len == 0)
		return;

	// SPI1_CON - rcve, shift enabled
	lpc32x0__set_reg(SPI1_CON, 0x800e83);

	while (len) {
		if (len > 65535)
			frameLen = 65535;
		else
			frameLen = len;
		len -= frameLen;
		// SPI1_FRM
		lpc32x0__set_reg(SPI1_FRM, frameLen);

		// dummy read?
		lpc32x0__get_reg(SPI1_DAT, &val);

		while (frameLen--) {
			lpc32x0__get_reg(SPI1_STAT, &val);
			while (val & 0x01)
				lpc32x0__get_reg(SPI1_STAT, &val);

			lpc32x0__get_reg(SPI1_STAT, &val);
			if (val & 0x08)
				lpc32x0__set_reg(SPI1_CON, 0x802E83);

			lpc32x0__get_reg(SPI1_DAT, &val);
			*data_p++ = val;
		}

		// SPI1_STAT - clear interrupt
		lpc32x0__set_reg(SPI1_STAT, 0x100);
	}
}

static void
spi_readflash (uint8_t *data_p, uint32_t addr, uint32_t len)
{
	uint8_t command[5];

	if (len == 0)
		return;

	memset(buf_G, 0, sizeof(buf_G));
	command[0] = 0x0b;
	command[1] = (addr >> 16) & 0xff;
	command[2] = (addr >> 8) & 0xff;
	command[3] = addr & 0xff;
	command[4] = 0;

	cs_low();
	spi_tx(command, 5);
	spi_rx(data_p, len);
	cs_high();
	print_buf(len);
}

static void
spi_getid (void)
{
	uint8_t command;

	memset(buf_G, 0, sizeof(buf_G));
	command = 0x9f;

	cs_low();
	spi_tx(&command, 1);
	spi_rx(buf_G, 3);
	cs_high();
	print_buf(3);
}
