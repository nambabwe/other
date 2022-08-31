/*
 * Tulsa 2021.10.08 Arduino for Beginners Make and Take Clinic
 * 
 * To flash two LEDs, at pin 13 and pin 12 alternating
 * 
 * Author: Speed Muller
 * Date: 2021.10.08, 08:03
 */

#define VERSION_STR "003_more, 2021.10.08, 08:03, ver 0.01"

#define LED1       13
#define LED2       12

#define ONTIME   500
#define OFFTIME  500

void setup( ) {
  Serial.begin( 115200 );
  
  pinMode( LED1, OUTPUT );
  pinMode( LED2, OUTPUT );
  
  Serial.println( );
  Serial.println( VERSION_STR );
} // setup( )

void loop( ) {
  digitalWrite( LED1, HIGH );
  digitalWrite( LED2, LOW );
  delay( ONTIME );
  
  digitalWrite( LED1, LOW );
  digitalWrite( LED2, HIGH );
  delay( OFFTIME );
  //Serial.print( "." );
} // loop( )

//
// You could also just place the 2nd LED between Vcc and pin 13 to get the same effect.
// In other words, when pin 13 is LOW, LED2 is on and then HIGH, LED1 is on instead.
//
