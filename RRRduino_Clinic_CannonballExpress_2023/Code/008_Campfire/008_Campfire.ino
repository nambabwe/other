#define VERSION_STR    "008_CampFire, LongIsland, 2023.10.07"

/*
 * Campfire on PWM pin 5, with heartbeat on 13 (LED_BUILTIN)
 */

#include "Campfire.h"
#include "Heartbeat.h"

#define CAMPFIREPIN             5

// How often to evaluate
#define CAMPFIRETIMEOUT        10

// How often to trigger (0-100)
#define CAMPFIRETRIGGER        80

// Dimmest level
#define CAMPFIREMIN            20

// Brightest level
#define CAMPFIREMAX           100

Campfire *campfire = new Campfire( CAMPFIREPIN, CAMPFIRETIMEOUT, CAMPFIRETRIGGER,
                                       CAMPFIREMIN, CAMPFIREMAX ); // create the first CampfireLite object using PWM pin 5

#define HEARTBEATPIN  LED_BUILTIN
#define HEARTBEATONTIME        50
#define HEARTBEATOFFTIME      617

Heartbeat  *myHeart    = new Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  campfire->begin( );
  myHeart->begin( );
} // setup


void loop( ) {
  campfire->update( );
  myHeart->update( );
} // loop
