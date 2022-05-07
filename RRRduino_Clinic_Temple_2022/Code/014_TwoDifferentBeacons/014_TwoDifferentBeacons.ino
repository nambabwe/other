#define VERSION_STR    "011_Temple Two Different Beacons by Pointer, 2022.05.07"

/*
 * Two Different Beacons by Pointer
 */

#include "Beacon.h"
#include "Heartbeat.h"

#define HEARTBEATPIN        13
#define HEARTBEATTIME      500

#define BEACON1PIN          12
#define BEACON1ONTIME       50
#define BEACON1OFFTIME    1000

#define BEACON2PIN          11
#define BEACON2ONTIME      250
#define BEACON2OFFTIME    3000

Beacon    *myBeacon       = new Beacon( BEACON1PIN, BEACON1ONTIME, BEACON1OFFTIME );
Beacon    *mySecondBeacon = new Beacon( BEACON2PIN, BEACON2ONTIME, BEACON2OFFTIME );
Heartbeat *myHeart        = new Heartbeat( HEARTBEATPIN, HEARTBEATTIME );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  myBeacon->begin( );
  mySecondBeacon->begin( );
  myHeart->begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forver loop in main.cpp calls loop over and over
void loop( ) {
  myBeacon->update( );
  mySecondBeacon->update( );
  myHeart->update( );
} // loop( )

