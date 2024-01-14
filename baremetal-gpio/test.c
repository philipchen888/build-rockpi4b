/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include "registers.h"
#include "uart.h"
#include "timer.h"
#include "clock.h"
#include "gpio.h"
#include "pwm.h"
#include "i2c.h"
#include "spi.h"
#include "mmc.h"

int current_el( void )
{
    int el;
    asm volatile("mrs %0, CurrentEL" : "=r" (el) : : "cc");
    return el >> 2;
}

int getCpuId( void )
{
    asm volatile( "mrs x0, mpidr_el1\n"
                  "and x0, x0, #3\n"
                  "ret" );
}

unsigned int randomized( unsigned int i )
{
    unsigned int j;
    int k;
    int m;

    k = (i >> 28) + (i >> 2) & 0xf;
    if ( k < 5 )
        k = 5;
    j = i + 3;
    for (m = 0; m < k; m++) {
       if ( j & 0x80000000 ) {
           j = ((j ^ 0x00000062) << 1) + 1;
       } else {
           j = (j << 1);
       }
    }
    return j;
}

void ddr_test( void )
{
    unsigned long long i;
    printf( "In ddr_test\r\n");

    printf( "Writing 2GB DDR with random data\r\n");
    for ( i = 0x1000000; i < 0x8000000; i = i + 4 ) {
        *(volatile unsigned int *)i = randomized( i );
        if ( i < 0x1000100 )
            printf( "0x%x\r\n", *(volatile unsigned int *)i);
        if ( (i & 0xfffff) == 0x0 )
            printf( ".");
    }
    printf( "Reading 2GB DDR\r\n");
    for ( i = 0x1000000; i < 0x8000000; i = i + 4 ) {
        if ( i < 0x1000100 )
            printf( "0x%x\r\n", *(volatile unsigned int *)i);
        if ( *(volatile unsigned int *)i != randomized( i ) )
            printf( "dram test failed !\r\n");
        if ( (i & 0xfffff) == 0x0 )
            printf( ".");
    }
}

void displaydst( char *dst )
{
    int i;
    unsigned int tmp;

    for (i=0; i<512; i++) {
        printf( "%02X ", dst[i] );
        if ( (i & 0xf) == 0xf ) {
            printf( "\r\n" );
        }
    }
}

void mmc_test( void )
{
    int i;
    char dst[512] __attribute__((aligned(64)));
    char src[512] __attribute__((aligned(64)));

    printf( "In mmc_test\r\n");
    printf( "Read sdcard lba 32768\r\n");
    mmc_bread( dst, 32768, 1 );
    displaydst( dst );
    printf( "Read sdcard lba 6283260\r\n");
    mmc_bread( dst, 6283260, 1 );
    displaydst( dst );

    // prepare write data
    for (i=0; i < 512; i++) {
        src[i] = i + 7;
    }
    printf( "Write sdcard lba 6283260\r\n");
    mmc_bwrite( src, 6283260, 1 );
    printf( "Read sdcard lba 6283260\r\n");
    mmc_bread( dst, 6283260, 1 );
    displaydst( dst );
}

void dmac_test( void )
{
    unsigned int sram2ddrcode[8] __attribute__((aligned(64))) = {0x000000bc, 0x02bc0100, 0x20000000, 0xc2f701bc, 0x7f2000bd, 0x02380804, 0x00000000};
    unsigned int ddr2ddrcode[8] __attribute__((aligned(64))) = {0x000000bc, 0x02bc2000, 0x30000000, 0xc2f701bc, 0x7f2000bd, 0x02380804, 0x00000000};
    unsigned long long i;
    unsigned long long src;
    unsigned long long dest;
    int k;
    // dma 16KB, 16 beats with 8 byte width, loop 128 times
    // DMAMOV SAR 0x01000000           (BC,00,00,00,00,01)
    // DMAMOV DAR 0x20000000           (BC,02,00,00,00,20)
    // DMAMOV CCR SB16 SS64 DB16 DS64  (BC,01,F7,C2,BD,00)  // was BC,01,F7,C0,3D,00
    // DMALP lc0 128                   (20,7f)
    // DMALD                           (04)
    // DMAST                           (08)
    // DMALPEND lc0                    (38,02)
    // DMAEND                          (00)

    for ( i = 0x1000000; i < 0x1004000; i = i + 4 ) {
        *(volatile unsigned int *)i = randomized( i );
        if ( i < 0x1000040 )
            printf( "0x%x\r\n", *(volatile unsigned int *)i);
        if ( (i & 0xfffff) == 0x0 )
            printf( ".");
    }
    //---- sram to ddr then ddr to ddr  -----------------------------
    for ( k = 0; k < 2; k++ ) {
        // wait idle
        while( *DMAC_BUS_DBGST & 0x1 );
        *DMAC_BUS_DBGINST0 = 0x00A20000;  // non secure, was 0x00A00000
        if ( k == 0 ) {
            *DMAC_BUS_DBGINST1 = (unsigned long long)sram2ddrcode;
            *(volatile unsigned int *)0x20000000 = 0x12345678;
        } else {
            *DMAC_BUS_DBGINST1 = (unsigned long long)ddr2ddrcode;
            *(volatile unsigned int *)0x30000000 = 0x9abcdef0;
        }
        *DMAC_BUS_DBGCMD = 0x0;           // start DMA
        udelay( 10 );
        // wait idle
        while( *DMAC_BUS_DBGST & 0x1 );

        if ( k == 0 ) {
            src  = 0x01000000;
            dest = 0x20000000;
        } else {
            src  = 0x20000000;
            dest = 0x30000000;
        }
        for (i=0x0; i<0x4000; i = i + 4) {
            if (i < 0x40 )
                printf( "0x%x\r\n", *(volatile unsigned int *)dest );
            if ( *(volatile unsigned int *)dest != *(volatile unsigned int *)src ) {
                printf( "Error ! dest addr = 0x%x, dest data = 0x%x, src data = 0x%x,\r\n", dest, *(volatile unsigned int *)dest, *(volatile unsigned int *)src );
                break;
            }
            src += 4;
            dest += 4;
        }
        // registers changed after dma
        printf( "DMAC_BUS_CPC0 = 0x%x\r\n", *DMAC_BUS_CPC0 );
        printf( "DMAC_BUS_SAR0 = 0x%x\r\n", *DMAC_BUS_SAR0 );
        printf( "DMAC_BUS_DAR0 = 0x%x\r\n", *DMAC_BUS_DAR0 );
        printf( "DMAC_BUS_CCR0 = 0x%x\r\n", *DMAC_BUS_CCR0 );
    }
}

char test_menu( void )
{
    char ch;

    uart_clear();
    printf( "--- select a test ---\r\n" );
    printf( "1. uart test\r\n2. led test\r\n3. button test\r\n4. pwm led test\r\n5. i2c lcd test\r\n6. tongsong\r\n7. servo\r\n8. spi oled test\r\n9. ddr test\r\na. mmc test\r\nb. dmac test\r\nq. quit\r\n");
    ch = uart_getc();
    if ( ch == 'q' )
        printf( "Goodbye !\r\n" );
    return ch;
}

int main( void ) {
    char test_item;
    char ch;

    uart_init();
    printf("Cpu is %d\r\n", getCpuId());
    timer_init();
    rkclk_init();
    pmuclk_init();
    printf("Hello world ! EL = %d\r\n", current_el());

    mmc_init();

    test_item = '\0';
    while( test_item != 'q' ) {
        test_item = test_menu();
        if ( test_item == '1' ) {
            uart_test();
        } else if ( test_item == '2' ) {
            led_test();
        } else if ( test_item == '3' ) {
            button_test();                // push button 10 times
        } else if ( test_item == '4' ) {
            pwm_led_test();
        } else if ( test_item == '5' ) {
            i2c_lcd_test();
            printf( "press x to exit i2c lcd test\r\n" );
            ch = uart_getc();
        } else if ( test_item == '6' ) {
            tongsong();
        } else if ( test_item == '7' ) {
            servo();
        } else if ( test_item == '8' ) {
            ssd1306_test();
        } else if ( test_item == '9' ) {
            ddr_test();
        } else if ( test_item == 'a' ) {
            mmc_test();
        } else if ( test_item == 'b' ) {
            dmac_test();
        } 
    }

    while( 1 );
    return 0;
}
