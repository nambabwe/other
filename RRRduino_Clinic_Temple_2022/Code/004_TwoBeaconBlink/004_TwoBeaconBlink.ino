#define VERSION_STR    "004_Temple Two Beacon Blink, 2022.05.07"

/*
 * Making two beacons work! Need external LEDs on pins 11 and 12
 */
 
// \_________________/----  ,  \_________________/----  ,  \____
// --\_________________/--  ,  --\_________________/--  ,  --\__
// x x               x x    ,  x x               x x    ,  x x  
 
#define LEDPIN1         12
#define LEDPIN2         11
#define TIME1           50
#define TIME2         1850
#define TIME3           50
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

