/*
 * Heartbeat class
 * Tracks State to determine to wait for the TimeoutOn to expire, or the TimeoutOff to expire.
 * 
 * __off__/--on--, \__off__/--on--, \__
 * 
 *    
 * Author: Speed Muller, 2023.10.07
 */

class Heartbeat {
private:
  uint8_t u8Pin;
  bool bPinHigh;
  unsigned long ulTimeoutOn;
  unsigned long ulTimeoutOff;
  unsigned long ulPrevious;

public:
  // Constructor
  Heartbeat( uint8_t u8ThePin, unsigned long ulTheTimeoutOn, unsigned long ulTheTimeoutOff ) {
    u8Pin = u8ThePin;
    ulTimeoutOn  = ulTheTimeoutOn;
    ulTimeoutOff = ulTheTimeoutOff;
    bPinHigh = false;
  } // constructor void Heartbeat( uint8_t, unsigned long, unsigned long )


  // Set the pin and time up
  void begin( ) {
    ulPrevious = millis( );
    pinMode( u8Pin, OUTPUT );
    writeLo( );                   // let's begin with off
  } // begin ( )


  // Write HI
  void writeHi( ) {
    digitalWrite( u8Pin, HIGH );
    bPinHigh = true;
  } // void writeHi( )


  // Read and write the opposite
  void writeLo( ) {
    digitalWrite( u8Pin, LOW );
    bPinHigh = false;
  } // void writeLo( )


  // Read and write the opposite
  void toggle( ) {
    digitalWrite( u8Pin, !digitalRead( u8Pin ) );
  } // void toggle( )


  // Check state, and then if it is time, and then toggle if so
  void update( ) {
    unsigned long ulNow = millis( );
    if( bPinHigh ) {
      if( ulNow - ulPrevious > ulTimeoutOn ) {
        ulPrevious = ulNow; 
        writeLo( );
      } // if
    } else {
      if( ulNow - ulPrevious > ulTimeoutOff ) {
        ulPrevious = ulNow; 
        writeHi( );
      } // if      
    }
  } // void update( )
}; // class Heartbeat
