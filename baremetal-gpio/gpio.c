/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include "gpio.h"
#include "timer.h"

int digitalRead( void )
{
    if ((*GPIO1_EXT_PORTA & 0x200) != 0x0) {
        return 1;
    } else {
        return 0;
    }
}

void digitalWrite( int state )
{
    if ( state == LOW ) {
        *GPIO3_SWPORTA_DR &= ~(0x10000);
    } else if ( state == HIGH ) {
        *GPIO3_SWPORTA_DR |= 0x10000;
    }
}

void led_test( void )
{
    int i;

    // GPIO3_C0 as output
    *GPIO3C = (0x3 << 16);
    *GPIO3_SWPORTA_DDR |= 0x10000;

    for (i=0; i< 5; i++) {
        digitalWrite ( HIGH );
        udelay( 1000000 );
        digitalWrite ( LOW );
        udelay( 1000000 );
    }
}

void button_test( void )
{
    int i;
    int old_state;
    int current_state;

    // GPIO3_C0 as output
    *GPIO3C = (0x3 << 16);
    *GPIO3_SWPORTA_DDR |= 0x10000;
    // GPIO1_B1 as input
    *PMUGRF_GPIO1B = (0xc << 16);
    *GPIO1_SWPORTA_DDR &= ~(0x200);

    printf("Push button 10 times\r\n");
    old_state = 0;
    for (i=0; i<10; ) {
        current_state = digitalRead();
        if ( old_state == 0 && current_state == 1 ) {
            digitalWrite ( HIGH );  // On
            old_state = current_state;
        } else if ( old_state == 1 && current_state == 0 ) {
            digitalWrite ( LOW );   // Off
            old_state = current_state;
            i++;
        }
        udelay( 50000 );
    }
}
