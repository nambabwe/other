#define BIGVALUE          6000
#define CIRCBUFSIZE          4

class DistanceSensor {

private:
  uint8_t u8TrigPin;
  uint8_t u8EchoPin;
  uint32_t u32Previous;
  uint32_t u32Timeout;
  uint32_t u32Duration;
//  uint32_t u32Distance;
  uint32_t u32CircDuration[CIRCBUFSIZE] = { 0 };
  uint8_t u8HeadPtr = 0;
  
public:
  /*
   * Constructor
   */
  DistanceSensor( uint8_t u8TheTrigPin, uint8_t u8TheEchoPin, uint32_t u32TheTimeout  ) {
    u8TrigPin  = u8TheTrigPin;
    u8EchoPin = u8TheEchoPin;
    u32Timeout = u32TheTimeout;
  } // constructor void DistanceSensor( uint8_t, unsigned long, unsigned long )


  /*
   * Set the pins and default values
   */
  void begin( ) {
    pinMode( u8TrigPin, OUTPUT ); // Sets the trigPin1 as an OUTPUT
    pinMode( u8EchoPin, INPUT ); // Sets the echoPin1 as an INPUT
    u32Previous = millis( );
    for( uint8_t i=0; i<CIRCBUFSIZE; ++i ) {
      u32CircDuration[ i ] = 0;
    } // for
    u8HeadPtr = 0;
  } // void begin( )


  bool allEqualDistance( ) {
    bool retVal = true;
    uint32_t u32DurTemp = u32CircDuration[0];
    for( uint8_t i=1; i<CIRCBUFSIZE; ++i ) {
      if( u32DurTemp != u32CircDuration[ i ] ) {
        retVal = false;
        break;
      } // if      
    } // for
    
    if( u32DurTemp == 0 ) {
      retVal = false;
    } // 0 is not equal
    
    return retVal;  
  } // bool allEqualDistance( )

  /*
   * Update makes a measurement and update the filtered value
   */
  void update( ) {
    uint32_t u32NewDuration;
    uint32_t u32Now = millis( );
    if( u32Now - u32Previous > u32Timeout ) {
      u32Previous = u32Now;
      // Clear the trigPin1 condition
      digitalWrite( u8TrigPin, LOW );
      delayMicroseconds( 2 );
      // Set the trigPin1 HIGH (ACTIVE) for 10 microseconds
      digitalWrite( u8TrigPin, HIGH );
      delayMicroseconds( 10 );
      digitalWrite( u8TrigPin, LOW );
      // Read the echoPin, returns the sound wave travel time in microseconds
      u32NewDuration = pulseIn( u8EchoPin, HIGH );
      if( u32NewDuration < BIGVALUE ) {
        u32Duration = u32NewDuration;
      } // if too big

      if( ( allEqualDistance( ) ) && ( u32Duration != 0 ) ) {
        u32Duration = 0;
      } // if

      u32CircDuration[ u8HeadPtr++ ] = u32Duration;
      
      if( u8HeadPtr >= CIRCBUFSIZE ) {
        u8HeadPtr = 0;
      } // if
  
      // Calculating distance from time is homework!
      
      Serial.println( u32Duration );
    } // if time
  } // void update( )
}; // class DistanceSencor
