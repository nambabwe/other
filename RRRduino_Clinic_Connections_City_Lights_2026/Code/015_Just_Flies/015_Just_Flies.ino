#define VERSION_STR    "015_Just_Flies, 2026.06.20"

/*
 * Just flies...with pointers
 */


#include "Heartbeat.h"
#include "FireFly.h"

// Firefly settings
#define FLIES                        6
#define FLYOFFDELAYMIN             500
#define FLYOFFDELAYMAX           15000
#define FLYMIN                      50

const uint8_t FIREFLYPINS[FLIES] = { 3, 5, 6, 9, 10, 11 };

FireFly      *myFlies[FLIES];
//FireFly      myFly1      = FireFly( FIREFLY1PIN, FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );
//FireFly      myFly2      = FireFly( FIREFLY2PIN, FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );
//FireFly      myFly3      = FireFly( FIREFLY3PIN, FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );
//FireFly      myFly4      = FireFly( FIREFLY4PIN, FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );
//FireFly      myFly5      = FireFly( FIREFLY5PIN, FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );
//FireFly      myFly6      = FireFly( FIREFLY6PIN, FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );

#define HEARTBEATPIN       LED_BUILTIN
#define HEARTBEATONTIME             50
#define HEARTBEATOFFTIME           617

Heartbeat  myHeart    = Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  myHeart.begin( );
  for( uint8_t i=0; i<FLIES; i++ ) {
    myFlies[i] = new FireFly( FIREFLYPINS[i], FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );
    myFlies[i]->begin( );
  } // for
  // myFly2.begin( );
  // myFly3.begin( );
  // myFly4.begin( );
  // myFly5.begin( );
  // myFly6.begin( );

  randomSeed( analogRead( 0 ) );
} // void setup( )

void loop() {
  myHeart.update( );
  for( uint8_t i=0; i<FLIES; i++ ) {
    myFlies[i]->update( );
  } // for
  // myFly1.update( );
  // myFly2.update( );
  // myFly3.update( );
  // myFly4.update( );
  // myFly5.update( );
  // myFly6.update( );
} // void loop( )
