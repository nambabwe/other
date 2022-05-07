#define VERSION_STR    "010_Temple OOP, 2022.05.07"

/*
 * No more use of the blocking delay call in loop( )
 * Heartbeat Showing OOP: Obeject Oriented Programming
 * You already used it with Serial.xxxx
 */

#include "Heartbeat.h"

#define LEDPIN1        13
#define TIME1         250

Heartbeat myHeart = Heartbeat( LEDPIN1, TIME1 );

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

