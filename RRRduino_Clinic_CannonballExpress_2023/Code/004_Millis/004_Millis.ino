#define VERSION_STR    "004_Millis, LongIsland, 2023.10.07"

/*
 * No more use of the blocking delay call in loop( )
 */

// \_____/-----\_____/-----

#define LEDPIN1           5
#define TIME1           500

unsigned long ulPrevious, ulNow = 0;

// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( VERSION_STR );

  pinMode( LEDPIN1, OUTPUT );

  // to toggle, we need to set it to known state first
  // HIGH helps to show it works on startup
  digitalWrite( LEDPIN1, HIGH );
  delay( 1000 );
} // setup( )


// Read the pin and write the opposite
void toggle( uint8_t u8ThePin ) {
  digitalWrite( u8ThePin, !digitalRead( u8ThePin ) );
} // void toggle( uint8_t )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  ulNow = millis( );

  if( ulNow - ulPrevious > TIME1 ) {
    // very important to update ulBefore for next time
    ulPrevious = ulNow;
    toggle( LEDPIN1 )
  } // if
} // loop( )
