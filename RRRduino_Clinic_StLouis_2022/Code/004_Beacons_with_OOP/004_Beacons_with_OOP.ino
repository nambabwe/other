#define VERSION_STR    "003_StLouis Blink_with_OOP, 2022.08.09"

/*
 * No use of the blocking delay call in loop( )
 * Heartbeat and Beacon showing OOP: Obeject Oriented Programming
 * Attached 1k resistor and LED (in series) between pin 12 and GND
 * pin 12, --|>--, --R--, GND.  LED cathode towards GND.
 * 
 * Pin 13 will do:
 *    \____617____/-50-,  \____617____/-50-,  \__
 *    x           x    ,  x           x    ,  x    
 *    
 * Pin 12 will do:
 *    \_______1900________/-100--,  \_______1900________/-100--,  \__
 *    x                   x      ,  x                   x      ,  x    
 *    
 *    
 * Author: Gert 'Speed' Muller, 2022.08.09
 */

#include "Heartbeat.h"

#define LEDPIN1        13
#define TIMEOFF       617
#define TIMEON         50

#define BEACONPIN1     12
#define BCNTIMEOFF   1900
#define BCNTIMEON     100

Heartbeat myHeart  = Heartbeat( LEDPIN1, TIMEON, TIMEOFF );
Heartbeat myBeacon = Heartbeat( BEACONPIN1, BCNTIMEON, BCNTIMEOFF );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  myHeart.begin( );
  myBeacon.begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forver loop in main.cpp calls loop over and over
void loop( ) {
  myHeart.update( );
  myBeacon.update( );
} // loop( )
