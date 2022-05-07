#define VERSION_STR    "015_Temple 10 Beacons by Pointer, 2022.05.07"

/*
 * 10 Beacons, different on times, different off times
 */

#include "Beacon.h"
#include "Heartbeat.h"

#define HEARTBEATPIN        13
#define HEARTBEATTIME      500

#define BEACONS             10

uint8_t u8aBeaconPins[ BEACONS ]           = {   12,   11,   A3,    9,    8,    7,    6,    5,    4,    3 };
unsigned long ulaBeaconOnTimes[ BEACONS ]  = {  100,  100,  100,  250,  250,   50,   50,  100,  100,  250 };
unsigned long ulaBeaconOffTimes[ BEACONS ] = {  500, 1000, 2000, 1000,  500,  500, 1000, 1000, 4000, 3000 };

Beacon    *myBeacons[ BEACONS ];
Heartbeat *myHeart        = new Heartbeat( HEARTBEATPIN, HEARTBEATTIME );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  for( uint8_t i = 0; i< BEACONS; ++i ) {
    myBeacons[i] = new Beacon( u8aBeaconPins[i], ulaBeaconOnTimes[i], ulaBeaconOffTimes[i] );
    myBeacons[i]->begin( );
  } // for

  myHeart->begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  for( uint8_t i = 0; i< BEACONS; ++i ) {
    myBeacons[i]->update( );
  } // for

  myHeart->update( );
} // loop( )

