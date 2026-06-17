#define VERSION_STR    "013_StreetLight_On_Digital_Only, 2026.06.20"

/*
 * Street Light with a digital pin only
 */

#include "StreetLightNoPWM.h"

#define DIGITAL_PIN           2

StreetLight streetLight( DIGITAL_PIN );
StreetLight streetLight2( 5 );
StreetLight streetLight3( 6 );

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  streetLight.begin( );
  streetLight2.begin( );
  streetLight3.begin( );
} // void setup( )

void loop() {
  streetLight.update( );
  streetLight2.update( );
  streetLight3.update( );
} // void loop( )
