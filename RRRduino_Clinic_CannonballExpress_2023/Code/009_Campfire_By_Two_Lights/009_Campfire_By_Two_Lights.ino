#define VERSION_STR    "009_CampFire_By_Two_Lights, LongIsland, 2023.10.07"

/*
 * Campfire on PWM pins 5 and 6, with heartbeat on 13 (LED_BUILTIN)
 */

#include "Campfire.h"
#include "Heartbeat.h"

#define CAMPFIRE1PIN            5
#define CAMPFIRE2PIN            6

// How often to evaluate
#define CAMPFIRE1TIMEOUT       10
#define CAMPFIRE2TIMEOUT       15

// How often to trigger (0-100)
#define CAMPFIRE1TRIGGER       80
#define CAMPFIRE2TRIGGER       60

// Dimmest levels
#define CAMPFIRE1MIN           20
#define CAMPFIRE2MIN           10

// Brightest levels
#define CAMPFIRE1MAX          100
#define CAMPFIRE2MAX           50

Campfire *campfire1 = new Campfire( CAMPFIRE1PIN, CAMPFIRE1TIMEOUT, CAMPFIRE1TRIGGER,
                                    CAMPFIRE1MIN, CAMPFIRE1MAX ); // create the first CampfireLite object using PWM pin 5
Campfire *campfire2 = new Campfire( CAMPFIRE2PIN, CAMPFIRE2TIMEOUT, CAMPFIRE2TRIGGER,
                                    CAMPFIRE2MIN, CAMPFIRE2MAX ); // create the first CampfireLite object using PWM pin 6 

#define HEARTBEATPIN  LED_BUILTIN
#define HEARTBEATONTIME        50
#define HEARTBEATOFFTIME      617

Heartbeat  *myHeart    = new Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  campfire1->begin( );
  campfire2->begin( );
  myHeart->begin( );
} // setup


void loop( ) {
  campfire1->update( );
  campfire2->update( );
  myHeart->update( );
} // loop
