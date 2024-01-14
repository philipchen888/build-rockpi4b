/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _GPIO_H_
#define _GPIO_H_

#include "registers.h"

int digitalRead( void );
void digitalWrite ( int state );
void led_test( void );
void button_test( void );

#endif
