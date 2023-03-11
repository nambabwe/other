#define VERSION_STR    "008_OOP_Beacons, LSR Div1, 2023.03.11"

/*
 * Beacons with Object Oriented Programming, shorter off time on first beacon
 */

#include "Beacon.h"
#include "Heartbeat.h"

#define HEARTBEATPIN        13
#define HEARTBEATONTIME     50
#define HEARTBEATOFFTIME   617

#define BEACONPIN1           5
#define BEACONPIN2           6
#define BEACONONTIME       100
#define BEACONOFFTIME     2000

Beacon    myBeacon1 = Beacon( BEACONPIN1, BEACONONTIME, BEACONOFFTIME/1.5 );
Beacon    myBeacon2 = Beacon( BEACONPIN2, BEACONONTIME, BEACONOFFTIME );
Heartbeat myHeart  = Heartbeat( HEARTBEATPIN, (unsigned long)HEARTBEATONTIME, (unsigned long)HEARTBEATOFFTIME );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  myBeacon1.begin( );
  // delay( 200 );     // no delay needed, the beacons are independant
  myBeacon2.begin( );
  myHeart.begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  myBeacon1.update( );
  myBeacon2.update( );
  myHeart.update( );
} // loop( )
