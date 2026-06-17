#define VERSION_STR    "011_TV_and_Dumpster, 2026.06.20"
/*
 * TV
 * Drives one or two sets of RGB LEDs to mimic the flickering glow of a television.
  * Dumpster fire on PWM pin 5, with heartbeat on 13 (LED_BUILTIN)
 */

#include "TV_3Led.h"
#include "DumpsterFire.h"
#include "Heartbeat.h"
#include "FireFly.h"

// Firefly settings

#define FIREFLY1PIN                  3

#define FLYOFFDELAYMIN             500
#define FLYOFFDELAYMAX           15000
#define FLYMIN                      50

FireFly      myFly      = FireFly( FIREFLY1PIN, FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );

#define DUMPSTERFIRE1PIN             5
#define DUMPSTERFIRE2PIN             6

// How often to evaluate
#define DUMPSTERFIRE1TIMEOUT        10
#define DUMPSTERFIRE2TIMEOUT        15

// How often to trigger (0-100)
#define DUMPSTERFIRE1TRIGGER        80
#define DUMPSTERFIRE2TRIGGER        60

// Dimmest level
#define DUMPSTERFIRE1MIN            20
#define DUMPSTERFIRE2MIN            10

// Brightest level
#define DUMPSTERFIRE1MAX           100
#define DUMPSTERFIRE2MAX            50

DumpsterFire dumpsterFire1 = DumpsterFire( DUMPSTERFIRE1PIN, DUMPSTERFIRE1TIMEOUT, DUMPSTERFIRE1TRIGGER,
                              DUMPSTERFIRE2MIN, DUMPSTERFIRE1MAX ); // create the first dumpsterFire object using PWM pin 5
DumpsterFire dumpsterFire2 = DumpsterFire( DUMPSTERFIRE2PIN, DUMPSTERFIRE2TIMEOUT, DUMPSTERFIRE2TRIGGER,
                              DUMPSTERFIRE2MIN, DUMPSTERFIRE2MAX ); // create the second dumpsterFire object using PWM pin 6

#define HEARTBEATPIN       LED_BUILTIN
#define HEARTBEATONTIME             50
#define HEARTBEATOFFTIME           617

Heartbeat  myHeart    = Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

#define TV_R_PIN                     9
#define TV_G_PIN                    10
#define TV_B_PIN                    11

// TVLed( int rPin, int gPin, int bPin )
TVLed tv1( TV_R_PIN, TV_G_PIN, TV_B_PIN );
// TVLed tv2( 3, 5, 6 );

void setup( ) {
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  dumpsterFire1.begin( );
  dumpsterFire2.begin( );
  myHeart.begin( );
  myFly.begin( );

  randomSeed( analogRead( 0 ) );

  tv1.begin( );
//  tv2.begin( );
} // void setup( )


void loop( ) {
  dumpsterFire1.update( );
  dumpsterFire2.update( );
  myHeart.update( );
  myFly.update( );

  tv1.update( );
//  tv2.update( );
} // void loop( )
