#define VERSION_STR    "005_Blink_with_OOP, StLouis, 2022.08.09"

/*
 * No more use of the blocking delay call in loop( )
 * Heartbeat Showing OOP: Obeject Oriented Programming
 * You already used it with Serial.begin() and Serial.println(...)
 * 
 * Pin 13 will do:
 *    \_______617_______/--50--  ,  \_______617_______/--50--  ,  \__
 *    x                 x        ,  x                 x        ,  x    
 *    
 *    
 * Author: Gert 'Speed' Muller, 2022.08.09
 */

#include "Heartbeat.h"

#define LEDPIN1        13
#define TIMEON         50
#define TIMEOFF       617
// 50+617 ms --> 90 beats per minute

Heartbeat myHeart = Heartbeat( LEDPIN1, (unsigned long)TIMEON, (unsigned long)TIMEOFF );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  myHeart.begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forver loop in main.cpp calls loop over and over
void loop( ) {
  myHeart.update( );
} // loop( )
