/*
 * Heartbeat class
 */

// \_____/-----\_____/-----

class Heartbeat {
private:
  uint8_t u8Pin;
  unsigned long ulTimeout;
  unsigned long ulOriginalTimeout;  
  unsigned long ulPrevious;

public:
  // Constructor
  Heartbeat( uint8_t u8ThePin, unsigned long ulTheTimeout ) {
    u8Pin = u8ThePin;
    ulTimeout = ulOriginalTimeout = ulTheTimeout;
  } // constructor void Heartbeat( uint8_t, unsinged long )


  // Set the pin and time up
  void begin( ) {
    ulPrevious = millis( );
    pinMode( u8Pin, OUTPUT );
  } // begin ( )


  // Read and write the opposite
  void toggle( ) {
    digitalWrite( u8Pin, !digitalRead( u8Pin ) );
  } // void toggle( )


  // Read and write the opposite
  void doubleBeat( bool bTheDoubleIsOn ) {
    if( bTheDoubleIsOn ) {
      ulTimeout = ulOriginalTimeout / 4;
    } else {
      ulTimeout = ulOriginalTimeout;
    } // if
  } // void doubleBeat( bool )

  
  // Check if it is time, and then toggle if so
  void update( ) {
    unsigned long ulNow = millis( );
    if( ulNow - ulPrevious > ulTimeout ) {
      ulPrevious = ulNow; 
      toggle( );
    } // if
  } // void update( )
}; // class Heartbeat

