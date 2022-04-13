// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "registers.h"

static void usage (char *pgm_p);

int
main (int argc, char *argv[])
{
	int c;
	int ret=1;
	int addr, value;
	uint32_t before, after;
	struct option longOpts[] = {
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0},
	};

	while (1) {
		c = getopt_long(argc, argv, "h", longOpts, NULL);
		if (c == -1)
			break;
		switch (c) {
			case 'h':
				usage(argv[0]);
				return 0;
		}
	}

	// there should be 2 arguments left over
	if (argc != (optind+2)) {
		printf("incorrect number of cmdline arguments\n");
		usage(argv[0]);
		return 1;
	}
	if ((sscanf(argv[optind++], "%i", &addr)) != 1) {
		printf("can't convert addr\n");
		return 1;
	}
	if ((sscanf(argv[optind], "%i", &value)) != 1) {
		printf("can't convert value\n");
		return 1;
	}

	if (!lpc32x0__get_reg(addr, &before)) {
		printf("can't get register at addr 0x%08x\n", addr);
		goto exitmemfd;
	}
	if (before != (uint32_t)value) {
		if (!lpc32x0__set_reg(addr, value)) {
			printf("can't set register 0x%08x to 0x%08x\n", addr, value);
			goto exitmemfd;
		}
		if (!lpc32x0__get_reg(addr, &after)) {
			printf("can't get register at addr 0x%08x\n", addr);
			goto exitmemfd;
		}
		lpc32x0__print_reg(addr, before, true);
		lpc32x0__print_reg(addr, after, true);
	}
	else
		printf("0x%08x already set to 0x%08x\n", addr, value);

	ret = 0;
exitmemfd:
	lpc32x0__cleanup();
	return ret;
}

static void
usage (char *pgm_p)
{
	printf("set the contents of a register at <addr> to a given <value>\n");
	printf("usage:\n");
	printf("  %s <addr> <value>\n", pgm_p);
}
