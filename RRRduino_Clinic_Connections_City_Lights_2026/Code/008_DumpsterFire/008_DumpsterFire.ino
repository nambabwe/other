#define VERSION_STR    "008_DumpsterFire, 2026.06.20"

/*
 * Dumpster fire on PWM pin 5, with heartbeat on 13 (LED_BUILTIN)
 */

#include "DumpsterFire.h"
#include "Heartbeat.h"

#define DUMPSTERFIREPIN             5

// How often to evaluate
#define DUMPSTERFIRETIMEOUT        10

// How often to trigger (0-100)
#define DUMPSTERFIRETRIGGER        80

// Dimmest level
#define DUMPSTERFIREMIN            20

// Brightest level
#define DUMPSTERFIREMAX           100

DumpsterFire dumpsterFire = DumpsterFire( DUMPSTERFIREPIN, DUMPSTERFIRETIMEOUT, DUMPSTERFIRETRIGGER,
                              DUMPSTERFIREMIN, DUMPSTERFIREMAX ); // create the first dumpsterFire object using PWM pin 5

#define HEARTBEATPIN  LED_BUILTIN
#define HEARTBEATONTIME        50
#define HEARTBEATOFFTIME      617

Heartbeat  myHeart    = Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  dumpsterFire.begin( );
  myHeart.begin( );
} // setup


void loop( ) {
  dumpsterFire.update( );
  myHeart.update( );
} // loop
