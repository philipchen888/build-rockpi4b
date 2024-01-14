/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include "timer.h"
#include "pwm.h"
#include "uart.h"

void pwmSetRange( int period )
{
    int pwm_value;
    int range;

    range = *PWM1_PERIOD;           //Get period
    pwm_value = range - *PWM1_DUTY; //Get duty
    *PWM1_CTRL &= ~(1<<0);          //Disable PWM
    *PWM1_PERIOD = period;          //Set period PWM
    *PWM1_CTRL |= (1<<0);           //Enable PWM
    pwmWrite( pwm_value );
}

void pwmSetClock( int value )
{
    int divisor;
    divisor = (value & 0x1ff) >> 1;

    *PWM1_CTRL &= ~(1<<0);        //Disable PWM
    *PWM1_CTRL = (*PWM1_CTRL & ~(0xff << 16)) | ((0xff & divisor) << 16) | (1<<9);                           //PWM div
    *PWM1_CTRL |= (1<<0);         //Enable PWM
}

void pwmMode( void )
{
    *GPIO4C =  (0x3000 << 16) | 0x1000;  // select PWM1
    udelay( 5000 );
    pwmSetRange ( 1024 );       // Default range of 1024
    pwmSetClock ( 80 );         // 48Mhz / 80 = 600KHz
}

void pwmWrite( int value )
{
    int range;

    if ( value == 0 ) {
        range = *PWM1_PERIOD;
        *PWM1_DUTY = range;
    } else {
        range = *PWM1_PERIOD;
        *PWM1_CTRL &= ~(1<<0);        //Disable PWM
        *PWM1_DUTY = range - value;   //Set duty
        *PWM1_CTRL &= ~(1<<5);
        *PWM1_CTRL |= (1<<1);
        *PWM1_CTRL &= ~(1<<2);
        *PWM1_CTRL |= (1<<4);
        *PWM1_CTRL |= (1<<0);         //Enable PWM
    }
}

void pwmToneWrite( int freq )
{
    int divi;
    int pwm_clock;
    int range;
    int rangediv2;

    divi = ((*PWM1_CTRL >> 16) & 0xff) << 1;
    if(divi == 0)
        divi = 512;
    if (freq == 0)
        pwmWrite( 0 );
    else {
        pwm_clock = 600000; //48000000 / divi;
        //range = pwm_clock / freq;
        range = 0;
        while( pwm_clock > freq){
           pwm_clock -= freq;
           range += 1;
        }
        pwmSetRange( range );
        rangediv2 = range >> 1;
        pwmWrite ( rangediv2 );
    }
}

void pwm_led_test( void )
{
    int i;
    int bright;

    pwmMode();
    for (i=0; i< 10; i++) {
        for (bright = 0; bright < 1024; bright=bright+4)
        {
          pwmWrite ( bright );
          udelay( 5000 );
        }

        for (bright = 1023; bright >= 0; bright=bright-4)
        {
          pwmWrite ( bright );
          udelay( 5000 );
        }
    }
}

void tone( int melody, int duration, int duration2 ) 
{
    int melodydiv2;

    melodydiv2 = melody >> 1;
    if ( melody == 0 ) {
       pwmWrite( 0 );
       udelay( duration );
    } else {
       pwmToneWrite( melody );
       pwmWrite( melodydiv2 );
       udelay( duration );
       pwmWrite( 0 );
    }
    udelay( duration2 ); // * 0.8 );
}

void tongsong( void )
{
    int thisNote;
    int duration;
    int duration2;
    int tmp;

    int melody[] = {
      262, 196, 196, 220, 196, 0, 247, 262
    };

    // note durations: 4 = quarter note, 8 = eighth note, etc.:
    int noteDurations[] = {
      4, 8, 8, 4, 4, 4, 4, 4
    };

    pwmMode();
    for ( thisNote = 0; thisNote < 8; thisNote++ ) {
       tmp = noteDurations[thisNote];
       //duration = 1000000 / tmp;
       if ( tmp == 4 ) {
           duration  = 250000;
           duration2 = 200000;
       } else if ( tmp == 8 ) {
           duration  = 125000;
           duration2 = 100000;
       }
       tone( melody[thisNote], duration, duration2 );
    }
}

void servo ( void )
{
    int i;

    pwmMode();
    pwmSetClock ( 287 );    // 48 MHz / 287 / 1024 = 163 Hz
    pwmSetRange ( 1024 );
    pwmWrite( 265 );        // min 175 = 0 degree, max 355 = 90 degree
    udelay( 1000000 );

    for (i=0; i< 3; i++) {
      pwmWrite( 175 );
      printf( "0 degree\r\n" );
      udelay( 1000000 );
      pwmWrite( 265 );
      printf( "45 degree\r\n" );
      udelay( 1000000 );
      pwmWrite( 355 );
      printf( "90 degree\r\n" );
      udelay( 1000000 );
      pwmWrite( 265 );
      printf( "45 degree\r\n" );
      udelay( 1000000 );
    }
    pwmWrite( 0 );
}
