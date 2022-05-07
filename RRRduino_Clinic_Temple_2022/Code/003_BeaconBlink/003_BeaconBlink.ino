#define VERSION_STR    "003_Temple Beacon Blink, 2022.05.07"

/*
 * A beacon can be implemented with an ON time and an OFF time
 */

// \_________________/----  ,  \_________________/----  ,  \____
// x                 x      ,  x                 x      ,  x    

#define LEDPIN1         13
#define TIMEON          50
#define TIMEOFF       1850

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  pinMode( LEDPIN1, OUTPUT );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  digitalWrite( LEDPIN1, LOW );
  Serial.println( "Off 1" );
  delay( TIMEOFF );

  digitalWrite( LEDPIN1, HIGH );
  Serial.println( "On 1" );
  delay( TIMEON );
} // loop( )

