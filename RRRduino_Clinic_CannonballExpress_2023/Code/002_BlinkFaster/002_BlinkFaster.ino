#define VERSION_STR    "002_BlinkFaster, LongIsland, 2023.10.07"

/*
 * Blink by setting the ONTIME and the OFFTIME separately
 *    
 * Author: Speed Muller, 2023.10.07
 */

// \_________________/----  ,  \_________________/----  ,  \__
// x                 x      ,  x                 x      ,  x

#define LEDPIN    LED_BUILTIN
#define ONTIME            100
#define OFFTIME          1900

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  pinMode( LEDPIN, OUTPUT );
  // digitalWrite( LEDPIN, HIGH );  removed to contol all timing in loop( )
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
