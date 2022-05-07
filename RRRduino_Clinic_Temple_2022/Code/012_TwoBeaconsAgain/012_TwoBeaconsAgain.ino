#define VERSION_STR    "012_Temple Beacons Again, 2022.05.07"

/*
 * Temple Beacons Again by OOP
 */

#include "Beacon.h"
#include "Heartbeat.h"

#define HEARTBEATPIN        13
#define HEARTBEATTIME      500

#define BEACON1PIN          12
#define BEACON2PIN          11
#define BEACONONTIME        50
#define BEACONOFFTIME     2000

Beacon    myBeacon       = Beacon( BEACON1PIN, BEACONONTIME, BEACONOFFTIME );
Beacon    mySecondBeacon = Beacon( BEACON2PIN, BEACONONTIME, BEACONOFFTIME );
Heartbeat myHeart        = Heartbeat( HEARTBEATPIN, HEARTBEATTIME );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  myBeacon.begin( );
  mySecondBeacon.begin( );
  myHeart.begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  myBeacon.update( );
  mySecondBeacon.update( );
  myHeart.update( );
} // loop( )

