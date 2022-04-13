// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>

#include "registers.h"

int
main (void)
{
	int cvt;
	char buf[256];
	uint32_t addr, val1, val2, val3, val4;

	printf("specify a register and its value and this program will break it down into fields\n");
	printf("you can specify up to 4 values, which the code will assume are the next 3 registers\n");
	printf("use the following form:\n");
	printf("    <reg1>: <reg1-value> [<reg2-value> [<reg3-value> [<reg4-value>]]]\n\n");

	while(fgets(buf,sizeof(buf)-1,stdin)) {
		addr = 0;
		val1 = 0;
		val2 = 0;
		val3 = 0;
		val4 = 0;

		cvt = sscanf(buf, "%x: %x %x %x %x", &addr, &val1, &val2, &val3, &val4);

		/*
		 * Adding the 0xc to the lower addresses is done to account
		 * for the fact that the register dump might not be aligned to
		 * the start of a particular region's addresses, and the fact
		 * each dump line can have up to 4 values (per start address).
		 *
		 * e.g. clock and power starts at 0x40004000, but the dump
		 * might have been done such that:
		 *   0x40003ff4: (*0x40003ff4) (*0x40003ff8) (*0x40003ffc) (*0x40004000)
		 *   0x40004004: (*0x40004004) (*0x40004008) (*0x4000400c) (*0x40004010)
		 *
		 * This means addr must be less than 0xfffffff4
		 */
		if (addr >= 0xfffffff4)
			continue;

		if (cvt > 1)
			lpc32x0__print_reg(addr, val1, true);
		if (cvt > 2)
			lpc32x0__print_reg(addr+4, val2, true);
		if (cvt > 3)
			lpc32x0__print_reg(addr+8, val3, true);
		if (cvt > 4)
			lpc32x0__print_reg(addr+12, val4, true);
	}

	return 0;
}
