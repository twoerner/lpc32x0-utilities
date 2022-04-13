// SPDX-License-Identifier: OSL-3.0
/*
 * Copyright (C) 2018-2022  Trevor Woerner <twoerner@gmail.com>
 */

#ifndef LPC32X0_REGISTERS_H
#define LPC32X0_REGISTERS_H

#include <stdint.h>
#include <stdbool.h>

#define unused __attribute__((unused))

typedef enum {
	accessRead  = 1,
	accessWrite = 2,
} Access_e;
#define accessRW (accessRead | accessWrite)

typedef void (*ParseField_t)(uint32_t val);

typedef struct {
	uint32_t addr;
	uint32_t resetState;
	char *name_p;
	char *desc_p;
	Access_e access;
	ParseField_t field_fp;
} RegisterDescription_t;

typedef struct {
	char *name_p;
	size_t *sz_p;
	RegisterDescription_t *reg_p;
} AllRegisters_t;

void print_access(Access_e access);
uint32_t print_field (uint32_t val, unsigned start, unsigned end);
uint32_t get_field (uint32_t val, unsigned start, unsigned end);
bool lpc32x0__get_reg (uint32_t addr, uint32_t *regRet_p);
bool lpc32x0__set_reg (uint32_t addr, uint32_t val);
bool lpc32x0__print_reg (uint32_t addr, uint32_t val, bool verbose);
bool lpc32x0__get_and_print_reg_set_by_name (char *regSetName_p, bool verbose);
bool lpc32x0__get_and_print_all_regs (bool verbose);
void lpc32x0__cleanup (void);

#define ITEMFMT "\t\t%7s %s\n"

#define P2_MUX_CLR  0x4002802C
#define P2_DIR_SET  0x40028010
#define SPI_CTRL    0x400040C4
#define SPI1_GLOBAL 0x20088000
#define P3_OUTP_SET 0x40028004
#define P3_OUTP_CLR 0x40028008
#define SPI1_CON    0x20088004
#define SPI1_FRM    0x20088008
#define SPI1_STAT   0x20088010
#define SPI1_DAT    0x20088014
#define P3_INP_STATE 0x40028000

#endif
