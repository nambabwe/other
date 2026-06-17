#define VERSION_STR    "010_TV_By_RGB, 2026.06.20"
/*
 * TV
 * Drives one or two sets of RGB LEDs to mimic the flickering glow of a television.
 */

#include "TV_3Led.h"

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

  randomSeed( analogRead( 0 ) );

  tv1.begin( );
//  tv2.begin( );
} // void setup( )


void loop( ) {
  tv1.update( );
//  tv2.update( );
} // void loop( )
