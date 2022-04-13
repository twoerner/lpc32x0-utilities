// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>

#include "registers.h"

static char *p0state[] = {
	"P0.0 / I2S1 RX_CLK",
	"P0.1 / I2S1 RX_WS",
	"P0.2 / I2S0 RX_SDA | LCDVD[4]",
	"P0.3 / I2S0 RX_CLK | LCDVD[5]",
	"P0.4 / I2S0 RS_WS | LCDVD[6]",
	"P0.5 / I2S0 TX_SDA | LCDVD[7]",
	"P0.6 / I2S0 TX_CLK | LCDVD[12]",
	"P0.7 / I2S0 TX_WS | LCDVD[13]",
};

static void
gpio__p0state (uint32_t val)
{
	int i;

	for (i=7; i>-1; --i) {
		print_field(val, i, i);
		printf("%s\n", p0state[i]);
	}
}

static void
gpio__p0dirstate (uint32_t val)
{
	int i;
	char buf[10];
	uint32_t field;

	for (i=7; i>-1; --i) {
		sprintf(buf, "[%d]", i);
		printf(ITEMFMT, buf, p0state[i]);
		field = print_field(val, i, i);
		if (field == 1)
			printf("output\n");
		else
			printf("input\n");
	}
}

static char *p1state[] = {
	"EMC_A[0] / P1.0",
	"EMC_A[1] / P1.1",
	"EMC_A[2] / P1.2",
	"EMC_A[3] / P1.3",
	"EMC_A[4] / P1.4",
	"EMC_A[5] / P1.5",
	"EMC_A[6] / P1.6",
	"EMC_A[7] / P1.7",
	"EMC_A[8] / P1.8",
	"EMC_A[9] / P1.9",
	"EMC_A[10] / P1.10",
	"EMC_A[11] / P1.11",
	"EMC_A[12] / P1.12",
	"EMC_A[13] / P1.13",
	"EMC_A[14] / P1.14",
	"EMC_A[15] / P1.15",
	"EMC_A[16] / P1.16",
	"EMC_A[17] / P1.17",
	"EMC_A[18] / P1.18",
	"EMC_A[19] / P1.19",
	"EMC_A[20] / P1.20",
	"EMC_A[21] / P1.21",
	"EMC_A[22] / P1.22",
	"EMC_A[23] / P1.23",
};
static void
gpio__p1state (uint32_t val)
{
	int i;

	for (i=23; i>-1; --i) {
		print_field(val, i, i);
		printf("EMC_A[%d] / P1.%d\n", i, i);
	}
}

static void
gpio__p1dirstate (uint32_t val)
{
	int i;
	char buf[10];
	uint32_t field;

	for (i=23; i>-1; --i) {
		sprintf(buf, "[%d]", i);
		printf(ITEMFMT, buf, p1state[i]);
		field = print_field(val, i, i);
		if (field == 1)
			printf("output\n");
		else
			printf("input\n");
	}
}

static char *p23state[] = {
	"EMC_D[19] / P2.0",
	"EMC_D[20] / P2.1",
	"EMC_D[21] / P2.2",
	"EMC_D[22] / P2.3",
	"EMC_D[23] / P2.4",
	"EMC_D[24] / P2.5",
	"EMC_D[25] / P2.6",
	"EMC_D[26] / P2.7",
	"EMC_D[27] / P2.8",
	"EMC_D[28] / P2.9",
	"EMC_D[29] / P2.10",
	"EMC_D[30] / P2.11",
	"EMC_D[31] / P2.12",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"(reserved)",
	"GPIO_0",
	"GPIO_1",
	"GPIO_2",
	"GPIO_3",
	"GPIO_4",
	"GPIO_5",
};

static void
gpio__p2state (uint32_t val)
{
	int i;

	for (i=12; i>-1; --i) {
		print_field(val, i, i);
		printf("%s\n", p23state[i]);
	}
}

static void
gpio__p23dirstate (uint32_t val)
{
	int i;
	char buf[10];
	uint32_t field;

	for (i=30; i>24; --i) {
		sprintf(buf, "[%d]", i);
		printf(ITEMFMT, buf, p23state[i]);
		field = print_field(val, i, i);
		if (field == 1)
			printf("output\n");
		else
			printf("input\n");
	}
	for (i=12; i>-1; --i) {
		sprintf(buf, "[%d]", i);
		printf(ITEMFMT, buf, p23state[i]);
		field = print_field(val, i, i);
		if (field == 1)
			printf("output\n");
		else
			printf("input\n");
	}

}

static void
gpio__p3instate (uint32_t val)
{
	print_field(val, 31, 29);
	printf("(reserved)\n");
	print_field(val, 28, 28);
	printf("GPI_28 / U3_RI\n");
	print_field(val, 27, 27);
	printf("GPI_27 / SPI2_DATIN / MISO1 | LCDVD[21]\n");
	print_field(val, 26, 26);
	printf("(reserved)");
	print_field(val, 25, 25);
	printf("GPI_25 / SPI1_DATIN / MISO0 / MCFB2\n");
	print_field(val, 24, 24);
	printf("GPIO_5 / SSEL0 / MCFB0\n");
	print_field(val, 23, 23);
	printf("GPI_23 / U7_RX / CAP0.0 | LCDVD[10]\n");
	print_field(val, 22, 22);
	printf("GPI_22 / U7_HCTS / CAP0.1 | LCDCLKIN\n");
	print_field(val, 21, 21);
	printf("GPI_21 / U6_IRRX\n");
	print_field(val, 20, 20);
	printf("GPI_20 / U5_RX\n");
	print_field(val, 19, 19);
	printf("GPI_19 / U4_RX\n");
	print_field(val, 18, 18);
	printf("GPI_18 / U3_RX\n");
	print_field(val, 17, 17);
	printf("GPI_17 / U2_RX | U3_DSR\n");
	print_field(val, 16, 16);
	printf("GPI_16 / U2_HCTS | U3_CTS\n");
	print_field(val, 15, 15);
	printf("GPI_15 / U1_RX | CAP1.0\n");
	print_field(val, 14, 14);
	printf("GPI_4 / SSEL1 | LCDVD[22]\n");
	print_field(val, 13, 13);
	printf("GPI_3 / KEY_ROW7 | ENET_MDIO\n");
	print_field(val, 12, 12);
	printf("GPI2 / KEY_ROW6 | ENET_MDC\n");
	print_field(val, 11, 11);
	printf("GPIO_1\n");
	print_field(val, 10, 10);
	printf("GPIO_0\n");
	print_field(val, 9, 9);
	printf("GPI_9 / KEY_COL7 | ENET_COL\n");
	print_field(val, 8, 8);
	printf("GPI_8 / KEY_COL6 / SPI2_BUSY | ENET_RX_DV\n");
	print_field(val, 7, 7);
	printf("GPI_7\n");
	print_field(val, 6, 6);
	printf("GPI_6 / HSTIM_CAP | ENET_RXD2\n");
	print_field(val, 5, 5);
	printf("GPI_5 / U3_DCD\n");
	print_field(val, 4, 4);
	printf("GPI_4 / SPI1_BUSY\n");
	print_field(val, 3, 3);
	printf("GPI_3\n");
	print_field(val, 2, 2);
	printf("GPI_2 / CAP2.0 | ENET_RXD3\n");
	print_field(val, 1, 1);
	printf("GPI_1 / SERVICE_N\n");
	print_field(val, 0, 0);
	printf("GPI_0 / I2S1RX_SDA\n");
}

static void
gpio__p3outstate (uint32_t val)
{
	print_field(val, 31, 31);
	printf("(reserved)\n");
	print_field(val, 30, 30);
	printf("GPIO_5 / SSEL0 / MCFB0\n");
	print_field(val, 29, 29);
	printf("GPIO_4 / SSEL1 | LCDVD[22]\n");
	print_field(val, 28, 28);
	printf("GPIO_3 / KEY_ROW7 | ENET_MDIO\n");
	print_field(val, 27, 27);
	printf("GPIO_2 / KEY_ROW6 | ENET_MDC\n");
	print_field(val, 26, 26);
	printf("GPIO_1\n");
	print_field(val, 25, 25);
	printf("GPIO_0\n");
	print_field(val, 24, 24);
	printf("(reserved)\n");
	print_field(val, 23, 23);
	printf("GPO_23 / U2_HRTS | U3_RTS\n");
	print_field(val, 22, 22);
	printf("GPO_22 / U7_HRTS / LCDVD[22]\n");
	print_field(val, 21, 21);
	printf("GPO_21 / U4_TX | LCDVD[3]\n");
	print_field(val, 20, 20);
	printf("GPO_20\n");
	print_field(val, 19, 19);
	printf("GPO_19\n");
	print_field(val, 18, 18);
	printf("GPO_18 / MC0A | LCDLP\n");
	print_field(val, 17, 17);
	printf("GPO_17\n");
	print_field(val, 16, 16);
	printf("GPO_16 / MC0B | LCDLP\n");
	print_field(val, 15, 15);
	printf("GPO_15 / MC1A | LCDFP\n");
	print_field(val, 14, 14);
	printf("GPO_14\n");
	print_field(val, 13, 13);
	printf("GPO_13 / MC1B | LCDDCLK\n");
	print_field(val, 12, 12);
	printf("GPO_12 / KC2A | LCDLE\n");
	print_field(val, 11, 11);
	printf("GPO_11\n");
	print_field(val, 10, 10);
	printf("GPO_10 / MC2B | LCDPWR\n");
	print_field(val, 9, 9);
	printf("GPO_9 | LCDVD[9]\n");
	print_field(val, 8, 8);
	printf("GPO_8 | LCDVD[8]\n");
	print_field(val, 7, 7);
	printf("GPO_7 | LCDVD[2]\n");
	print_field(val, 6, 6);
	printf("GPO_6 | LCDVD[18]\n");
	print_field(val, 5, 5);
	printf("GPO_5\n");
	print_field(val, 4, 4);
	printf("GPO_4\n");
	print_field(val, 3, 3);
	printf("GPO_3 | LCDVD[1]\n");
	print_field(val, 2, 2);
	printf("GPO_2 / T1_MAT.0 | LCDVD[0]\n");
	print_field(val, 1, 1);
	printf("GPO_1\n");
	print_field(val, 0, 0);
	printf("GPO_0 | TST_CLK1\n");
	
}

RegisterDescription_t gpio[] = {
	/* port 0 */
	{0x40028040, 0, "P0_INP_STATE", "Port 0 input pin state", accessRead, gpio__p0state},
	{0x40028044, 0, "P0_OUTP_SET", "Port 0 output pin set", accessWrite, NULL},
	{0x40028048, 0, "P0_OUTP_CLR", "Port 0 ouptut pin clear", accessWrite, NULL},
	{0x4002804c, 0, "P0_OUTP_STATE", "Port 0 output pin state", accessRead, gpio__p0state},
	{0x40028050, 0, "P0_DIR_SET", "Port 0 direction set", accessWrite, NULL},
	{0x40028054, 0, "P0_DIR_CLR", "Port 0 direction clear", accessWrite, NULL},
	{0x40028058, 0, "P0_DIR_STATE", "Port 0 direction state", accessRead, gpio__p0dirstate},
	/* port 1 */
	{0x40028060, 0, "P1_INP_STATE", "Port 1 input pin state", accessRead, gpio__p1state},
	{0x40028064, 0, "P1_OUTP_SET", "Port 1 output pin set", accessWrite, NULL},
	{0x40028068, 0, "P1_OUTP_CLR", "Port 1 ouptut pin clear", accessWrite, NULL},
	{0x4002806c, 0, "P1_OUTP_STATE", "Port 1 output pin state", accessRead, gpio__p1state},
	{0x40028070, 0, "P1_DIR_SET", "Port 1 direction set", accessWrite, NULL},
	{0x40028074, 0, "P1_DIR_CLR", "Port 1 direction clear", accessWrite, NULL},
	{0x40028078, 0, "P1_DIR_STATE", "Port 1 direction state", accessRead, gpio__p1dirstate},
	/* port 2 */
	{0x4002801c, 0, "P2_INP_STATE", "Port 2 input pin state", accessRead, gpio__p2state},
	{0x40028020, 0, "P2_OUTP_SET", "Port 2 output pin set", accessWrite, NULL},
	{0x40028024, 0, "P2_OUTP_CLR", "Port 2 ouptut pin clear", accessWrite, NULL},
	{0x40028010, 0, "P2_DIR_SET", "Port 2/3 direction set", accessWrite, gpio__p23dirstate},
	{0x40028014, 0, "P2_DIR_CLR", "Port 2/3 direction clear", accessWrite, NULL},
	{0x40028018, 0, "P2_DIR_STATE", "Port 2/3 direction state", accessRead, NULL},
	/* port 3 */
	{0x40028000, 0, "P3_INP_STATE", "Port 3 input pin state", accessRead, gpio__p3instate},
	{0x40028004, 0, "P3_OUTP_SET", "Port 3 output pin set", accessWrite, NULL},
	{0x40028008, 0, "P3_OUTP_CLR", "Port 3 output pin clear", accessWrite, NULL},
	{0x4002800c, 0, "P3_OUTP_STATE", "Port 3 output pin state", accessRead, gpio__p3outstate},
};
size_t gpioSZ = sizeof(gpio)/sizeof(gpio[0]);
