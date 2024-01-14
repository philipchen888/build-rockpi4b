/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _RK_I2C_H_
#define _RK_I2C_H_

#include "registers.h"
 
/* i2c timerout */
#define I2C_TIMEOUT_MS		100
#define I2C_TIMEOUT_US		100000
#define I2C_RETRY_COUNT		3

/* i2c error return code */
#define I2C_OK			0
#define I2C_ERROR_TIMEOUT	-1
#define I2C_ERROR_NOACK		-2
#define I2C_ERROR_IO		-3

/* Clock dividor register */
#define I2C_CLKDIV_VAL(divl, divh) \
        (((divl) & 0xffff) | (((divh) << 16) & 0xffff0000))

/* rk i2c fifo max transfer bytes */
#define RK_I2C_FIFO_SIZE	32

#define I2C_CON      0x0000
#define I2C_CLKDIV   0x0004
#define I2C_MRXADDR  0x0008
#define I2C_MRXRADDR 0x000c
#define I2C_MTXCNT   0x0010
#define I2C_MRXCNT   0x0014
#define I2C_IEN      0x0018
#define I2C_IPD      0x001c
#define I2C_FCNT     0x0020
#define I2C_TXDATA   0x0100
#define I2C_RXDATA   0x0200
#define I2C_TXDATA_BASE         0X100
#define I2C_RXDATA_BASE         0x200

#define I2C_MRXADDR_SET(vld, addr)	(((vld) << 24) | (addr))
#define I2C_MRXRADDR_SET(vld, raddr)	(((vld) << 24) | (raddr))

#define I2C_CON_EN		(1 << 0)
#define I2C_CON_MOD(mod)	((mod) << 1)
#define I2C_MODE_TX		0x00
#define I2C_MODE_TRX		0x01
#define I2C_MODE_RX		0x02
#define I2C_MODE_RRX		0x03
#define I2C_CON_MASK		(3 << 1)

#define I2C_CON_START		(1 << 3)
#define I2C_CON_STOP		(1 << 4)
#define I2C_CON_LASTACK		(1 << 5)
#define I2C_CON_ACTACK		(1 << 6)

#define I2C_BTFIEN		(1 << 0)
#define I2C_BRFIEN		(1 << 1)
#define I2C_MBTFIEN		(1 << 2)
#define I2C_MBRFIEN		(1 << 3)
#define I2C_STARTIEN		(1 << 4)
#define I2C_STOPIEN		(1 << 5)
#define I2C_NAKRCVIEN		(1 << 6)

#define I2C_BTFIPD              (1 << 0)
#define I2C_BRFIPD              (1 << 1)
#define I2C_MBTFIPD             (1 << 2)
#define I2C_MBRFIPD             (1 << 3)
#define I2C_STARTIPD            (1 << 4)
#define I2C_STOPIPD             (1 << 5)
#define I2C_NAKRCVIPD           (1 << 6)
#define I2C_IPD_ALL_CLEAN       0x7f

void rk_i2c_get_div( int div, int *divh, int *divl );
void rk_i2c_set_clk( int scl_rate );
int rk_i2c_send_start_bit( void );
int rk_i2c_send_stop_bit( void );
void rk_i2c_disable( void );
int rk_i2c_read( char chip, int reg, int r_len, char *buf, int b_len );
int rk_i2c_write( char chip, int reg, int r_len, char *buf, int b_len );
void wiringPiI2CWriteReg8( char chip, int reg, int bufi );
void set_backlight( int r, int g, int b );
void textCommand( int cmd );
void setText( char * text );
void i2c_lcd_test( void );

#endif
