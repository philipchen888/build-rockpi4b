/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __PWM_H__
#define __PWM_H__

#include "registers.h"

void pwmSetRange( int period );
void pwmSetClock( int value );
void pwmMode( void );
void pwmWrite( int value );
void pwm_led_test( void );
void pwmToneWrite( int freq );
void tone( int melody, int duration, int duration2 );
void tongsong( void );
void servo ( void );

#endif
