// \__________/----------\__________/----------\__

// We want the pre-processor to replace all LEDPIN with 13
// so we only have to replace it in one place
#define LEDPIN             13

// You will also see: 
// const uint8_t u8LedPin = 8;
// If you did care that the replacement does not violate a type mismatch in during compilation


// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );
  Serial.println( "001_Temple Blink Slow, 2022.05.07" );

  pinMode( LEDPIN, OUTPUT );         // make the pin an OUTPUT, driving something
  digitalWrite( LEDPIN, HIGH );      // make the pin HIGH (5V in the UNO and Nano cases)
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  delay( 2000 );                    // wait 2,000 milliseconds by blocking the mcu 
  digitalWrite( LEDPIN, LOW );      // make the pin LOW  (0V in all Arduino cases)
  delay( 2000 );                    // wait 2,000 milliseconds by blocking the mcu 
  digitalWrite( LEDPIN, HIGH );     // make the pin HIGH (5V in the UNO and Nano cases)
} // loop( )
