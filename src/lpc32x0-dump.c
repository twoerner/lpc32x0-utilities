// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

#include "registers.h"

extern AllRegisters_t AllRegisters_G[];
extern size_t AllRegistersSZ;

static void usage(char *pgm_p);

int
main (int argc, char *argv[])
{
	int c;
	int retVal = -1;
	char *nextTok_p;
	char *regSet_p = NULL;
	char *reg_p = NULL;
	bool doSet = false;
	bool doReg = false;
	bool verbose = false;
	struct option longOpts[] = {
		{"verbose", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{"set", required_argument, NULL, 's'},
		{"reg", required_argument, NULL, 'r'},
		{NULL, 0, NULL, 0},
	};

	while (1) {
		c = getopt_long(argc, argv, "vhs:r:", longOpts, NULL);
		if (c == -1)
			break;
		switch (c) {
			case 'v':
				verbose = true;
				break;
			case 'h':
				usage(argv[0]);
				return 0;
			case 's':
				doSet = true;
				regSet_p = strdup(optarg);
				if (regSet_p == NULL) {
					perror("strdup()");
					return -1;
				}
				break;

			case 'r':
				doReg = true;
				reg_p = strdup(optarg);
				if (reg_p == NULL) {
					perror("strdup()");
					return -1;
				}
				break;
		}
	}

	if (doSet) {
		if (regSet_p == NULL) {
			printf("internal error\n");
			goto badexit;
		}

		nextTok_p = strtok(regSet_p, " ,");
		while (nextTok_p != NULL) {
			lpc32x0__get_and_print_reg_set_by_name(nextTok_p, verbose);
			nextTok_p = strtok(NULL, " ,");
		}
	}

	if (doReg) {
		int tmp;
		uint32_t addr, val;

		if (reg_p == NULL) {
			printf("internal error\n");
			goto badexit;
		}

		nextTok_p = strtok(reg_p, " ,");
		while (nextTok_p != NULL) {
			if ((sscanf(nextTok_p, "%i", &tmp)) != 1)
				printf("can't convert '%s' to register\n", nextTok_p);
			else {
				addr = (uint32_t)tmp;
				if (lpc32x0__get_reg(addr, &val))
					lpc32x0__print_reg(addr, val, true);
			}
			nextTok_p = strtok(NULL, " ,");
		}
	}

	// dump all registers
	if ((!doSet) && (!doReg))
		lpc32x0__get_and_print_all_regs(verbose);

	retVal = 0;
badexit:
	if (regSet_p != NULL)
		free(regSet_p);
	if (reg_p != NULL)
		free(reg_p);
	return retVal;
}

static void
usage (char *pgm_p)
{
	size_t i;

	printf("usage:\n");
	if (pgm_p != NULL)
		printf("%s [<options>]\n", pgm_p);
	printf("  where:\n");
	printf("    options:\n");
	printf("      -h|--help      print usage information and exit successfully\n");
	printf("      -v|--verbose   print register value and sub-field values\n");
	printf("      -r|--reg <reg> specify register(s) by address\n");
	printf("      -s|--set <set> specify set(s) of registers by name\n");
	printf("      register set names are:\n");
	for (i=0; i<AllRegistersSZ; ++i)
		printf("        %s\n", AllRegisters_G[i].name_p);
}
