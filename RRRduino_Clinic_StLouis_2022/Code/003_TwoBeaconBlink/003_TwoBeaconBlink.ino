#define VERSION_STR    "003_TwoBeaconBlink, St Louis, 2022.08.09"

/*
 * Making two beacons work! Need external LED and resistor on pin A5
 */
 
// \_________________/----  ,  \_________________/----  ,  \____
// --\_________________/--  ,  --\_________________/--  ,  --\__
// x x               x x    ,  x x               x x    ,  x x  
 
#define LEDPIN1         13
#define LEDPIN2         A5
#define TIME1          250
#define TIME2         1850
#define TIME3          250
#define TIME4           50

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  pinMode( LEDPIN1, OUTPUT );
  pinMode( LEDPIN2, OUTPUT );
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  digitalWrite( LEDPIN1, LOW );
  Serial.println( "Off 1" );
  delay( TIME1 );

  digitalWrite( LEDPIN2, LOW );
  Serial.println( "Off 2" );
  delay( TIME2 );

  digitalWrite( LEDPIN1, HIGH );
  Serial.println( "On 1" );
  delay( TIME3 );

  digitalWrite( LEDPIN2, HIGH );
  Serial.println( "On 2" );
  delay( TIME4 );
} // loop( )
