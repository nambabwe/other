#define VERSION_STR    "012_CampFire_By_Two_When_Dark, ZoomTRAK, 2023.10.28"

/*
 * Campfire on PWM pins 5 and 6, with heartbeat
 */

#include "Campfire.h"
#include "Heartbeat.h"

// Light sensor pin
#define LIGHTSENSORPIN      A0
// 10-bit A/D, 0-1023

#define CAMPFIRE1PIN         5
#define CAMPFIRE2PIN         6

// How often to evaluate
#define CAMPFIRE1TIMEOUT    10
#define CAMPFIRE2TIMEOUT    15

// How often to trigger (0-100)
#define CAMPFIRE1TRIGGER    80
#define CAMPFIRE2TRIGGER    60

// Dimmest levels
#define CAMPFIRE1MIN        20
#define CAMPFIRE2MIN        10

// Brightest levels
#define CAMPFIRE1MAX       100
#define CAMPFIRE2MAX        50

// How often to check light sensor
#define DARKTIMECHECK      250

// Below what light level to start fire
#define LIGHTLEVEL4FIRE    400

Campfire campfire1 = Campfire( CAMPFIRE1PIN, CAMPFIRE1TIMEOUT, CAMPFIRE1TRIGGER,
                               CAMPFIRE1MIN, CAMPFIRE1MAX ); // create the first CampfireLite object using PWM pins 9 
Campfire campfire2 = Campfire( CAMPFIRE2PIN, CAMPFIRE2TIMEOUT, CAMPFIRE2TRIGGER,
                               CAMPFIRE2MIN, CAMPFIRE2MAX ); // create the first CampfireLite object using PWM pins 9 

#define HEARTBEATPIN  LED_BUILTIN
#define HEARTBEATONTIME        50
#define HEARTBEATOFFTIME      617

Heartbeat  myHeart    = Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

unsigned long ulNow, ulBefore;
uint16_t analogValue;

void setup( ) {  
  Serial.begin( 115200 );
  pinMode( LIGHTSENSORPIN, INPUT );

  campfire1.begin( );
  campfire2.begin( );
  myHeart.begin( );

  Serial.println( );
  Serial.println( VERSION_STR );

  Serial.println( "light, smores" );

  ulNow = ulBefore = millis( );
} // setup


void stopFire( ) {
  campfire1.stop( );
  campfire2.stop( );
} // void stopFire( ) 


void startFire( ) {
  campfire1.start( );
  campfire2.start( );
} // void startFire( ) 


void loop( ) {
  ulNow = millis( );

  if( ulNow - ulBefore > DARKTIMECHECK ) {
    ulBefore = ulNow;
    analogValue = analogRead( LIGHTSENSORPIN );
    Serial.print( "light:" );
    Serial.print( analogValue );

    if( analogValue <= LIGHTLEVEL4FIRE ) {
      startFire( );
    } else {
      stopFire( );
    } // if dark enough
    
    Serial.print( ", smores:" );
    if( campfire1.bIsRunning( ) ) {
      Serial.println( "100" );
    } else {
      Serial.println( "0" );
    } // if running
  }
  campfire1.update( );
  campfire2.update( );
  myHeart.update( );
} // loop
