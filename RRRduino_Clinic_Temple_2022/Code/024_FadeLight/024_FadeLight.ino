#define VERSION_STR    "024_FadeLite, 2022.05.07"

//
// FadeLite, by Speed
//
// FadeLite on an analog pin, using a PWM pin with analogWrite
//

#import "FadeLite.h"

#define BAUD        115200

// FADEPIN needs to be a PWM capable one: 3, 5, 6, 9, 10 or 11
#define FADEPIN          9
#define TOGGLEEVERY   2000

FadeLite lite1( FADEPIN, 300 );

unsigned long ulNow;
unsigned long ulBefore;

void setup( ) {
  Serial.begin( BAUD );
  Serial.println( );
  Serial.println( VERSION_STR );

  ulBefore = millis( );
} // setup( )


void loop( ) {
  lite1.update( );
  ulNow = millis( );

  if ( ulNow - ulBefore > TOGGLEEVERY ) {
    lite1.toggle( );
    ulBefore = ulNow;
  } // if
} // loop( )

