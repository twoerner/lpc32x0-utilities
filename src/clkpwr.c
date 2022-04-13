// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registers.h"

static void
clkpower__pwr_ctrl (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[10]", "force HCLK and ARMCLK to run from PERIPH_CLK in order to save power");
	field = print_field(val, 10, 10);
	if (field == 0)
		printf("normal mode (ARM and AHB not running on PERIPH_CLK)");
	else
		printf("ARM and AHB matrix (AHB) runs with PERIPH_CLK frequency");
	printf("\n");

	printf(ITEMFMT, "[9]", "EMCSREFREQ value");
	field = print_field(val, 9, 9);
	if (field == 0)
		printf("no SDRAM self refresh");
	else
		printf("SDRAM self refresh request");
	printf("\n");

	printf(ITEMFMT, "[8]", "update EMCSREFREQ");
	field = print_field(val, 8, 8);
	if (field == 0)
		printf("no action");
	else
		printf("update EMCSREFREQ according to PWR_CTRL[9]");
	printf("\n");

	printf(ITEMFMT, "[7]", "SDRAM auto-exit self-refresh enable");
	field = print_field(val, 7, 7);
	if (field == 0)
		printf("disable auto exit self refresh");
	else
		printf("enable auto exit self refresh");
	printf("\n");

	printf(ITEMFMT, "[5]", "HIGHCORE pin level");
	field = print_field(val, 5, 5);
	if (field == 0)
		printf("HIGHCORE will drive low");
	else
		printf("HIGHCORE will drive high");
	printf("\n");

	printf(ITEMFMT, "[4]", "SYSCLKEN pin level");
	field = print_field(val, 4, 4);
	if (field == 0)
		printf("SYSCLKEN will drive low");
	else
		printf("SYSCLKEN will drive high");
	printf("\n");

	printf(ITEMFMT, "[3]", "SYSCLKEN pin drives high");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("SYSCLKEN will drive high when not in STOP mode");
	else
		printf("SYSCLKEN will always drive the level specified by bit 4");
	printf("\n");

	printf(ITEMFMT, "[2]", "RUN mode control");
	field = print_field(val, 2, 2);
	if (field == 0)
		printf("direct RUN mode");
	else
		printf("normal RUN mode (ARM and HCLK is sourced from PLL output)");
	printf("\n");

	printf(ITEMFMT, "[1]", "core voltage supply level signalling control");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("HIGHCORE pin will drive high during STOP mode and drive low in all other modes");
	else
		printf("HIGHCORE pin is always driving the level as specified in bit 5");
	printf("\n");

	printf(ITEMFMT, "[0]", "STOP mode control register");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("restore value to 0 if STOP was never entered");
	else
		printf("instruct hardware to enter STOP mode");
}

static void
clkpower__osc_ctrl (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[8:2]", "load capacitance");
	field = print_field(val, 8, 2);
	if (field == 0x40)
		printf("default setting of 6.4pF added");
	else
		printf("add %d x 0.1 pF load capacitance to SYSX_IN and SYSX_OUT", field);
	printf("\n");

	printf(ITEMFMT, "[1]", "main oscillator test mode");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("normal mode (either oscillation mode or power down mode)");
	else
		printf("test mode");
	printf("\n");

	printf(ITEMFMT, "[0]", "main oscillator enable");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("main oscillator is enabled");
	else
		printf("main oscillator is diabled and in power down mode");
}

static void
clkpower__sysclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[11:2]", "bad phase clock switch");
	field = print_field(val, 11, 2);
	printf("0x%08x\n", field);

	printf(ITEMFMT, "[1]", "main oscillator switch");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("switch to main oscillator");
	else
		printf("switch to 13' MHz clock source (PLL397 output)");
	printf("\n");

	printf(ITEMFMT, "[0]", "SYSCLK MUX status");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("main oscillator selected as the clock source");
	else
		printf("13' MHz PLL397 output selected as the clock source");
}

static void
clkpower__pll397_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[10]", "PLL MSLOCK status");
	field = print_field(val, 10, 10);
	if (field == 0)
		printf("PLL is not locked");
	else
		printf("PLL is locked and stable");
	printf("\n");

	printf(ITEMFMT, "[9]", "PLL397 bypass control");
	field = print_field(val, 9, 9);
	if (field == 0)
		printf("bypass - PLL is bypassed and output clock is the input clock");
	else
		printf("no bypass");
	printf("\n");

	printf(ITEMFMT, "[8:6]", "PLL397 charge pump bias control");
	field = print_field(val, 8, 6);
	switch (field) {
		case 0:
			printf("Normal bias setting");
			break;
		case 1:
			printf("-12.5%% of resistance");
			break;
		case 2:
			printf("-25%% of resistance");
			break;
		case 3:
			printf("-37.5%% of resistance");
			break;
		case 4:
			printf("+12.5%% of resistance");
			break;
		case 5:
			printf("+25%% of resistance");
			break;
		case 6:
			printf("+37.5%% of resistance");
			break;
		case 7:
			printf("+50%% of resistance");
			break;
		default:
			printf("??");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[1]", "PLL397 operational control");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("PLL is running");
	else
		printf("PLL397 is stopped and is in low power mode");
	printf("\n");

	printf(ITEMFMT, "[0]", "PLL LOCK status");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("PLL is not locked");
	else
		printf("PLL is locked and stable");
	printf("\n");

}

static void
clkpower__hclkpll_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[16]", "PLL power down");
	field = print_field(val, 16, 16);
	if (field == 0)
		printf("PLL is in power down mode");
	else
		printf("PLL is in operating mode");
	printf("\n");

	printf(ITEMFMT, "[15]", "bypass control");
	field = print_field(val, 15, 15);
	if (field == 0)
		printf("CCO clock is sent to the post divider");
	else
		printf("PLL input clock bypasses the CCO and is sent directly to the post divider");
	printf("\n");

	printf(ITEMFMT, "[14]", "direct output control");
	field = print_field(val, 14, 14);
	if (field == 0)
		printf("the output of the post-divider is used as output of the PLL");
	else
		printf("CCO clock is the direct output of the PLL, bypassing the post divider");
	printf("\n");

	printf(ITEMFMT, "[13]", "feedback divider path control");
	field = print_field(val, 13, 13);
	if (field == 0)
		printf("feedback divider clocked by the CCO clock");
	else
		printf("feedback divider clocked by the FCLKOUT");
	printf("\n");

	printf(ITEMFMT, "[12:11]", "PLL post-divider P setting");
	field = print_field(val, 12, 11);
	switch (field) {
		case 0:
			printf("divide by 2");
			break;
		case 1:
			printf("divide by 4");
			break;
		case 2:
			printf("divide by 8");
			break;
		case 3:
			printf("divide by 16");
			break;
		default:
			printf("??");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[10:9]", "PLL pre-divider N setting");
	field = print_field(val, 10, 9);
	printf("%d\n", field+1);

	printf(ITEMFMT, "[8:1]", "PLL feedback divider M setting");
	field = print_field(val, 8, 1);
	printf("%d\n", field+1);

	printf(ITEMFMT, "[0]", "PLL LOCK status");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("PLL is not locked");
	else
		printf("PLL is locked and stable");
	printf("\n");
}

static void
clkpower__hclkdiv_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[8:7]", "DDRAM_CLK control");
	field = print_field(val, 8, 7);
	switch (field) {
		case 0:
			printf("DDRAM clock stopped");
			break;
		case 1:
			printf("DDRAM nominal speed");
			break;
		case 2:
			printf("DDRAM half speed");
			break;
		case 3:
			printf("not used");
			break;
		default:
			printf("??");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[6:2]", "PERIPH_CLK divider control");
	field = print_field(val, 6, 2);
	if (field == 0)
		printf("PERIPH_CLK is ARM PLL clock in RUN mode");
	else
		printf("PERIPH_CLK is ARM PLL clock divided by %d in RUN mode", field+1);
	printf("\n");

	printf(ITEMFMT, "[1:0]", "HCLK divider control");
	field = print_field(val, 1, 0);
	switch (field) {
		case 0:
			printf("HCLK is ARM PLL clock in RUN mode");
			break;
		case 1:
			printf("HCLK is ARM PLL clock divided by 2 in RUN mode");
			break;
		case 2:
			printf("HCLK is ARM PLL clock divided by 4 in RUN mode");
			break;
		case 3:
			printf("not used");
			break;
		default:
			printf("??");
			break;
	}
	printf("\n");
}

static void
clkpower__test_clk (uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[6:5]", "the selected clock is output on GPO_0 TEST_CLK1 pin if bit 4 of this register is 1");
	field = print_field(val, 6, 5);
	switch (field) {
		case 0:
			printf("PERIPH_CLK, this clock stops in STOP mode");
			break;
		case 1:
			printf("RTC clock, un-synchronized version");
			break;
		case 2:
			printf("main oscillator clock");
			break;
		case 3:
			printf("not used");
			break;
		default:
			printf("??");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[4]", "GPO_0 TST_CLK1 output");
	field = print_field(val, 4, 4);
	if (field == 0)
		printf("output is connected to the GPIO block");
	else
		printf("output is the clock selected by register bits [6:5]");
	printf("\n");

	printf(ITEMFMT, "[3:1]", "the selected clock is output on TST_CLK2 pin if bit 0 of this register contain 1");
	field = print_field(val, 3, 1);
	switch (field) {
		case 0:
			printf("HCLK");
			break;
		case 1:
			printf("PERIPH_CLK");
			break;
		case 2:
			printf("USB clock");
			break;
		case 3:
		case 4:
			printf("reserved");
			break;
		case 5:
			printf("main oscillator clock");
			break;
		case 6:
			printf("reserved");
			break;
		case 7:
			printf("PLL397 output clock (13.008896 MHz)");
			break;
		default:
			printf("??");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[0]", "TST_CLK2");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("TST_CLK2 is turned off");
	else
		printf("TST_CLK2 outputs the clock selected by register bits [3:1]");
	printf("\n");
}

static void
clkpower__autoclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[6]", "USB slave HCLK clocking");
	field = print_field(val, 6, 6);
	if (field == 0)
		printf("autoclock enabled on USB slave HCLK");
	else
		printf("always clocked (autoclock disabled)");
	printf("\n");

	printf(ITEMFMT, "[1]", "IRAM clocking");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("autoclock enabled on IRAM");
	else
		printf("always clocked");
	printf("\n");

	printf(ITEMFMT, "[0]", "IROM clocking");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("autoclock enabled on IROM");
	else
		printf("always clocked");
	printf("\n");
}

static void
clkpower__start_er_pin(uint32_t val)
{
	printf(ITEMFMT, "", "The following devices can or cannot start up the chip from STOP mode");
	print_field(val, 31, 31);
	printf("[31] UART7_RX\n");
	print_field(val, 30, 30);
	printf("[30] UART7_HCTS\n");
	print_field(val, 29, 29);
	printf("[29] (reserved)\n");
	print_field(val, 28, 28);
	printf("[28] UART6_IRRX\n");
	print_field(val, 27, 27);
	printf("[27] (reserved)\n");
	print_field(val, 26, 26);
	printf("[26] UART5_RX/USB_DAT_VP\n");
	print_field(val, 25, 25);
	printf("[25] GPI_28\n");
	print_field(val, 24, 24);
	printf("[24] UART3_RX\n");
	print_field(val, 23, 23);
	printf("[23] UART2_HCTS\n");
	print_field(val, 22, 22);
	printf("[22] UART2_RX\n");
	print_field(val, 21, 21);
	printf("[21] UART1_RX\n");
	print_field(val, 20, 19);
	printf("[20:19] (reserved)\n");
	print_field(val, 18, 18);
	printf("[18] SDIO_INT_N\n");
	print_field(val, 17, 17);
	printf("[17] MSDIO_START (logical OR of MS_DIO[3:0])\n");
	print_field(val,16, 16);
	printf("[16] GPI_6/HSTIM_CAP\n");
	print_field(val, 15, 15);
	printf("[15] GPI_5\n");
	print_field(val, 14, 14);
	printf("[14] GPI_4\n");
	print_field(val, 13, 13);
	printf("[13] GPI_3\n");
	print_field(val, 12, 12);
	printf("[12] GPI_2\n");
	print_field(val, 11, 11);
	printf("[11] GPI_1/SERVICE_N\n");
	print_field(val, 10, 10);
	printf("[10] GPI_0/I2S1RX_SDA\n");
	print_field(val, 9, 9);
	printf("[9] SYSCLKEN\n");
	print_field(val, 8, 8);
	printf("[8] SPI1_DATIN\n");
	print_field(val, 7, 7);
	printf("[7] GPI_7\n");
	print_field(val, 6, 6);
	printf("[6] SPI2_DATIN\n");
	print_field(val, 5, 5);
	printf("[5] GPI_19/UART4_RX\n");
	print_field(val, 4, 4);
	printf("[4] GPI_9\n");
	print_field(val, 3, 3);
	printf("[3] GPI_8\n");
	print_field(val, 2, 0);
	printf("[2:0] (reserved)\n");
	printf("\n");
}

static void
clkpower__start_er_int(uint32_t val)
{
	printf(ITEMFMT, "", "The following internal sources can start up the chip from STOP mode");
	print_field(val, 31, 31);
	printf("[31] TS_INT\n");
	print_field(val, 30, 30);
	printf("[30] TS_P\n");
	print_field(val, 29, 29);
	printf("[29] TS_AUX\n");
	print_field(val, 28, 27);
	printf("[28:27] (reserved)\n");
	print_field(val, 26, 26);
	printf("[26] USB_AHB_NEED_CLK\n");
	print_field(val, 25, 25);
	printf("[25] MSTIMER_INT\n");
	print_field(val, 24, 24);
	printf("[24] RTC_INT interrupt from RTC\n");
	print_field(val, 23, 23);
	printf("[23] USB_NEED_CLK\n");
	print_field(val, 22, 22);
	printf("[22] USB_INT\n");
	print_field(val, 21, 21);
	printf("[21] USB_I2C_INT\n");
	print_field(val, 20, 20);
	printf("[20] USB_OTG_TIMER_INT\n");
	print_field(val, 19, 19);
	printf("[19] USB_OTG_ATX_INT_N\n");
	print_field(val, 18, 17);
	printf("[18:17] (reserved)\n");
	print_field(val, 16, 16);
	printf("[16] KEY_IRQ\n");
	print_field(val, 15, 8);
	printf("[15:8] (reserved)\n");
	print_field(val, 7, 7);
	printf("[7] ethernet MAC start request\n");
	print_field(val, 6, 6);
	printf("[6] port 0/port 1 start request (OR's all or port 0 and port 1, see P0_INTR_ER)\n");
	print_field(val, 5, 5);
	printf("[5] GPIO_5\n");
	print_field(val, 4, 4);
	printf("[4] GPIO_4\n");
	print_field(val, 3, 3);
	printf("[3] GPIO_3\n");
	print_field(val, 2, 2);
	printf("[2] GPIO_2\n");
	print_field(val, 1, 1);
	printf("[1] GPIO_1\n");
	print_field(val, 0, 0);
	printf("[0] GPIO_0\n");
	printf("\n");
}

static void
clkpower__p0_intr_er(uint32_t val)
{
	print_field(val, 31, 11);
	printf("[31:11] P1.[23:3]\n");
	print_field(val, 10, 10);
	printf("[10] P1.2\n");
	print_field(val, 9, 9);
	printf("[9] P1.1\n");
	print_field(val, 8, 8);
	printf("[8] P1.0\n");
	print_field(val, 7, 7);
	printf("[7] P0.7\n");
	print_field(val, 6, 6);
	printf("[6] P0.6\n");
	print_field(val, 5, 5);
	printf("[5] P0.5\n");
	print_field(val, 4, 4);
	printf("[4] P0.4\n");
	print_field(val, 3, 3);
	printf("[3] P0.3\n");
	print_field(val, 2, 2);
	printf("[2] P0.2\n");
	print_field(val, 1, 1);
	printf("[1] P0.1\n");
	print_field(val, 0, 0);
	printf("[0] P0.0\n");
	printf("\n");
}

static void
clkpower__start_sr_pin(uint32_t val)
{
	print_field(val, 31, 3);
	printf("[31:3] current state of device pin startup sources after masking with START_ER_PIN\n");
	print_field(val, 2, 0);
	printf("[2:0] (reserved)\n");
	printf("\n");
}

static void
clkpower__start_sr_int(uint32_t val)
{
	print_field(val, 31, 3);
	printf("[31:3] current state of possible internal startup sources, after masking with START_ER_INT\n");
	print_field(val, 2, 0);
	printf("[2:0] (reserved)\n");
	printf("\n");
}

static void
clkpower__start_rsr_pin(uint32_t val)
{
	print_field(val, 31, 3);
	printf("[31:3] current state of possible device pin startup sources, prior to masking\n");
	print_field(val, 2, 0);
	printf("[2:0] (reserved)\n");
	printf("\n");
}

static void
clkpower__start_rsr_int(uint32_t val)
{
	print_field(val, 31, 3);
	printf("[31:3] current state of possible internal startup sources, prior to masking\n");
	print_field(val, 2, 0);
	printf("[2:0] (reserved)\n");
	printf("\n");
}

static void
clkpower__start_apr_pin(uint32_t val)
{
	print_field(val, 31, 3);
	printf("[31:3] allows selecting the polarity that device pin start signal sources use as a start condition\n");
	print_field(val, 2, 0);
	printf("[2:0] (reserved)\n");
	printf("\n");
}

static void
clkpower__start_apr_int(uint32_t val)
{
	print_field(val, 31, 3);
	printf("[31:3] allows selecting the polarity that internal start signal sources use as a start condition\n");
	print_field(val, 2, 0);
	printf("[2:0] (reserved)\n");
	printf("\n");
}

static void
clkpower__usb_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[24]", "USB slave HCLK control");
	field = print_field(val, 24, 24);
	if (field == 0)
		printf("disabled");
	else
		printf("enabled");
	printf("\n");

	printf(ITEMFMT, "[23]", "usb_i2c enable");
	field = print_field(val, 23, 23);
	if (field == 0)
		printf("ip_3506_otg_tx_en_n is fed to OE_TP_N pad");
	else
		printf("'0' is fed to OE_TP_N pad");
	printf("\n");

	printf(ITEMFMT, "[22]", "usb_dev_need_clk_en");
	field = print_field(val, 22, 22);
	if (field == 0)
		printf("usb_dev_need_clk is not let into the clock switch");
	else
		printf("usb_dev_need_clk is let into clock switch");
	printf("\n");

	printf(ITEMFMT, "[21]", "usb_host_need_clk_en");
	field = print_field(val, 21, 21);
	if (field == 0)
		printf("usb_host_need_clk_en is not let into the clock switch");
	else
		printf("usb_host_need_clk_en is let into clock switch");
	printf("\n");

	printf(ITEMFMT, "[20:19]", "pad control for USB_DAT_VP and USB_SE0_VM pads");
	field = print_field(val, 20, 19);
	switch (field) {
		case 0:
			printf("pull-up added to pad");
			break;
		case 1:
			printf("bus keeper, retains the last driven value");
			break;
		case 2:
			printf("no added function");
			break;
		case 3:
			printf("pull-down added to pad");
			break;
		default:
			printf("??");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[18]", "USB_Clken2 clock control");
	field = print_field(val, 18, 18);
	if (field == 0)
		printf("stop clock going into USB clock");
	else
		printf("enable clock going into USB clock");
	printf("\n");

	printf(ITEMFMT, "[17]", "USB_Clken1 clock control");
	field = print_field(val, 17, 17);
	if (field == 0)
		printf("stop clock going into the USB PLL");
	else
		printf("enable clock going into the USB PLL");
	printf("\n");

	printf(ITEMFMT, "[16]", "PLL power down");
	field = print_field(val, 16, 16);
	if (field == 0)
		printf("PLL is in power down mode");
	else
		printf("PLL is in operating mode");
	printf("\n");

	printf(ITEMFMT, "[15]", "bypass control");
	field = print_field(val, 15, 15);
	if (field == 0)
		printf("CCO clock is sent to the post divider");
	else
		printf("PLL input clock bypasses the CCO and is sent directly to the post divider");
	printf("\n");

	printf(ITEMFMT, "[14]", "direct output control");
	field = print_field(val, 14, 14);
	if (field == 0)
		printf("the output of the post-divider is used as output of the PLL");
	else
		printf("CCO clock is the direct output of the PLL, bypassing the post divider");
	printf("\n");

	printf(ITEMFMT, "[13]", "feedback divider path control");
	field = print_field(val, 13, 13);
	if (field == 0)
		printf("feedback divider clocked by CCO clock");
	else
		printf("feedback divider clocked by post FCLKOUT");
	printf("\n");

	printf(ITEMFMT, "[12:11]", "PLL post-divider (P) setting");
	field = print_field(val, 12, 11);
	switch (field) {
		case 0:
			printf("divide by 2 (P=1)");
			break;
		case 1:
			printf("divide by 4 (P=2)");
			break;
		case 2:
			printf("divide by 8 (P=4)");
			break;
		case 3:
			printf("divide by 16 (P=8)");
			break;
		default:
			printf("??");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[10:9]", "PLL pre-divider (N) setting");
	field = print_field(val, 10, 9);
	printf("%d\n", field + 1);

	printf(ITEMFMT, "[8:1]", "PLL feedback divider (M) setting");
	field = print_field(val, 8, 1);
	printf("%d\n", field + 1);

	printf(ITEMFMT, "[0]", "PLL lock status");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("PLL not locked");
	else
		printf("PLL locked, output clock stable");
	printf("\n");
}

static void
clkpower__usb_div(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3:0]", "USB_RATE: controls USB pre-clock divider");
	field = print_field(val, 3, 0);
	printf("%d\n", field + 1);
	printf("\n");
}

static void
clkpower__ms_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[10]", "disable SDcard pins");
	field = print_field(val, 10, 10);
	if (field == 0)
		printf("enable SDcard interface pins");
	else
		printf("disable SDcard interface pins and enable peripheral MUX registers");
	printf("\n");

	printf(ITEMFMT, "[9]", "enables clock and pull-ups to MSSDIO pins");
	field = print_field(val, 9, 9);
	if (field == 0)
		printf("MSSDIO pull-up and clock disable");
	else
		printf("MSSDIO pull-up and clock enable");
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
		printf("MSSDIO1 pad has no pull-up");
	printf("\n");

	printf(ITEMFMT, "[6]", "MSSDIO0/MSBS pad control");
	field = print_field(val, 6, 6);
	if (field == 0)
		printf("MSSDIO0 pad has pull-up enabled");
	else
		printf("MSSDIO0 pad has no pull-up");
	printf("\n");

	printf(ITEMFMT, "[5]", "SDcard clock control");
	field = print_field(val, 5, 5);
	if (field == 0)
		printf("clocks disabled");
	else
		printf("clocks enabled");
	printf("\n");

	printf(ITEMFMT, "[3:0]", "divider ratio when generating clock from ARM PLL output clock");
	field = print_field(val, 3, 0);
	switch (field) {
		case 0:
			printf("MSSDCLK stopped, divider in low power mode");
			break;
		default:
			printf("MSSDCLK equals ARM PLL output clock divided by %d", field);
			break;
	}
	printf("\n");
}

static void
clkpower__dmaclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[0]", "DMA clock");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("all clocks to DMA stopped, no access to DMA registers allowed");
	else
		printf("all clocks to DMA enabled");
	printf("\n");
}

static void
clkpower__flashclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[5]", "which NAND flash controller interrupt is connected to the interrupt controller");
	field = print_field(val, 5, 5);
	if (field == 0)
		printf("enable the SLC NAND flash controller interrupt");
	else
		printf("enable the MLC NAND flash controller interrupt");
	printf("\n");

	printf(ITEMFMT, "[4]", "enable NAND_DMA_REQ on NAND_RnB (MLC only)");
	field = print_field(val, 4, 4);
	if (field == 0)
		printf("disable");
	else
		printf("enable");
	printf("\n");

	printf(ITEMFMT, "[3]", "enable NAND_DMA_REQ on NAND_INT (MLC only)");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("disable");
	else
		printf("enable");
	printf("\n");

	printf(ITEMFMT, "[2]", "SLC/MLC select");
	field = print_field(val, 2, 2);
	if (field == 0)
		printf("MLC");
	else
		printf("SLC");
	printf("\n");

	printf(ITEMFMT, "[1]", "MLC NAND flash clock enable");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("disable clocks to the block, including the AHB interface");
	else
		printf("enable clock");
	printf("\n");

	printf(ITEMFMT, "[0]", "SLC NAND flash clock enable control");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("disable clocks to the block, including to the AHB interface");
	else
		printf("enable clock");
	printf("\n");
}

static void
clkpower__macclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[4:3]", "ethernet MAC hardware interface control");
	field = print_field(val, 4, 3);
	switch (field) {
		case 0:
			printf("do not connect ethernet MAC to port pins");
			break;
		case 1:
			printf("connect ethernet MAC to port pins in MII mode");
			break;
		case 2:
			printf("do not connect ethernet MAC to port pins");
			break;
		case 3:
			printf("connect ethernet MAC to port pins in RMII mode");
			break;
		default:
			printf("??");
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[2]", "MASTER_CLK");
	field = print_field(val, 2, 2); 
	if (field == 0)
		printf("disabled");
	else
		printf("enabled");
	printf("\n");

	printf(ITEMFMT, "[1]", "REG_CLK");
	field = print_field(val, 1, 1); 
	if (field == 0)
		printf("disabled");
	else
		printf("enabled");
	printf("\n");

	printf(ITEMFMT, "[0]", "SLAVE_CLK");
	field = print_field(val, 0, 0); 
	if (field == 0)
		printf("disabled");
	else
		printf("enabled");
}

static void
clkpower__lcdclk_ctrl(uint32_t val)
{
	uint32_t field;
	uint32_t type;

	printf(ITEMFMT, "[8]", "DISPLAY_TYPE");
	type = print_field(val, 8, 8);
	printf("%s\n", type == 1? "STN" : "TFT");

	printf(ITEMFMT, "[7:6]", "MODE_SELECT");
	field = print_field(val, 7, 6);
	if (type == 0) { /* TFT */
		switch (field) {
			case 0:
				printf("single panel, 12-bit (4:4:4), 18 pins");
				break;
			case 1:
				printf("single panel, 16-bit (5:6:5), 22 pins");
				break;
			case 2:
				printf("single panel, 16-bit (1:5:5:5), 24 pins");
				break;
			case 3:
				printf("single panel, 24-bit, 30 pins");
				break;
			default:
				printf("?");
				break;
		}
	}
	else { /* STN */
		switch (field) {
			case 0:
				printf("single panel, 4-bit mono, 10 pins");
				break;
			case 1:
				printf("single panel, 8-bit mono/colour, 14 pins");
				break;
			case 2:
				printf("dual panel, 4-bit mono, 14 pins");
				break;
			case 3:
				printf("dual panel, 8-bit mono/colour, 22 pins");
				break;
			default:
				printf("?");
				break;
		}
	}
	printf("\n");

	printf(ITEMFMT, "[5]", "HCLK_ENABLE");
	field = print_field(val, 5, 5);
	printf("%sable HCLK signal to LCD controller\n", field == 0? "dis" : "en");

	printf(ITEMFMT, "[4:0]", "CLKDIV");
	printf("\t\t\tLCD panel clock prescaler: 0x%x\n", get_field(val, 4, 0));
}

static void
clkpower__i2s_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[6]", "I2S1 CLK_TX_MODE select");
	field = print_field(val, 6, 6);
	if (field == 0)
		printf("TX_CLK drives I2S TX timing");
	else
		printf("RX_CLK drives I2S TX timing");
	printf("\n");

	printf(ITEMFMT, "[5]", "I2S1_RX_MODE select");
	field = print_field(val, 5, 5);
	if (field == 0)
		printf("RX_CLK drives I2S RX timing");
	else
		printf("TX_CLK drives I2S RX timing");
	printf("\n");

	printf(ITEMFMT, "[4]", "I2S1 DMA1 connection control");
	field = print_field(val, 4, 4);
	if (field == 0)
		printf("UART7 RX is connected to DMA (I2S1 DMA 1 is not conntected to DMA");
	else
		printf("I2S1 DMA 1 in connected to DMA (UART7 RX is not connected to DMA)");
	printf("\n");

	printf(ITEMFMT, "[3]", "I2S0_CLK_TX_MODE select");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("TX_CLK drives I2S TX timing");
	else
		printf("RX_CLK drives I2S TX timing");
	printf("\n");

	printf(ITEMFMT, "[2]", "I2S0_CLK_RX_MODE select");
	field = print_field(val, 2, 2);
	if (field == 0)
		printf("RX_CLK drives I2S RX timing");
	else
		printf("TX_CLK drives I2S RX timing");
	printf("\n");

	printf(ITEMFMT, "[1]", "I2S1_CLK enable");
	field = print_field(val, 1, 1);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[1]", "I2S0_CLK enable");
	field = print_field(val, 0, 0);
	printf("%s\n", (field == 0)? "disable" : "enable");
}

static void
clkpower__ssp_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[5]", "SSP1 RX DMA connection control");
	field = print_field(val, 5, 5);
	if (field == 0)
		printf("SSP1 RX is not conntected to DMA and SPI2 is connected to DMA");
	else
		printf("SSP1 RX is connected to DMA and SPI2 is not connected to DMA");
	printf("\n");

	printf(ITEMFMT, "[4]", "SSP1 TX DMA connection control");
	field = print_field(val, 4, 4);
	if (field == 0)
		printf("SSP1 TX is not connected to DMA and SPI1 is connected to DMA");
	else
		printf("SSP1 TX is connected to DMA and SPI1 is not connected to DMA");
	printf("\n");

	printf(ITEMFMT, "[3]", "SSP0 RX DMA connection control");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("SSP0 RX is not connected to DMA");
	else
		printf("SSP0 RX is connected to DMA");
	printf("\n");

	printf(ITEMFMT, "[2]", "SSP0 TX DMA connection control");
	field = print_field(val, 2, 2);
	if (field == 0)
		printf("SSP0 TX is not connected to DMA");
	else
		printf("SSP0 TX is connected to DMA");
	printf("\n");

	printf(ITEMFMT, "[1]", "SSP1 clock enable");
	field = print_field(val, 1, 1);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[0]", "SSP0 clock enable");
	field = print_field(val, 0, 0);
	printf("%s\n", (field == 0)? "disable" : "enable");
}

static void
clkpower__spi_ctrl(uint32_t val)
{
	int pin1 = 0;
	int pin5 = 0;
	uint32_t field;

	pin1 = val & 0x02;
	pin5 = val & 0x20;

	printf(ITEMFMT, "[7]", "SPI2_DATIO output level");
	field = print_field(val, 7, 7);
	if (pin5 == 0) {
		if (field == 0)
			printf("the pin drives low (pin 5 == 0)");
		else
			printf("the pin drives high (pin 5 == 0)");
	}
	else
		printf("this pin is driven by the SPI2 block");
	printf("\n");

	printf(ITEMFMT, "[6]", "SPI2_CLK output level");
	field = print_field(val, 6, 6);
	if (pin5 == 0) {
		if (field == 0)
			printf("the pin drives low (bit 5 == 0)");
		else
			printf("the pin drives high (bit 5 == 0)");
	}
	else
		printf("this pin is driven by the SPI2 block");
	printf("\n");

	printf(ITEMFMT, "[5]", "output pin control");
	field = print_field(val, 5, 5);
	if (field == 0)
		printf("SPI2_DATIO and SPI2_CLK outputs the level set by bit 6 and 7");
	else
		printf("SPI2_DATIO and SPI2_CLK are driven by the SPI2 block");
	printf("\n");

	printf(ITEMFMT, "[4]", "SPI2 clock enable control");
	field = print_field(val, 4, 4);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[3]", "SPI1_DATIO output level");
	field = print_field(val, 3, 3);
	if (pin1 == 0) {
		if (field == 0)
			printf("the pin drives low (bit 1 == 0)");
		else
			printf("the pin drives high (bit 1 == 0)");
	}
	else
		printf("this pin is driven by the SPI1 block");
	printf("\n");

	printf(ITEMFMT, "[2]", "SPI1_CLK output level");
	field = print_field(val, 2, 2);
	if (pin1 == 0) {
		if (field == 0)
			printf("the pin drives low (bit 1 == 0)");
		else
			printf("the pin drives high (bit 1 == 0)");
	}
	else
		printf("this pin is driven by the SPI1 block");
	printf("\n");

	printf(ITEMFMT, "[1]", "output pin control");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("SPI1_DATIO and SPI1_CLK outputs the level set by bit 2 and 3");
	else
		printf("SPI1_DATIO and SPI1_CLK are driven by the SPI1 block");
	printf("\n");

	printf(ITEMFMT, "[0]", "SPI1 clock control");
	field = print_field(val, 0, 0);
	printf("%s\n", (field == 0)? "disable" : "enable");
}

static void
clkpower__i2cclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[4]", "driver strength control for USB_I2C_SCL and USB_I2C_SDA");
	field = print_field(val, 4, 4);
	if (field == 0)
		printf("USB I2C pins operate in low drive mode");
	else
		printf("USB I2C pins operate in high drive mode");
	printf("\n");

	printf(ITEMFMT, "[3]", "I2C2_SCL and I2C2_SDA driver strength control");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("I2C2 pins operate in low drive mode");
	else
		printf("I2C2 pins operate in high drive mode");
	printf("\n");

	printf(ITEMFMT, "[2]", "I2C1_SCL and I2C1_SDA driver strength control");
	field = print_field(val, 2, 2);
	if (field == 0)
		printf("I2C1 pins operate in low drive mode");
	else
		printf("I2C1 pins operate in high drive mode");
	printf("\n");

	printf(ITEMFMT, "[1]", "software must set this bit before using the I2C2 block, it can be cleared if I2C2 block is not in use");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("I2C2 HCLK stopped, no I2C registers are accessible");
	else
		printf("I2C2 HCLK enabled");
	printf("\n");

	printf(ITEMFMT, "[0]", "software must set this bit before using the I2C1 block, it can be cleared if the I2C1 block is not in use");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("I2C1 HCLK stopped, no I2C registers are accessible");
	else
		printf("I2C1 HCLK enabled");
	printf("\n");
}

static void
clkpower__timclk_ctrl1(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[6]", "motor control clock enable");
	field = print_field(val, 6, 6);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[5]", "timer 3 clock enable control");
	field = print_field(val, 5, 5);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[4]", "timer 2 clock enable control");
	field = print_field(val, 4, 4);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[3]", "timer 1 clock enable control");
	field = print_field(val, 3, 3);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[2]", "timer 0 clock enable control");
	field = print_field(val, 2, 2);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[1]", "timer 5 clock enable control");
	field = print_field(val, 1, 1);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[0]", "timer 4 clock enable control");
	field = print_field(val, 0, 0);
	printf("%s\n", (field == 0)? "disable" : "enable");
}

static void
clkpower__timclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[1]", "HSTimer clock enable control");
	field = print_field(val, 1, 1);
	printf("%s\n", (field == 0)? "disable" : "enable");

	printf(ITEMFMT, "[0]", "Watchdog clock enable control");
	field = print_field(val, 0, 0);
	printf("%s\n", (field == 0)? "disable" : "enable");
}

static void
clkpower__adclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[0]", "analog to digital converter and touch screen");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("disable 32kHz clock to ADC block");
	else
		printf("enable clock");
	printf("\n");
}

static void
clkpower__adclk_ctrl1(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[8]", "ADC clock select");
	field = print_field(val, 8, 8);
	if (field == 0)
		printf("clock ADC and touch screen from RTC clock");
	else
		printf("clock ADC and touch screen from PERIPH_CLK clock");
	printf("\n");

	printf(ITEMFMT, "[7:0]", "controls the clock divider for ADC when peripheral clock (bit 8) is enabled");
	field = print_field(val, 7, 0);
	printf("%d\n", field + 1);
}

static void
clkpower__keyclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[0]", "keyboard scan peripheral");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("disable clock to keyboard block");
	else
		printf("enable clock");
	printf("\n");
}

static void
clkpower__pwmclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[11:8]", "PWM2_FREQ, controls the clock divider for PWM2");
	field = print_field(val, 11, 8);
	switch (field) {
		case 0:
			printf("PWM2_CLK = off");
			break;
		case 1:
			printf("PWM2_CLK = CLKin");
			break;
		default:
			printf("PWM2_CLK = CLKin / %d", field);
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[7:4]", "PWM1_FREQ, controls the clock divider for PWM1");
	field = print_field(val, 7, 4);
	switch (field) {
		case 0:
			printf("PWM1_CLK = off");
			break;
		case 1:
			printf("PWM1_CLK = CLKin");
			break;
		default:
			printf("PWM1_CLK = CLKin / %d", field);
			break;
	}
	printf("\n");

	printf(ITEMFMT, "[3]", "PWM2 clock source selection");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("32kHz RTC_CLK");
	else
		printf("PERIPH_CLK");
	printf("\n");

	printf(ITEMFMT, "[2]", "PWM2 block enable");
	field = print_field(val, 2, 2);
	if (field == 0)
		printf("disable clock to PWM2 block");
	else
		printf("enable clock to PWM2 block");
	printf("\n");

	printf(ITEMFMT, "[1]", "PWM1 clock source selection");
	field = print_field(val, 1, 1);
	if (field == 0)
		printf("32kHz RTC_CLK");
	else
		printf("PERIPH_CLK");
	printf("\n");

	printf(ITEMFMT, "[0]", "PWM1 block enable");
	field = print_field(val, 0, 0);
	if (field == 0)
		printf("disable clock to PWM1 block");
	else
		printf("enable clock to PWM1 block");
	printf("\n");
}

static void
clkpower__uartclk_ctrl(uint32_t val)
{
	uint32_t field;

	printf(ITEMFMT, "[3]", "uart 6");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("HCLK disabled and in low-power mode, no access to UART registers allowed");
	else
		printf("HCLK enabled");
	printf("\n");

	printf(ITEMFMT, "[2]", "uart 5");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("HCLK disabled and in low-power mode, no access to UART registers allowed");
	else
		printf("HCLK enabled");
	printf("\n");

	printf(ITEMFMT, "[1]", "uart 4");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("HCLK disabled and in low-power mode, no access to UART registers allowed");
	else
		printf("HCLK enabled");
	printf("\n");

	printf(ITEMFMT, "[0]", "uart 3");
	field = print_field(val, 3, 3);
	if (field == 0)
		printf("HCLK disabled and in low-power mode, no access to UART registers allowed");
	else
		printf("HCLK enabled");
	printf("\n");
}

static void
clkpower__pos0_iram_ctrl(uint32_t val)
{
	printf(ITEMFMT, "", "Power-Off Switch (POS) 0");
	print_field(val, 3, 3);
	printf("[3] control for 0x30000-0x3ffff\n");
	print_field(val, 2, 2);
	printf("[2] control for 0x20000-0x2ffff\n");
	print_field(val, 1, 1);
	printf("[1] control for 0x10000-0x1ffff\n");
	printf("\n");
}

static void
clkpower__pos1_iram_ctrl(uint32_t val)
{
	printf(ITEMFMT, "", "Power-Off Switch (POS) 1");
	print_field(val, 3, 3);
	printf("[3] control for 0x30000-0x3ffff\n");
	print_field(val, 2, 2);
	printf("[2] control for 0x20000-0x2ffff\n");
	print_field(val, 1, 1);
	printf("[1] control for 0x10000-0x1ffff\n");
	printf("\n");
}

RegisterDescription_t clkpower[] = {
	{0x40004044, 0x00000012, "PWR_CTRL", "AHB/ARM power control register", accessRW, clkpower__pwr_ctrl},
	{0x4000404c, 0x00000100, "OSC_CTRL", "Main oscillator control register", accessRW, clkpower__osc_ctrl},
	{0x40004050, 0x00000b48, "SYSCLK_CTRL", "SYSCLK control register", accessRW, clkpower__sysclk_ctrl},
	{0x40004048, 0,          "PLL397_CTRL", "PLL397 PLL control register", accessRW, clkpower__pll397_ctrl},
	{0x40004058, 0,          "HCLKPLL_CTRL", "ARM and HCLK PLL control register", accessRW, clkpower__hclkpll_ctrl},
	{0x40004040, 0,          "HCLKDIV_CTRL", "HCLK divider settings", accessRW, clkpower__hclkdiv_ctrl},
	{0x400040a4, 0,          "TEST_CLK", "Clock testing control", accessRW, clkpower__test_clk},
	{0x400040ec, 0,          "AUTOCLK_CTRL", "Auto clock control register", accessRW, clkpower__autoclk_ctrl},
	{0x40004030, 0,          "START_ER_PIN", "Start Enable register - pin sources", accessRW, clkpower__start_er_pin},
	{0x40004020, 0,          "START_ER_INT", "Start Enable register - internal sources", accessRW, clkpower__start_er_int},
	{0x40004018, 0,          "P0_INTR_ER", "Start and Interrupt Enable register; p0 & p1 sources", accessRW, clkpower__p0_intr_er},
	{0x40004038, 0,          "START_SR_PIN", "Start status register - pin sources", accessRead, clkpower__start_sr_pin},
	{0x40004028, 0,          "START_SR_INT", "Start status register - internal sources", accessRead, clkpower__start_sr_int},
	{0x40004034, 0,          "START_RSR_PIN", "Start Raw status register - pin sources", accessRW, clkpower__start_rsr_pin},
	{0x40004024, 0,          "START_RSR_INT", "Start Raw status register - internal sources", accessRW, clkpower__start_rsr_int},
	{0x4000403c, 0,          "START_APR_PIN", "Start activation polarity register - pin sources", accessRW, clkpower__start_apr_pin},
	{0x4000402c, 0,          "START_APR_INT", "Start activation polarity register - internal sources", accessRW, clkpower__start_apr_int},
	{0x40004064, 0x00080000, "USB_CTRL", "USB PLL and pad control register", accessRW, clkpower__usb_ctrl},
	{0x4000401c, 0x0000000c, "USBDIV_CTRL", "USB PLL pre-divier settings", accessRW, clkpower__usb_div},
	{0x40004080, 0,          "MS_CTRL", "SD Card interface clock and pad control", accessRW, clkpower__ms_ctrl},
	{0x400040e8, 0x00000001, "DMACLK_CTRL", "DMA clock control register", accessRW, clkpower__dmaclk_ctrl},
	{0x400040c8, 0x00000003, "FLASHCLK_CNTRL", "Flash clock control", accessRW, clkpower__flashclk_ctrl},
	{0x40004090, 0,          "MACCLK_CTRL", "Ethernet MAC clock control", accessRW, clkpower__macclk_ctrl},
	{0x40004054, 0,          "LCDCLK_CTRL", "LCD clock control", accessRW, clkpower__lcdclk_ctrl},
	{0x4000407c, 0,          "I2S_CTRL", "I2S control register", accessRW, clkpower__i2s_ctrl},
	{0x40004078, 0,          "SSP_CTRL", "SSP0 and SSP1 clock control", accessRW, clkpower__ssp_ctrl},
	{0x400040c4, 0,          "SPI_CTRL", "SPI1 and SPI2 clock and pin control", accessRW, clkpower__spi_ctrl},
	{0x400040ac, 0,          "I2CCLK_CTRL", "I2C clock control register", accessRW, clkpower__i2cclk_ctrl},
	{0x400040c0, 0,          "TIMCLK_CTRL1", "Timer[5:0] and MCPWM clock control", accessRW, clkpower__timclk_ctrl1},
	{0x400040bc, 0,          "TIMCLK_CTRL", "Timer clock control", accessRW, clkpower__timclk_ctrl},
	{0x400040b4, 0,          "ADCLK_CTRL", "ADC clock control", accessRW, clkpower__adclk_ctrl},
	{0x40004060, 0,          "ADCLK_CTRL1", "Second ADC clock control register", accessRW, clkpower__adclk_ctrl1},
	{0x400040b0, 0,          "KEYCLK_CTRL", "Keypad clock control", accessRW, clkpower__keyclk_ctrl},
	{0x400040b8, 0,          "PWMCLK_CTRL", "PWM clock control", accessRW, clkpower__pwmclk_ctrl},
	{0x400040e4, 0x0000000f, "UARTCLK_CTRL", "General UART clock control register", accessRW, clkpower__uartclk_ctrl},
	{0x40004110, 0,          "POS0_IRAM_CTRL", "Internal memory power off control register 0", accessRW, clkpower__pos0_iram_ctrl},
	{0x40004114, 0,          "POS1_IRAM_CTRL", "Internal memory power off control register 1", accessRW, clkpower__pos1_iram_ctrl},
};
size_t clkpowerSZ = (sizeof(clkpower)/sizeof(clkpower[0]));
