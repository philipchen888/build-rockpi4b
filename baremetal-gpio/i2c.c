/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include "i2c.h"
#include "timer.h"

/*
 * SCL Divisor = 8 * (CLKDIVL+1 + CLKDIVH+1)
 * SCL = PCLK / SCLK Divisor
 * i2c_rate = PCLK
 */

void rk3399_i2c_set_clk( int scl_rate )
{
    unsigned int src_clk_div;

    src_clk_div = 39;           // GPLL_HZ 800 * MHz / 40 = 20 MHz
    *CRU_CLKSEL_CON63 = (0xff00 << 16) | (src_clk_div << 8) | 1<<15;
    *I2C7_CLKDIV = 0x000b000b;  // 100 KHz
}

int rk_i2c_send_start_bit( void )
{
	int TimeOut = I2C_TIMEOUT_US;

        *I2C7_IPD = I2C_IPD_ALL_CLEAN;
        *I2C7_CON = I2C_CON_EN | I2C_CON_START;
        *I2C7_IEN = I2C_STARTIEN;

	while (TimeOut--) {
	    if (*I2C7_IPD & I2C_STARTIPD) {
                *I2C7_IPD = I2C_STARTIPD;
		break;
	    }
	    udelay( 1 );
	}
	if (TimeOut <= 0) {
		return I2C_ERROR_TIMEOUT;
	}

	return I2C_OK;
}

int rk_i2c_send_stop_bit( void )
{
	int TimeOut = I2C_TIMEOUT_US;

        *I2C7_IPD = I2C_IPD_ALL_CLEAN;
        *I2C7_CON = I2C_CON_EN | I2C_CON_STOP;
        *I2C7_IEN = I2C_CON_STOP;

        while (TimeOut--) {
	    if (*I2C7_IPD & I2C_STOPIPD) {
                *I2C7_IPD = I2C_STOPIPD;
	        break;
	    }
	    udelay( 1 );
	}
	if (TimeOut <= 0) {
		return I2C_ERROR_TIMEOUT;
	}

	return I2C_OK;
}

void rk_i2c_disable( void )
{
        *I2C7_CON = 0x0;
}

int rk_i2c_write( char chip, int reg, int r_len, char *buf, int b_len )
{
	int err = I2C_OK;
	int TimeOut = I2C_TIMEOUT_US;
	char *pbuf = buf;
	int bytes_remain_len = b_len + r_len + 1;
	int bytes_tranfered_len = 0;
	int words_tranfered_len = 0;
	int txdata;
	int i, j;

	err = rk_i2c_send_start_bit();
	if (err != I2C_OK) {
	    return err;
	}

        while (bytes_remain_len) {
            if (bytes_remain_len > RK_I2C_FIFO_SIZE) {
	        bytes_tranfered_len = 32;
            } else {
	        bytes_tranfered_len = bytes_remain_len;
            }
            words_tranfered_len = (bytes_tranfered_len + 3) / 4;

	    for (i = 0; i < words_tranfered_len; i++) {
	        txdata = 0;
	        for (j = 0; j < 4; j++) {
	            if ((i * 4 + j) == bytes_tranfered_len) {
		        break;
		    }

		    if (i == 0 && j == 0) {
		        txdata |= (chip << 1);
		    } else if (i == 0 && j <= r_len) {
		        txdata |= (reg & (0xff << ((j - 1) * 8))) << 8;
		    } else {
		        txdata |= (*pbuf++)<<(j * 8);
		    }
                    *(volatile unsigned int *)(I2C7_TXDATA_BASE + i) = txdata;
		}
	    }

            *I2C7_CON = I2C_CON_EN | I2C_CON_MOD(I2C_MODE_TX);
            *I2C7_MTXCNT = bytes_tranfered_len;
            *I2C7_IEN = I2C_MBTFIEN | I2C_NAKRCVIEN;

	    TimeOut = I2C_TIMEOUT_US;
	    while (TimeOut--) {
	        if (*I2C7_IPD & I2C_NAKRCVIPD) {
                    *I2C7_IPD = I2C_NAKRCVIPD;
	            err = I2C_ERROR_NOACK;
	        }
	        if (*I2C7_IPD & I2C_MBTFIPD) {
                    *I2C7_IPD = I2C_MBTFIPD;
	            break;
		}
		udelay( 1 );
	    }

	    if (TimeOut <= 0) {
	        err =  I2C_ERROR_TIMEOUT;
	        goto i2c_exit;
	    }

	    bytes_remain_len -= bytes_tranfered_len;
	}

i2c_exit:
	rk_i2c_send_stop_bit();
	rk_i2c_disable();

	return err;
}

void wiringPiI2CWriteReg8( char chip, int reg, int bufi )
{
    char buf[1] = {bufi};
    rk_i2c_write( chip, reg, 1, buf, 1 );  
}

void set_backlight( int r, int g, int b )
{
    wiringPiI2CWriteReg8( 0x62, 0, 0 );
    wiringPiI2CWriteReg8( 0x62, 1, 0 );
    wiringPiI2CWriteReg8( 0x62, 8, 0xaa );
    wiringPiI2CWriteReg8( 0x62, 4, r );
    wiringPiI2CWriteReg8( 0x62, 3, g );
    wiringPiI2CWriteReg8( 0x62, 2, b );
}

void textCommand( int cmd )
{
    wiringPiI2CWriteReg8( 0x3e, 0x80, cmd);
}

void setText( char * text )
{
    int i;
    textCommand( 0x01 );        // clear display
    udelay( 50000 );
    textCommand( 0x08 | 0x04 ); // display on, no cursor
    textCommand( 0x28 );        // 2 lines
    udelay( 50000 );
    for (i=0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            textCommand( 0xc0 );
        } else {
            wiringPiI2CWriteReg8( 0x3e, 0x40, text[i]);
        }
    }
}

void i2c_lcd_test( void )
{
    int j;

    *GPIO2A = (0xc000 << 16) | 0x8000;  // select I2C7
    *GPIO2B = (0x3 << 16) | 0x2;        // select I2C7
    rk3399_i2c_set_clk( 100000 );
    textCommand( 0x01 );        // clear display
    udelay( 50000 );
    textCommand( 0x08 | 0x04 ); // display on, no cursor
    textCommand( 0x28 );        // 2 lines
    udelay( 50000 );

    set_backlight( 255, 0, 0 );
    for (j=0; j< 5; j++)
    {
      set_backlight( 255, 0, 0 );
      udelay( 1000000 ) ;
      set_backlight( 255, 255, 0 );
      udelay( 1000000 ) ;
      set_backlight( 0, 255, 0 );
      udelay( 1000000 ) ;
      set_backlight( 0, 255, 255 );
      udelay( 1000000 ) ;
      set_backlight( 0, 0, 255 );
      udelay( 1000000 ) ;
      set_backlight( 255, 0, 255 );
      udelay( 1000000 ) ;
    }
    set_backlight( 128, 255, 0 );
    setText( "Hello world !\nIt works !\n" );
}
