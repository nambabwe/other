#define VERSION_STR    "006_OOP_Beacon, LSR Div1, 2023.03.11"

/*
 * Beacon with Object Oriented Programming
 */

#include "Beacon.h"
#include "Heartbeat.h"

#define HEARTBEATPIN        13
#define HEARTBEATONTIME     50
#define HEARTBEATOFFTIME   617

#define BEACONPIN            5
#define BEACONONTIME        50
#define BEACONOFFTIME     2000

Beacon    myBeacon = Beacon( BEACONPIN, BEACONONTIME, BEACONOFFTIME );
Heartbeat myHeart  = Heartbeat( HEARTBEATPIN, (unsigned long)HEARTBEATONTIME, (unsigned long)HEARTBEATOFFTIME );

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
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  myBeacon.update( );
  myHeart.update( );
} // loop( )
