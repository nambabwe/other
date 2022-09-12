/*
 * Tulsa 2021.10.08 Arduino for Beginners Make and Take Clinic
 * 
 * To flash three LEDs, at pin 13, pin 12 and pin 11 in sequence
 * 
 * Author: Speed Muller, Date: 2021.10.08, 08:04
 */

#define VERSION_STR "004_sequence, 2021.10.08, 08:04, ver 0.01"

#define LED1       13
#define LED2       12
#define LED3       11

#define ONTIME1   250
#define ONTIME2   250
#define ONTIME3   250

void setup( ) {
  Serial.begin( 115200 );
  pinMode( LED1, OUTPUT );
  pinMode( LED2, OUTPUT );
//  pinMode( LED3, OUTPUT );
  Serial.println( );
  Serial.println( VERSION_STR );
} // setup( )

void loop( ) {
  digitalWrite( LED1, HIGH );
  delay( ONTIME1 );
  digitalWrite( LED1, LOW );
  digitalWrite( LED2, HIGH );
  delay( ONTIME2 );
  digitalWrite( LED2, LOW );
  digitalWrite( LED3, HIGH );
  delay( ONTIME3 );
  digitalWrite( LED3, LOW );
  //Serial.print( "." );
} // loop( )

// Pins can also sink current, so the LED can be connected to VCC (5V)
// and the other end to the pin, and when the pin goes low, the LED turns on!
