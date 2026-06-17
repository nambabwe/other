/*
 * TV
 * Drives two sets of RGB LEDs to mimic the flickering glow of a television.
 */

#include <Arduino.h>
#include "TVLed.h"

TVLed tv1( 3, 5, 6 );
TVLed tv2( 9, 10, 11 );


/*
 * setup:
 */
void setup( ) {
  randomSeed( analogRead( 0 ) );

  tv1.begin( );
  tv2.begin( );
} // void setup( )


/*
 * loop:
 */
void loop( ) {
  tv1.update( );
  tv2.update( );
} // void loop( )
