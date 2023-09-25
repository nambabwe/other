#define VERSION_STR "010_Plot_Light, LongIsland, 2023.10.07"

/*
 * The light into the resistive light sensor (10k resistor to GND)
 * reported and plotted
 * 
 * Author: Speed Muller, Date: 2023.03.04, 15:23
 */

#include "Heartbeat.h"
#include "Light.h"

#define BAUD               115200

// heartbeat
#define HEARTBEATPIN  LED_BUILTIN
#define HEARTBEATONTIME        50
#define HEARTBEATOFFTIME      617

// Light sensor pin
#define ANAPIN                 A0
// 10-bit A/D, 0-1023

#define DELAY                 125

#define TIMEOUT               100

Heartbeat  *myHeart    = new Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

unsigned long now, before;
uint16_t analogValue;


void setup( ) {
  Serial.begin( BAUD );

  pinMode( ANAPIN, INPUT );

  myHeart->begin( );  

  Serial.println( );
  Serial.println( VERSION_STR );

  now = before = millis( );
} // setup( )


void loop( ) {
  now = millis( );
  if( now - before > TIMEOUT ) {
    before = now;

    analogValue = analogRead( ANAPIN );
    Serial.print( "Light:" ); Serial.println( analogValue );
  } // if time to read sensor

  myHeart->update( );
} // loop( )
