/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include "uart.h"

char uart_getc( void )
{
    char ch;

    if ( DEFAULT_UART == 2 ) {
        while ( !(*UART2_LSR & 0x1) );
        ch = *UART2_DR & 0xff;
    } else if ( DEFAULT_UART == 4 ) {
        while ( !(*UART4_LSR & 0x1) );
        ch = *UART4_DR & 0xff;
    }
    return ch;
}

void uart_putc( char ch )
{
    if ( DEFAULT_UART == 2 ) {
        while (*UART2_LSR & UART_LSR_THRE)
              ;
        *UART2_THR = ch;
    } else if ( DEFAULT_UART == 4 ) {
        while (*UART4_LSR & UART_LSR_THRE)
              ;
        *UART4_THR = ch;
    }
}

// clear terminal screen

void uart_clear( void )
{
    char ch[5];

    ch[0] = 27;
    ch[1] = '[';
    ch[2] = '2';
    ch[3] = 'J';
    ch[4] = '\0';
    printf( ch );
}

void uart_init( void )
{
    char ch;

    if ( DEFAULT_UART == 2 ) {
        while ( *UART2_USR & 0x1 );  // wait for uart1 not busy
        *CRU_CLKSEL_CON35 = (0x300 << 16) | 0x200;  // select 24MHZ osc
        *GPIO4C = (0x3c0 << 16) | 0x140;
        *UART2_LCR = 0x83;           // 8N1
        *UART2_DLL = 0x01;           // 24000000 / 16 / 1500000 = 1
        *UART2_DLM = 0x0;
        *UART2_LCR = 0x03;           // 8N1
        *UART2_IER = 0x0;
        *UART2_MCR = 0x03;           // DTR, RTS
        *UART2_FCR = 0x07;           // enable fifo, reset rx fifo, reset tx fifo
        // clear received data
        while (*UART2_LSR & UART_LSR_DRY) {
            ch = *UART2_DR;
        }
        *UART2_IER = 0x80;
    } else if ( DEFAULT_UART == 4 ) {
        while ( *UART4_USR & 0x1 );  // wait for uart2 not busy
        *PMUCRU_CLKSEL_CON5 = (0x700 << 16) | 0x200;  // select 24MHZ osc
        *PMUGRF_GPIO1A = (0xc000 << 16) | 0x4000;
        *PMUGRF_GPIO1B = (0x3 << 16) | 0x1;
        *UART4_LCR = 0x83;           // 8N1
        *UART4_DLL = 0x01;           // 24000000 / 16 / 1500000 = 1
        *UART4_DLM = 0x0;
        *UART4_LCR = 0x03;           // 8N1
        *UART4_IER = 0x0;
        *UART4_MCR = 0x03;           // DTR, RTS
        *UART4_FCR = 0x07;           // enable fifo, reset rx fifo, reset tx fifo
        // clear received data
        while (*UART4_LSR & UART_LSR_DRY) {
            ch = *UART4_DR;
        }
        *UART4_IER = 0x80;
    }
}

void uart_test( void )
{
    char ch;

    printf( "Hello ! Type a sentence and press enter.\r\n" );
    ch = '\0';
    while( ch != '\r') {
        ch = uart_getc();
        uart_putc( ch );
    }
    printf( "\r\n" );
}
