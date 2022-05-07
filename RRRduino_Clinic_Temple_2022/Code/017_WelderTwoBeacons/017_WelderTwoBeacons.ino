#define VERSION_STR    "017_Temple Welder with Two Beacons, 2022.05.07"

/*
 * Two Beacons with Welder
 */

#include "Beacon.h"
#include "Heartbeat.h"
#include "Welder.h"

#define HEARTBEATPIN         13
#define HEARTBEATTIME       500

#define BEACON1PIN           12
#define BEACON1ONTIME        50
#define BEACON1OFFTIME     3500

#define BEACON2PIN           11
#define BEACON2ONTIME        50
#define BEACON2OFFTIME     2500

#define WELDERPIN            A3
#define WELDERONTIME        250
#define WELDERONWHEN       HIGH

Beacon     *myBeacon1   = new Beacon( BEACON1PIN, BEACON1ONTIME, BEACON1OFFTIME );
Beacon     *myBeacon2   = new Beacon( BEACON2PIN, BEACON2ONTIME, BEACON2OFFTIME );
Welder     *myWelder    = new Welder( WELDERPIN, WELDEROFFRANDOM, WELDERONWHEN );
Heartbeat  *myHeart     = new Heartbeat( HEARTBEATPIN, HEARTBEATTIME );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  myBeacon1->begin( );
  myBeacon2->begin( );
  myWelder->begin( );
  myHeart->begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  myBeacon1->update( );
  myBeacon2->update( );
  myWelder->update( );
  myHeart->update( );
} // loop( )

