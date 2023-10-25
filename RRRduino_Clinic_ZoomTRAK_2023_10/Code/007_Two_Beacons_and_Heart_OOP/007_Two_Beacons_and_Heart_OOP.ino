#define VERSION_STR    "007_Two_Beacons_and_Heart_OOP, ZoomTRAK, 2023.10.28"

/*
 * No more use of the blocking delay call in loop( )
 * Heartbeat Showing OOP: Obeject Oriented Programming
 * You already used it with Serial.begin() and Serial.println(...)/home/gert/prj/git/other/RRRduino_Clinic_CannonballExpress_2023/Code/008_CampfireByTwoLights/008_CampfireByTwoLights.ino
 * 
 * Pin 13 will do:
 *    \__________617__________/--50--,  \__________617__________/--50--,  \__
 *    x                       x      ,  x                       x      ,  x    
 *    
 * Pin 5 will do:
 *    \100/-------------------900------------------  ,\100/-------------------900------------------,  \__
 *    x   x                                          ,x   x                                        ,  x    
  *
 * Pin 6 will do:
 *    \100/---------500---------,  \100/---------500---------,  \100/---------500---------,  \
 *    x   x                     ,  x   x                     ,  x   x                     ,  x
 
 *    
 * Author: Gert 'Speed' Muller, 2023.10.06
 */

#include "Heartbeat.h"

#define LEDPIN1        13
#define TIMEON         50
#define TIMEOFF       617

#define BEACONPIN1      5
#define BCNTIMEON1    100
#define BCNTIMEOFF1   900

#define BEACONPIN2      6
#define BCNTIMEON2    100
#define BCNTIMEOFF2   500

// 50+617 ms --> 90 beats per minute

// compared this with 
// const unsigned long ulTimeOn  =  50;
// const unsigned long ulTimeOff = 617;

Heartbeat myHeart = Heartbeat( LEDPIN1, (unsigned long)TIMEON, (unsigned long)TIMEOFF );
Heartbeat myBeacon1 = Heartbeat( BEACONPIN1, BCNTIMEON1, BCNTIMEOFF1 );
Heartbeat myBeacon2 = Heartbeat( BEACONPIN2, BCNTIMEON2, BCNTIMEOFF2 );

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  myBeacon1.begin( );
  myBeacon2.begin( );
  myHeart.begin( );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forver loop in main.cpp calls loop over and over
void loop( ) {
  myBeacon1.update( );
  myBeacon2.update( );
  myHeart.update( );
} // loop( )
