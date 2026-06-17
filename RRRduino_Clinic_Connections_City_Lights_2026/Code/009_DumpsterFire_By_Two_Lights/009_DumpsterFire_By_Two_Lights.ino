#define VERSION_STR    "009_DumpsterFire_By_Two_Lights, 2026.06.20"

/*
 * Dumpster fire on PWM pin 5, with heartbeat on 13 (LED_BUILTIN)
 */

#include "DumpsterFire.h"
#include "Heartbeat.h"

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

#define HEARTBEATPIN      LED_BUILTIN
#define HEARTBEATONTIME            50
#define HEARTBEATOFFTIME          617

Heartbeat  myHeart    = Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  dumpsterFire1.begin( );
  dumpsterFire2.begin( );
  myHeart.begin( );
} // void setup( )


void loop( ) {
  dumpsterFire1.update( );
  dumpsterFire2.update( );
  myHeart.update( );
} // void loop( )
