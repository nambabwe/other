/*
 * Tulsa 2021.10.08 Arduino for Beginners Make and Take Clinic
 * 
 * This is my second program: 002_better.ino
 * 
 * To flash the LED at pin 13 
 * 
 * Author: Speed Muller
 * Date: 2021.10.08, 08:00
 * 
 */

#define VERSION_STR "002_better, 2021.10.08, 08:00, ver 0.01"

#define LED       13
#define ONTIME   750
#define OFFTIME  250

void setup( ) {
  Serial.begin( 115200 );
  
  pinMode( LED, OUTPUT );
  
  Serial.println( );
  Serial.println( VERSION_STR );
} // setup( )

void loop( ) {
  digitalWrite( LED, HIGH );
  delay( ONTIME );
  
  digitalWrite( LED, LOW );
  delay( OFFTIME );
  //Serial.print( "." );
} // loop( )


//int main( ) {
//  // setup all the things Arduino needs to set up, like the a Serial port
//
//  // then call setup( )
//  setup( );
//
//  while( 1 ) {
//    // do some interrupt things to see if new code is coming
//    // yada yada
//
//    // then call loop( )
//    loop( );
//  } // while( 1 )
//
//    return -1;
//} // int main( )

// Notes: Resistors are sticky where they go into the band, cut it off
//        Every LED needs its OWN current limiting resistor, Speed rule #114
//        Open the Serial Port Monitor with Ctrl+Shift+M
//        Ctrl-u will compile and upload
