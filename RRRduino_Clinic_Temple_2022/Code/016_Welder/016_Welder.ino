#define VERSION_STR    "016_Temple Welder, 2022.05.07"

/*
 * Welder with Heartbeat
 */


#include "Heartbeat.h"
#include "Welder.h"

#define HEARTBEATPIN        13
#define HEARTBEATTIME      500

#define WELDERPIN           A3
#define WELDERONTIME       250
#define WELDERONWHEN      HIGH

Welder     *myWelder   = new Welder( WELDERPIN, WELDEROFFRANDOM, WELDERONWHEN );
Heartbeat  *myHeart    = new Heartbeat( HEARTBEATPIN, HEARTBEATTIME );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  myWelder->begin( );
  myHeart->begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  myWelder->update( );
  myHeart->update( );
} // loop( )

