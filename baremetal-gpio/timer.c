/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include "timer.h"

void udelay( int usec )
{
    int i;
    unsigned long long tmp;
    unsigned long long tmp2;
    unsigned long long timebase_h;
    unsigned long long timebase_l;

    timebase_l = *TIMER5_CURR_VALUE0;
    timebase_h = *TIMER5_CURR_VALUE1;
    tmp = (timebase_h << 32) | timebase_l;
    tmp2 = 0;
    for ( i = 0; i < 24; i++ ) {            // timer frequency is 24 MHz
        tmp2 += usec;
    }
    tmp2 += tmp;
    tmp2 += 1;
    while( tmp < tmp2 ) {
        timebase_l = *TIMER5_CURR_VALUE0;
        timebase_h = *TIMER5_CURR_VALUE1;
        tmp = (timebase_h << 32) | timebase_l;
    }
}

void timer_init( void )
{
    *TIMER5_LOAD_COUNT2 = 0x0;
    *TIMER5_LOAD_COUNT3 = 0x0;
    *TIMER5_LOAD_COUNT0 = 0xffffffff;
    *TIMER5_LOAD_COUNT1 = 0xffffffff;
    *TIMER5_CTRL_REG    = 0x1;              // free running
}
