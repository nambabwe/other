#define VERSION_STR    "002_BlinkFaster, St Louis, 2022.08.09"

/*
 * Blink by setting the ONTIME and the OFFTIME separately
 *  
 * Pin 13 will do:
 *    \_________________/----  ,  \_________________/----  ,  \__
 *    x                 x      ,  x                 x      ,  x    
 *    
 *    
 * Author: Gert 'Speed' Muller, 2022.08.09
 */
 
  #define LEDPIN          13
  #define OFFTIME       1900 
  #define ONTIME         100

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  pinMode( LEDPIN, OUTPUT );
  // digitalWrite( LEDPIN, HIGH );  removed to contol ALL pin timing in loop( )
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  digitalWrite( LEDPIN, LOW );
  Serial.println( "Off" );
  delay( OFFTIME );
  
  digitalWrite( LEDPIN, HIGH );
  Serial.println( "On" );
  delay( ONTIME );
} // loop( )
