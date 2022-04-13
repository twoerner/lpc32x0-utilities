// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include "registers.h"

static void
pinmux__uartctrl (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[11]", "UART3_MD_CTRL");
	field = print_field(val, 11, 11);
	if (field == 0)
		printf("UART3 does not use modem control pins");
	else
		printf("UART3 uses modem control pins");
	printf("\n");

	printf(ITEMFMT, "[10]", "HDPX_INV");
	field = print_field(val, 10, 10);
	if (field == 0)
		printf("IRRX6 is not inverted");
	else
		printf("IRRX6 is inverted");
	printf("\n");

	printf(ITEMFMT, "[9]", "HDPX_EN");
	field = print_field(val, 9, 9);
	if (field == 0)
		printf("IRRX6 is not disabled by TXD");
	else
		printf("IRRX6 is masked while TXD is low");
	printf("\n");

	printf(ITEMFMT, "[8:6]", "(reserved)");
	field = print_field(val, 8, 6);
	printf("\n");

	printf(ITEMFMT, "[5]", "UART6_IRDA");
	field = print_field(val, 5, 5);
	if (field == 0)
		printf("UART6 uses the IrDA modem");
	else
		printf("UART6 bypasses the IrDA modem");
	printf("\n");

	printf(ITEMFMT, "[4]", "IRTX6_INV");
	field = print_field(val, 4, 4);
	if (field == 0)
		printf("IRTX6 is not inverted");
	else
		printf("IRTX6 is inverted");
	printf("\n");

	printf(ITEMFMT, "[3]", "IRRX6_INV");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("IRRX6 is not inverted");
	else
		printf("IRRX6 is inverted");
	printf("\n");

	printf(ITEMFMT, "[2]", "IR_RxLength");
	field = print_field(val, 2, 2);
	if (field == 0)
		printf("the IrDA expects Rx pulses 3/16 of the selected bit period");
	else
		printf("the IrDA expects Rx pulses 3/16 of a 115.2kbps bit period");
	printf("\n");

	printf(ITEMFMT, "[1]", "IR_TxLength");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("the IrDA Tx uses 3/16 of the selected bit period");
	else
		printf("the IrDA Tx uses 3/16 of a 115.2kbps bit period");
	printf("\n");

	printf(ITEMFMT, "[0]", "UART5_MODE");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("UART5 TX/RX is only routed to the U5_TX and U5_RX pins");
	else
		printf("UART5 TX/RX function is also routed to the USB D+ and D- pins");
}

static void
pinmux__ms (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:11]", "(reserved)");
	print_field(val, 31, 11);
	printf("\n");

	printf(ITEMFMT, "[10]", "disable SD card pins");
	field = print_field(val, 10, 10);
	if (field == 1)
		printf("disable SD card pins");
	else
		printf("enable SD card pins");
	printf("\n");

	printf(ITEMFMT, "[9]", "enables clock and pull-ups");
	field = print_field(val, 9, 9);
	if (field == 0)
		printf("MSSDIO pull-up and clock disabled");
	else
		printf("MSSDIO pull-up and clock enabled");
	printf("\n");

	printf(ITEMFMT, "[8]", "MSSDIO2 and MSSDIO3 pad control");
	field = print_field(val, 8, 8);
	if (field == 0)
		printf("MSSDIO2 and 3 pad has pull-up enabled");
	else
		printf("MSSDIO2 and 3 pad has no pull-up");
	printf("\n");

	printf(ITEMFMT, "[7]", "MSSDIO1 pad control");
	field = print_field(val, 7, 7);
	if (field == 0)
		printf("MSSDIO1 pad has pull-up enabled");
	else
		printf("MDDSIO1 pad has no pull-up");
	printf("\n");

	printf(ITEMFMT, "[6]", "MSSDIO0/MSBS pad control");
	field = print_field(val, 6, 6);
	if (field == 0)
		printf("MSSDIO0 pad has pull-up enabled");
	else
		printf("MSSDIO0 pad has no pull-up");
	printf("\n");

	printf(ITEMFMT, "[5]", "SD card clock control");
	field = print_field(val, 5, 5);
	if (field == 0)
		printf("clocks disabled");
	else
		printf("clocks enabled");
	printf("\n");

	printf(ITEMFMT, "[4]", "(reserved)");
	print_field(val, 4, 4);
	printf("\n");

	printf(ITEMFMT, "[3:0]", "clock divider ratio");
	field = print_field(val, 3, 0);
	if (field == 0)
		printf("MSSDCLK stopped, divider in low power mode");
	else
		printf("MSSDCLK = ARM PLL output clock divided by %d", field);
	printf("\n");
}

static void
pinmux__state (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:16]", "(reserved)");
	print_field(val, 31, 16);
	printf("\n");

	printf(ITEMFMT, "[15]", "U7_TX / MAT1.1 | LCDVD[11]");
	field = print_field(val, 15, 15);
	if (field == 1)
		printf("MAT1.1");
	else
		printf("U7_TX");
	printf(", unless LCD is enabled in which case LCDVD[11]\n");

	printf(ITEMFMT, "[14:13]", "(reserved)");
	print_field(val, 14, 13);
	printf("\n");

	printf(ITEMFMT, "[12]", "SPI1_CLK / SCK0");
	field = print_field(val, 12, 12);
	if (field == 1)
		printf("SCK0");
	else
		printf("SPI1_CLK");
	printf("\n");

	printf(ITEMFMT, "[11]", "(reserved)");
	print_field(val, 11, 11);
	printf("\n");

	printf(ITEMFMT, "[10]", "SPI1_DATIN / MISO0 / MCFB2");
	field = print_field(val, 10, 10);
	if (field == 1)
		printf("MISO0, unless TIMCLK_CTRL1[6] == 1");
	else
		printf("SPI1_DATIN");
	printf("\n");

	printf(ITEMFMT, "[9]", "SPI1_DATIO / MOSI0 / MCFB1");
	field = print_field(val, 9, 9);
	if (field == 1)
		printf("MOSI0, unless TIMCLK_CTRL1[6] == 1");
	else
		printf("SPI1_DATIO");
	printf("\n");

	printf(ITEMFMT, "[8]", "SPI2_CLK / SCK1 | LCDVD[23]");
	field = print_field(val, 8, 8);
	if (field == 1)
		printf("SCK1");
	else
		printf("SPI2_CLK");
	printf(", unless LCD is enabled in which case LCDVD[12]\n");

	printf(ITEMFMT, "[7]", "(reserved)");
	print_field(val, 7, 7);
	printf("\n");

	printf(ITEMFMT, "[6]", "SPI2_DATIN / MISO1 | LCDVD[21]");
	field = print_field(val, 6, 6);
	if (field == 1)
		printf("MISO1");
	else
		printf("SPI2_DATIN");
	printf(", unless LCD is enabled in which case LCDVD[21]\n");

	printf(ITEMFMT, "[5]", "SPI2_DATIO / MOSI1 | LCDVD[20]");
	field = print_field(val, 5, 5);
	if (field == 1)
		printf("MOSI1");
	else
		printf("SPI2_DATIO");
	printf(", unless LCD is enabled in which case LCDVD[20]\n");

	printf(ITEMFMT, "[4]", "I2S1TX_WS / CAP3.0");
	field = print_field(val, 4, 4);
	if (field == 1)
		printf("CAP3.0");
	else
		printf("I2S1TX_WS");
	printf("\n");

	printf(ITEMFMT, "[3]", "I2S1TX_CLK / MAT3.0");
	field = print_field(val, 3, 3);
	if (field == 1)
		printf("CAP3.0");
	else
		printf("I2S1TX_CLK");
	printf("\n");

	printf(ITEMFMT, "[2]", "I2S1TX_SDA / MAT3.1");
	field = print_field(val, 2, 2);
	if (field == 1)
		printf("MAT3.1");
	else
		printf("I2S1TX_SDA");
	printf("\n");

	printf(ITEMFMT, "[1:0]", "(reserved)");
	print_field(val, 1, 0);
}

static void
pinmux__p0state (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:8]", "(reserved)");
	print_field(val, 31, 8);
	printf("\n");

	printf(ITEMFMT, "[7]", "P0.7 / L2S0TX_WS | LCDVD[13]");
	field = print_field(val, 7, 7);
	if (field == 1)
		printf("I2S0TX_WS");
	else
		printf("P0.7");
	printf(", unless LCD is enabled in which case LCDVD[13]\n");

	printf(ITEMFMT, "[6]", "P0.6 / I2S0TX_CLK | LCDVD[12]");
	field = print_field(val, 6, 6);
	if (field == 1)
		printf("I2S0TX_CLK");
	else
		printf("P0.6");
	printf(", unless LCD is enabled in which case LCDVD[12]\n");

	printf(ITEMFMT, "[5]", "P0.5 / I2S0TX_SDA | LCDVD[7]");
	field = print_field(val, 5, 5);
	if (field == 1)
		printf("I2S0TX_SDA");
	else
		printf("P0.5");
	printf(", unless LCD is enabled in which case LCDVD[7]\n");

	printf(ITEMFMT, "[4]", "P0.4 / I2S0RX_WS | LCDVD[6]");
	field = print_field(val, 4, 4);
	if (field == 1)
		printf("I2S0RX_WS");
	else
		printf("P0.4");
	printf(", unless LCD is enabled in which case LCDVD[6]\n");

	printf(ITEMFMT, "[3]", "P0.3 / I2S0RX_CLK | LCDVD[5]");
	field = print_field(val, 3, 3);
	if (field == 1)
		printf("I2S0RX_CLK");
	else
		printf("P0.3");
	printf(", unless LCD is enabled in which case LCDVD[5]\n");

	printf(ITEMFMT, "[2]", "P0.2 / I2S0RX_SDA | LCDVD[4]");
	field = print_field(val, 2, 2);
	if (field == 1)
		printf("I2S0RX_SDA");
	else
		printf("P0.2");
	printf(", unless LCD is enabled in which case LCDVD[4]\n");

	printf(ITEMFMT, "[1]", "P0.1 / I2S1RX_WS");
	field = print_field(val, 1, 1);
	if (field == 1)
		printf("I2S1RX_WS");
	else
		printf("P0.1");
	printf("\n");

	printf(ITEMFMT, "[0]", "P0.0 / I2S1RX_CLK");
	field = print_field(val, 0, 0);
	if (field == 1)
		printf("I2S1RX_CLK");
	else
		printf("P0.0");
	printf("\n");
}

static void
pinmux__p1state (uint32_t val)
{
	int i;
	uint32_t field;
	char buf1[32];
	char buf2[32];

	printf(ITEMFMT, "[31:24]", "(reserved)");
	print_field(val, 31, 24);
	printf("\n");

	for (i=23; i>-1; --i) {
		sprintf(buf1, "[%d]", i);
		sprintf(buf2, "EMC_A[%02d] / P1.%d", i, i);
		printf(ITEMFMT, buf1, buf2);
		field = print_field(val, i, i);
		if (field == 1)
			printf("P1.%d", i);
		else
			printf("EMC_A[%02d]", i);
		printf("\n");
	}
}

static void
pinmux__p2state (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:6]", "(reserved)");
	print_field(val, 31, 6);
	printf("\n");

	printf(ITEMFMT, "[5]", "GPIO_5 / SSEL0 / MCFB0");
	field = print_field(val, 5, 5);
	if (field == 1)
		printf("SSEL0, unless TIMCLK_CTRL1[6] == 1 in which case MCFB0");
	else
		printf("GPIO_5");
	printf("\n");

	printf(ITEMFMT, "[4]", "GPIO_4 / SSEL1 | LCDVD[22]");
	field = print_field(val, 4, 4);
	if (field == 1)
		printf("SSEL1");
	else
		printf("GPIO_4");
	printf(", unless LCD is enabled in which case LCDVD[22]\n");

	printf(ITEMFMT, "[3]", "EMC_D_SEL");
	field = print_field(val, 3, 3);
	if (field == 1)
		printf("EMC_D[31:19] pins are connected to the GPIO block");
	printf("\n");

	printf(ITEMFMT, "[2]", "GPO_21 / U4_TX | LCDVD[3]");
	field = print_field(val, 2, 2);
	if (field == 1)
		printf("U4_TX");
	else
		printf("GPO_21");
	printf(", unless LCD is enabled in which case LCDVD[3]\n");

	printf(ITEMFMT, "[1]", "GPIO_3 / KEY_ROW7 | ENET_MDIO");
	field = print_field(val, 1, 1);
	if (field == 1)
		printf("KEY_ROW7");
	else
		printf("GPIO_3");
	printf(", unless ethernet MAC is enabled in which case ENET_MDIO\n");

	printf(ITEMFMT, "[0]", "GPIO_2 / KEY_ROW6 | ENET_MDC");
	field = print_field(val, 0, 0);
	if (field == 1)
		printf("KEY_ROW6");
	else
		printf("GPIO_2");
	printf(", unless ethernet MAC is enabled in which case ENET_MDC\n");
}

static void
pinmux__p3state (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[31:19]", "(reserved)");
	print_field(val, 31, 19);
	printf("\n");

	printf(ITEMFMT, "[18]", "GPO_18 / MC0A / LCDLP");
	field = print_field(val, 18, 18);
	if (field == 1)
		printf("MC0A if TIMCLK_CTRL1[6] == 1");
	else
		printf("GPO_18");
	printf(", unless LCD is enabled in which case LCDVD[0]\n");

	printf(ITEMFMT, "[17]", "(reserved)");
	print_field(val, 17, 17);
	printf("\n");

	printf(ITEMFMT, "[16]", "GPO_16 / MC0B / LCDENAB / LCDM");
	field = print_field(val, 16, 16);
	if (field == 1)
		printf("MC0B if TIMCLK_CTRL[6] == 1");
	else
		printf("GPO_16");
	printf(", unless LCD is enabled in which case LCDENAB / LCDM\n");

	printf(ITEMFMT, "[15]", "GPO_15 / MC1A / LCDFP");
	field = print_field(val, 15, 15);
	if (field == 1)
		printf("MC1A if TIMCLK_CTRL1[6] == 1");
	else
		printf("GPO_15");
	printf(", unless LCD is enabled in which case LCDFP\n");

	printf(ITEMFMT, "[14]", "(reserved)");
	print_field(val, 14, 14);
	printf("\n");

	printf(ITEMFMT, "[13]", "GPO_13 / MC1B / LCDDCLK");
	field = print_field(val, 13, 13);
	if (field == 1)
		printf("MC1B if TIMCLK_CTRL1[6] == 1");
	else
		printf("GPO_13");
	printf(", unless LCD is enabled in which case LCDDCLK\n");

	printf(ITEMFMT, "[12]", "GPO_12 / MC2A / LCDLE");
	field = print_field(val, 12, 12);
	if (field == 1)
		printf("KC2A if TIMCLK_CTRL1[6] == 1");
	else
		printf("GPO_12");
	printf(", unless LCD is enabled in which case LCDLE\n");

	printf(ITEMFMT, "[11]", "(reserved)");
	print_field(val, 11, 11);
	printf("\n");

	printf(ITEMFMT, "[10]", "GPO_10 / MC2B / LCDPWR");
	field = print_field(val, 10, 10);
	if (field == 1)
		printf("MC2B if TIMCLK_CTRL1[6] == 1");
	else
		printf("GPO_10");
	printf(", unless LCD is enabled in which case LCDPWR\n");

	printf(ITEMFMT, "[9:3]", "(reserved)");
	print_field(val, 9, 3);
	printf("\n");

	printf(ITEMFMT, "[2]", "GPO_2 / MAT1.0 | LCDVD[0]");
	field = print_field(val, 2, 2);
	if (field == 1)
		printf("MAT1.0");
	else
		printf("GPO_2");
	printf(", unless LCD is enabled in which case LCDVD[0]\n");

	printf(ITEMFMT, "[1:0]", "(reserved)");
	print_field(val, 1, 0);
}

RegisterDescription_t pinmux[] = {
	{0x40004090, 0, "MAC_CLK_CTRL", "Ethernet MAC clock control", accessRW, NULL},
	{0x40004054, 0, "LCD_CFG", "LCD configuration", accessRW, NULL},
	{0x40054000, 0, "UART_CTRL", "UART control", accessRW, pinmux__uartctrl},
	{0x40004080, 0, "MS_CTRL", "Memory card control", accessRW, pinmux__ms},
	{0x40028100, 0, "P_MUX_SET", "Peripheral multiplexer set", accessWrite, NULL},
	{0x40028104, 0, "P_MUX_CLR", "Peripheral multiplexer clear", accessWrite, NULL},
	{0x40028108, 0, "P_MUX_STATE", "Peripheral multiplexer state", accessRead, pinmux__state},
	{0x40028120, 0, "P0_MUX_SET", "Port 0 mux set", accessWrite, NULL},
	{0x40028124, 0, "P0_MUX_CLR", "Port 0 mux clear", accessWrite, NULL},
	{0x40028128, 0, "P0_MUX_STATE", "Port 0 mux state", accessRead, pinmux__p0state},
	{0x40028130, 0, "P1_MUX_SET", "Port 1 mux set", accessWrite, NULL},
	{0x40028134, 0, "P1_MUX_CLR", "Port 1 mux clear", accessWrite, NULL},
	{0x40028138, 0, "P1_MUX_STATE", "Port 1 mux state", accessRead, pinmux__p1state},
	{0x40028028, 0, "P2_MUX_SET", "Port 2 mux set", accessWrite, NULL},
	{0x4002802c, 0, "P2_MUX_CLR", "Port 2 mux clear", accessWrite, NULL},
	{0x40028030, 0, "P2_MUX_STATE", "Port 2 mux state", accessRead, pinmux__p2state},
	{0x40028110, 0, "P3_MUX_SET", "Port 3 mux set", accessWrite, NULL},
	{0x40028114, 0, "P3_MUX_CLR", "Port 3 mux clear", accessWrite, NULL},
	{0x40028118, 0, "P3_MUX_STATE", "Port 3 mux state", accessRead, pinmux__p3state},
};
size_t pinmuxSZ = sizeof(pinmux)/sizeof(pinmux[0]);
