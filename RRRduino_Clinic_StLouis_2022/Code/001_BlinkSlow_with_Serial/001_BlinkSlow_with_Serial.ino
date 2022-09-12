/*
 * 001_StLouis BlinkSlow_with_Serial, 2022.08.09
 *
 * Pin 13 will do: \____2s____/----2s----,  \____2s____/----2s----,  \__
 *    
 *    
 * Author: Gert 'Speed' Muller, 2022.08.09
 */

// We want the pre-processor to replace all LEDPIN with 13
// so we only have to replace it in one place
#define LEDPIN               13

// You will also see: 
// const uint8_t u8LedPin =   8;
// If you did care that the replacement does not violate a type mismatch in during compilation


// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  Serial.begin( 115200 ); // Set the serial port baud rate, do the same in the serial port monitor
  Serial.println( );      // Open Serial Monitor to see: Ctrl+Shift+M
  Serial.println( "001_BlinkSlow_with_Serial, St Louis, 2022.08.09" );

  pinMode( LEDPIN, OUTPUT );         // make the pin an OUTPUT, driving something
  digitalWrite( LEDPIN, HIGH );      // make the pin HIGH (5V in the UNO and Nano cases, 3.3V on some others)
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  delay( 2000 );                    // wait 2,000 milliseconds by blocking the mcu 
  digitalWrite( LEDPIN, LOW );      // make the pin LOW  (0V in all Arduino cases)
  delay( 2000 );                    // wait 2,000 milliseconds by blocking the mcu 
  digitalWrite( LEDPIN, HIGH );     // make the pin HIGH (5V in the UNO and Nano cases)
} // loop( )
