/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include "registers.h"

void timer_init( void );
void udelay( int usec );

#endif
