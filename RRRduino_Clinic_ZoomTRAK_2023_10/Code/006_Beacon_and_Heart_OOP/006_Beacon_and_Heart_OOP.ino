#define VERSION_STR    "006_Beacon_and_Heart_OOP, ZoomTRAK, 2023.10.28"

/*
 * No more use of the blocking delay call in loop( )
 * Heartbeat Showing OOP: Obeject Oriented Programming
 * You already used it with Serial.begin() and Serial.println(...)
 * 
 * Pin 13 will do:
 *    \_________617_________/--50--  ,  \_________617_________/--50--  ,  \__
 *    x                     x        ,  x                     x        ,  x    
 *    
 * Pin 5 will do:
 *    \100/-------------------900------------------  ,  \100/-------------------900------------------  ,  \__
 *    x   x                                          ,  x   x                                          ,  x    
 
 *    
 * Author: Gert 'Speed' Muller, 2023.10.06
 */

#include "Heartbeat.h"

#define LEDPIN1        13
#define TIMEON         50
#define TIMEOFF       617

#define BEACONPIN1      5
#define BCNTIMEON     100
#define BCNTIMEOFF    900

// 50+617 ms --> 90 beats per minute

// compared this with 
// const unsigned long ulTimeOn  =  50;
// const unsigned long ulTimeOff = 617;

Heartbeat myHeart = Heartbeat( LEDPIN1, (unsigned long)TIMEON, (unsigned long)TIMEOFF );
Heartbeat myBeacon = Heartbeat( BEACONPIN1, BCNTIMEON, BCNTIMEOFF );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  myBeacon.begin( );
  myHeart.begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forver loop in main.cpp calls loop over and over
void loop( ) {
  myBeacon.update( );
  myHeart.update( );
} // loop( )
