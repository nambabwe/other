#define VERSION_STR    "019_Temple CampFire by Two lights, 2022.05.07"

/*
 * Campfire on PWM pins 9 and 10, with heartbeat
 */

#include "CampfireLite.h"

#define CAMPFIRE1PIN         9
#define CAMPFIRE2PIN        10

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

CampfireLite *campfire1 = new CampfireLite( CAMPFIRE1PIN, CAMPFIRE1TIMEOUT, CAMPFIRE1TRIGGER,
                                           CAMPFIRE1MIN, CAMPFIRE1MAX ); // create the first CampfireLite object using PWM pins 9 
CampfireLite *campfire2 = new CampfireLite( CAMPFIRE2PIN, CAMPFIRE2TIMEOUT, CAMPFIRE2TRIGGER,
                                           CAMPFIRE2MIN, CAMPFIRE2MAX ); // create the first CampfireLite object using PWM pins 9 

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  campfire1->begin( );
  campfire2->begin( );
} // setup


void loop( ) {
  campfire1->update( );
  campfire2->update( );
} // loop

