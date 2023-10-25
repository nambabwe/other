/*
 * Campfire class
 * Analog writes to a PWM pin to simulate a fire, when running
 * 
 * Author: Speed Muller, 2023.10.07
 */

#define TRIGGERMAX 100

class Campfire {
  uint8_t       u8Pin;
  uint8_t       u8TriggerVal;
  uint8_t       u8RandomMin;
  uint8_t       u8RandomMax;
  uint8_t       u8Running;
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
    u8Running = false;
  } // Campfire( uint8_t, unsigned long, [uint8_t], [uint8_t], [uint8_t] )


  void begin( ) {    
    pinMode( u8Pin, OUTPUT );
    analogWrite( u8Pin, 0 );  // start with off
    ulPrevious = millis( );
  } // void begin( )


  void stop( ) {
    u8Running = false;
    analogWrite( u8Pin, 0 );
  } // void stop( )


  void start( ) {
    u8Running = true;
  } // void start( )


  bool bIsRunning( ) {
    return u8Running;
  } // void bIsRunning( ) 


  void update( ) {
    unsigned long ulNow = millis( );
    if( ulNow - ulPrevious >= ulTimeout ) {
      ulPrevious = ulNow;
      if( u8Running == 0 ) {
        analogWrite( u8Pin, 0 );
      } else {
        if( random( 0, TRIGGERMAX ) > u8TriggerVal ) {
          analogWrite( u8Pin, random( u8RandomMin, u8RandomMax ) );
        } // if triggered
      } // if running
    } // if time    
  } // void update( )
}; // class Campfire
