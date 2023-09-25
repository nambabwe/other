/*
 * Campfire class
 * Analog writes to a PWM pin to simulate a fire, when Flickering
 * Also added a fade out when asked to stop, using the flickering value decreasing max
 * 
 * Author: Speed Muller, 2023.10.07
 */

#define TRIGGERMAX 100

class Campfire {
  uint8_t       u8Pin;
  uint8_t       u8TriggerVal;
  uint8_t       u8RandomMin;
  uint8_t       u8RandomMax;
  uint8_t       u8RandomMaxUsed;
  uint8_t       u8Flickering;
  unsigned long ulTimeout;  
  unsigned long ulPrevious;

public:
  Campfire( uint8_t u8ThePin, unsigned long ulTheTimeout, uint8_t u8TheTrigger = 127, 
                uint8_t u8TheMin = 0, uint8_t u8TheMax = 255 ) {
    u8Pin = u8ThePin;
    u8TriggerVal = u8TheTrigger;
    u8RandomMin = u8TheMin;
    u8RandomMaxUsed = u8RandomMax = u8TheMax;
    ulTimeout = ulTheTimeout;
    u8Flickering = 0;
  } // Campfire( uint8_t, unsigned long, [uint8_t], [uint8_t], [uint8_t] )


  void begin( ) {    
    pinMode( u8Pin, OUTPUT );
    analogWrite( u8Pin, 0 );  // start with off
    ulPrevious = millis( );
  } // void begin( )


  void stop( ) {
    if( u8Flickering == 255 )  {
      u8Flickering = 254;
    } // if full on
    //analogWrite( u8Pin, 0 );
  } // void stop( )


  void start( ) {
    u8Flickering = 255;
    u8RandomMaxUsed = u8RandomMax;
  } // void start( )


  bool bIsHot( ) {
    return u8Flickering >= u8RandomMax / 2;
  } // bool bIsHot( ) 


  uint8_t getFlickering( ) {
    return u8Flickering;
  } // uint8_t getFlickering( ) 


  void update( ) {
    unsigned long ulNow = millis( );
    if( ulNow - ulPrevious >= ulTimeout ) {
      ulPrevious = ulNow;
      if( ( u8Flickering < 255 ) && ( u8Flickering > 0 ) ) {
        u8Flickering--;
        if( u8RandomMaxUsed > u8RandomMin ) {
          u8RandomMaxUsed--;
        } // if room to decrease
      } // if Flickering
      if( u8Flickering == 0 ) {
        analogWrite( u8Pin, 0 );
      } else {
        if( random( 0, TRIGGERMAX ) > u8TriggerVal ) {
          uint8_t u8OutputVal = random( 1 * u8RandomMin, u8RandomMaxUsed+1 );
          analogWrite( u8Pin, u8OutputVal );
        } // if triggered
      } // if Flickering
    } // if time    
  } // void update( )
}; // class Campfire

/* end of file */
