/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __UART_H__
#define __UART_H__

#include "registers.h"
#define DEFAULT_UART  2

char uart_getc( void );
void uart_putc( char ch );
void uart_clear( void );
void uart_init( void );
void uart_test( void );

#endif
