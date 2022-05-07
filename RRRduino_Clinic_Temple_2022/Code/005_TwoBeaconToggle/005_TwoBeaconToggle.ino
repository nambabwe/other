#define VERSION_STR    "005_Temple Two Beacon Toggle, 2022.05.07"

/*
 * Two beacons with printing
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

  // to toggle, we need to set it to known state first
  // HIGH helps to show it works on startup
  digitalWrite( LEDPIN1, HIGH );
  digitalWrite( LEDPIN2, HIGH );

  delay( 1000 );
} // setup( )


// Read the pin and write the opposite
void toggle( uint8_t u8ThePin ) {
  // digitalWrite( u8ThePin, !digitalRead( u8ThePin ) );

  bool bPinState = digitalRead( u8ThePin );
#if 1
  digitalWrite( u8ThePin, !bPinState );
  if( bPinState ) {
    Serial.print( "On " );
  } else {
    Serial.print( "Off " );
  }
  // we don't know the name of the light, pin # works too:
  Serial.println( u8ThePin, DEC );
#endif
} // void toggle( uint8_t )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  toggle( LEDPIN1 );
  delay( TIME1 );
  toggle( LEDPIN2 );
  delay( TIME2 );
  toggle( LEDPIN1 );
  delay( TIME3 );
  toggle( LEDPIN2 );
  delay( TIME4 );
} // loop( )

