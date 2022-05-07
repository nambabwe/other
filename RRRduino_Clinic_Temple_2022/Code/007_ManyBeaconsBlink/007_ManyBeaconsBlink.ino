#define VERSION_STR    "007_Temple Many Beacons Blink, 2022.05.07"

/*
 * Making many beacons work! Need external LEDs on pins 11, 12 and A3
 */
 
// \_________________/----  ,  \_________________/----  ,  \______
// --\_________________/--  ,  --\_________________/--  ,  --\____
// ----\_________________/  ,  ----\_________________/  ,  ----\__
// _________/----\________  ,  _________/----\________  ,  _______
// x x x    x    x   x x x  ,  x x x    x    x   x x x  ,  x x x  
 
#define LEDPIN1         12
#define LEDPIN2         11
#define LEDPIN3         A3
#define LEDPIN4         13

#define TIME1           50
#define TIME2           50
#define TIME3          200
#define TIME4           50
#define TIME5         1550
#define TIME6           50
#define TIME7           50
#define TIME8           12

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  pinMode( LEDPIN1, OUTPUT );
  pinMode( LEDPIN2, OUTPUT );
  pinMode( LEDPIN3, OUTPUT );
  pinMode( LEDPIN4, OUTPUT );
  
  // to toggle, we need to set it to known state first
  // HIGH helps to show it works on startup
  digitalWrite( LEDPIN1, HIGH );
  digitalWrite( LEDPIN2, HIGH );
  digitalWrite( LEDPIN3, HIGH );
  digitalWrite( LEDPIN4, LOW );

} // setup( )


// Read the pin and write the opposite
void toggle( uint8_t u8ThePin ) {
  // digitalWrite( u8ThePin, !digitalRead( u8ThePin ) );

  bool bPinState = digitalRead( u8ThePin );
#if 0
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
  digitalWrite( LEDPIN1, LOW );
  delay( TIME1 );
  
  digitalWrite( LEDPIN2, LOW );
  delay( TIME2 );
  
  digitalWrite( LEDPIN3, LOW );
  delay( TIME3 );
  
  digitalWrite( LEDPIN4, HIGH );
  delay( TIME4 );
  
  digitalWrite( LEDPIN4, LOW );
  delay( TIME5 );
  
  digitalWrite( LEDPIN1, HIGH );
  delay( TIME6 );
  
  digitalWrite( LEDPIN2, HIGH );
  delay( TIME7 );
  
  digitalWrite( LEDPIN3, HIGH );
  delay( TIME8 );
} // loop( )

