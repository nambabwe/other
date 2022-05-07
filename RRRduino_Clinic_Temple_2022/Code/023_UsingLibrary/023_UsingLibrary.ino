#define VERSION_STR    "023_Welder is now in the Library, 2022.05.07"

/*
 * Welder in Library with Heartbeat
 */

#include "Heartbeat.h"
#include "WelderInLib.h"

#define HEARTBEATPIN         A3
#define HEARTBEATTIME       500

#define WELDERPIN            12
#define WELDERONTIME        250
#define WELDERONWHEN       HIGH

WelderInLib *myWelder   = new WelderInLib( WELDERPIN, WELDEROFFRANDOM, WELDERONWHEN );
Heartbeat   *myHeart    = new Heartbeat( HEARTBEATPIN, HEARTBEATTIME );

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  myWelder->begin( );
  myHeart->begin( );  
} // setup


void loop( ) {
  myWelder->update( );
  myHeart->update( );
} // loop

