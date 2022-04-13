lpc32x0 utilities
=================

Introduction
------------
This repository contains a number of utilities for poking around on an NXP
lpc32x0 SoC. The current set of utilities include:
  * `lpc32x0-offline`
  * `lpc32x0-dump`
  * `lpc32x0-write`
  * `lpc32x0-spi`

`lpc32x0-dump`, `lpc32x0-write`, and `lpc32x0-spi` are meant to be run on
an lpc32x0 device and will interact with the actual registers of the
lpc32x0 SoC to get and potentially set their values. These utilities
require adequate privilege in order to run successfully (i.e. be root).

`lpc32x0-offline`, on the other hand, can be run on an lpc32x0 device or on
another device, and will simply display information about the data you give
to it.

lpc32x0-offline
---------------
Use this program to fully decode the registers and register sub-fields
of the lpc3250 configuration registers. This program does not probe the actual
contents of the registers; it only responds to the data you give it.

This program expects input on stdin, one line at a time. Each line is expected
to start with an address, followed by a colon, followed by up to 4 words of
data (i.e. the contents of memory starting at the address, plus up to the next
3 32-bit locations). E.g.

```
0x400240e0:     0x13cc1980      0x411028da      0x553e4d00      0x98180c9b
```

This expected input format was chosen because it is easy to generate from gdb
(e.g. while remote jtag debugging via openocd) and is also easy to generate
from U-Boot.

From gdb:

```
(gdb) x /7 0x40024000
0x40024000:     0x0066506c      0xfea1eb93      0x00800440      0x00010401
0x40024010:     0x00000402      0x00000000      0x38802e75
```

From U-Boot:

```
uboot> md.l 0x40024000 7
40024000: 0060418b bea7fa74 00800400 00010401    .A`.t...........
40024010: 00000000 00000000 38802e75             ........u..8
```

lpc32x0-dump
------------
Use this utility on-device to see the contents of various registers. You can
specify the register(s) by specifying them on the cmdline with the
`-r|--reg` option, you can specify sets of registers with the `-s|--set`
option, or a combination of both. Both options take a space-delimited,
comma-delimited, or space-and-comma-delimited list of registers/sets.
Registers are specified by providing their address(es).

By default using the `-s|--set` option will show their contents; specifying
the `-r|--reg` option will give a detailed breakdown of the sub-fields
within the register. The `-v|--verbose` option can be used with the
`-s|--set` option to show detailed information for sets of registers.

The `-h|--help` option shows the help information along with the
currently-supported list of register set names.

examples:

	# lpc32x0-dump -h
	usage:
	lpc32x0_dump [<options>]
	  where:
	    options:
	      -h|--help      print usage information and exit successfully
	      -v|--verbose   print register value and sub-field values
	      -r|--reg <reg> specify register(s) by address
	      -s|--set <set> specify set(s) of registers by name
	      register set names are:
		clkpwr
		gpdma
		emc
		mlc
		slc
		lcd
		ts
		spi
		ssp
		gpio
		pinmux

	# lpc32x0-dump -s spi,ssp
	spi:
	  SPI_CTRL (400040c4)                   0x00000003
	  SPI1_GLOBAL (20088000)                0x00000001
	  SPI2_GLOBAL (20090000)                0x00000000
	  SPI1_CON (20088004)                   0x00802e83
	  SPI2_CON (20090004)                   0x00000e08
	  SPI1_FRM (20088008)                   0x00000000
	  SPI2_FRM (20090008)                   0x00000000
	  SPI1_IER (2008800c)                   0x00000000
	  SPI2_IER (2009000c)                   0x00000000
	  SPI1_STAT (20088010)                  0x00000000
	  SPI2_STAT (20090010)                  0x00000001
	  SPI1_DAT (20088014) - skip non-readable
	  SPI2_DAT (20090014) - skip non-readable
	  SPI1_TIM_CTRL (20088400)              0x00000002
	  SPI2_TIM_CTRL (20090400)              0x00000002
	  SPI1_TIM_COUNT (20088404)             0x00000000
	  SPI2_TIM_COUNT (20090404)             0x00000000
	  SPI1_TIM_STAT (20088408)              0x00008000
	  SPI2_TIM_STAT (20090408)              0x00000000
	ssp:
	  SSP_CTRL (40004078)                   0x00000000
	  SSP0CR0 (20084000)                    0x00000000
	  SSP1CR0 (2008c000)                    0x00000000
	  SSP0CR1 (20084004)                    0x00000000
	  SSP1CR1 (2008c004)                    0x00000000
	  SSP0DR (20084008) - skip non-readable
	  SSP1DR (2008c008) - skip non-readable
	  SSP0SR (2008400c)                     0x00000000
	  SSP1SR (2008c00c)                     0x00000000
	  SSP0CPSR (20084010)                   0x00000000
	  SSP1CPSR (2008c010)                   0x00000000
	  SSP0IMSC (20084014)                   0x00000000
	  SSP1IMSC (2008c014)                   0x00000000
	  SSP0RIS (20084018)                    0x00000000
	  SSP1RIS (2008c018)                    0x00000000
	  SSP0MIS (2008401c)                    0x00000000
	  SSP1MIS (2008c01c)                    0x00000000
	  SSP0ICR (20084020) - skip non-readable
	  SSP1ICR (2008c020) - skip non-readable
	  SSP0DMACR (20084024)                  0x00000000
	  SSP1DMACR (2008c024)                  0x00000000

	# lpc32x0-dump -r 0x20088004
	0x20088004 SPI1_CON        SPI1 control
		type:  RW
		reset: 0x00000e08
		value: 0x00802e83
			   [23] selects bidirectional or unidirectional usage of the SPIn_DATIO pin
				0b1................................SPIn_DATIO pin is unidirectional
			   [22] busy halt, determines whether SPIn_BUSY affects SPI operation
				0b0................................the SPIn_BUSY pin is ignored during master operation
			   [21] busy polarity - controls the polarity of the SPIn_BUSY signal
				0b0................................SPIn_BUSY is active LOW
			   [20] (reserved)
				0b0................................
			   [19] endian - controls the order in which the bits are transfered
				0b0................................data is xfered MSB-first
			   [18] (reserved)
				0b0................................
			[17:16] SPI mode selection
				0b00...............................SPI mode 0: clock starts low, data is sampled at the clock rising edge
			   [15] rxtx - controls the direction of data transfer
				0b0................................data is shifted into the SPI (receive)
			   [14] thr - controls the FIFO threshold, determines operation of interrupt flag
				0b0................................the FIFO threshold is disabled; threshold=1 entry in FIFO
			   [13] shift_off - controls generation of clock pulses on SPIn_CLK
				0b1................................disables the generation of clock pulses on SPIn_CLK
			 [12:9] bitnum - defines the number of bits to xmit or rcve in one block xfer
				0b0111.............................8 bits
			    [8] (reserved)
				0b0................................
			    [7] master
				0b1................................SPI is operating as master
			  [6:0] rate - SPI transfer rate - SPIn_CLK = HCLK / ((rate+1) x 2)
				0b0000011..........................value: 3

lpc32x0-write
-------------
Use this program to set register values. The expected usage is:

	# lpc32x0-write <addr> <value>

This program will only set the value of a register if it is different from its
current value.

lpc32x0-spi
-----------
This program is an investigation program I wrote to interact with what is
assumed to be a spi-nor flash (e.g. M25P16) sitting on the SPI1 bus which is
enabled by /GPIO_5.

This program configures the lpc32x0's SPI controller, performs a "read
identification" command (0x9f), displays the result, performs a "fast
read" command (0x0b) to grab the first 256 bytes of data from the spi-nor, and
displays those results.

It interacts with the SPI controller's registers, as well as with the chip
select, directly.


Compiling/Building
------------------
- this project uses cmake
- assuming you want to build in a subdirectory under the main checkout called
  `builddir`, from the project's top-level directory, run:

```
$ mkdir builddir
$ cd builddir
$ cmake ..
```

- this only ever needs to be done once
- then switch to the build location (i.e. `builddir`) and run:

```
$ make
```
