#define VERSION_STR    "018_Temple CampFire, 2022.05.07"

/*
 * Campfire on PWM pin, with heartbeat
 */

#include "CampfireLite.h"

#define CAMPFIREPIN         10

// How often to evaluate
#define CAMPFIRETIMEOUT     10

// How often to trigger (0-100)
#define CAMPFIRETRIGGER     80

// Dimmest level
#define CAMPFIREMIN         20

// Brightest level
#define CAMPFIREMAX        100

CampfireLite *campfire = new CampfireLite( CAMPFIREPIN, CAMPFIRETIMEOUT, CAMPFIRETRIGGER,
                                           CAMPFIREMIN, CAMPFIREMAX ); // create the first CampfireLite object using PWM pins 9 

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  campfire->begin( );
} // setup


void loop( ) {
  campfire->update( );
} // loop
