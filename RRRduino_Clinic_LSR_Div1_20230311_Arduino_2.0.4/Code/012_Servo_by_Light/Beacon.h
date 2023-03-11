/*
 * Beacon class
 */

// \___________/---\___________/---

class Beacon {
private:
  uint8_t u8Pin;
  unsigned long ulOnTimeout;
  unsigned long ulOffTimeout;
  unsigned long ulPrevious;

public:
  // Constructor
  Beacon( uint8_t u8ThePin, unsigned long ulTheOnTimeout, unsigned long ulTheOffTimeout ) {
    u8Pin = u8ThePin;
    ulOnTimeout = ulTheOnTimeout;
    ulOffTimeout = ulTheOffTimeout;
  } // constructor void Beacon( uint8_t, unsinged long, unsinged long )


  // Set the pin and time up
  void begin( ) {
    ulPrevious = millis( );
    pinMode( u8Pin, OUTPUT );
  } // begin ( )


  // Read and write the opposite
  void toggle( ) {
    digitalWrite( u8Pin, !digitalRead( u8Pin ) );
  } // void toggle( )


  // Check if it is time, and then toggle if so
  void update( ) {
    unsigned long ulNow = millis( );
    if( digitalRead( u8Pin ) ) {
      if( ulNow - ulPrevious > ulOnTimeout ) {
        ulPrevious = ulNow; 
        toggle( );
      } // if      
    } else {
      if( ulNow - ulPrevious > ulOffTimeout ) {
        ulPrevious = ulNow; 
        toggle( );
      } // if      
    } // if HIGH or LOW
  } // void update( )
}; // class Beacon

