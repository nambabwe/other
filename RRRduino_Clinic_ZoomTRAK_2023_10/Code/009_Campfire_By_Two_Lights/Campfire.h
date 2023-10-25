/*
 * Campfire class
 * Analog writes to a PWM pin to simulate a fire
 * 
 * Author: Speed Muller, 2023.10.07
 */

#define TRIGGERMAX 100

class Campfire {
  uint8_t       u8Pin;
  uint8_t       u8TriggerVal;
  uint8_t       u8RandomMin;
  uint8_t       u8RandomMax;
  unsigned long ulTimeout;  
  unsigned long ulPrevious;

public:
  Campfire( uint8_t u8ThePin, unsigned long ulTheTimeout, uint8_t u8TheTrigger = 127, 
                uint8_t u8TheMin = 0, uint8_t u8TheMax = 255 ) {
    u8Pin = u8ThePin;
    u8TriggerVal = u8TheTrigger;
    u8RandomMin = u8TheMin;
    u8RandomMax = u8TheMax;
    ulTimeout = ulTheTimeout;
  } // Campfire( uint8_t, unsigned long, [uint8_t], [uint8_t], [uint8_t] )


  void begin( ) {    
    pinMode( u8Pin, OUTPUT );
    analogWrite( u8Pin, 0 );  // start with off
    ulPrevious = millis( );
  } // void begin( )


  void update( ) {
    unsigned long ulNow = millis( );
    if( ulNow - ulPrevious >= ulTimeout ) {
      ulPrevious = ulNow;
      if( random( 0, TRIGGERMAX ) > u8TriggerVal ) {
        analogWrite( u8Pin, random( u8RandomMin, u8RandomMax ) );
      } // if triggered
    } // if time    
  } // void update( )
}; // class Campfire
