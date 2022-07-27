class DistanceSensor {

private:
  uint8_t u8TrigPin;
  uint8_t u8EchoPin;
  uint32_t u32Previous;
  uint32_t u32PreviousTrigger;
  uint32_t u32Timeout;
  uint32_t u32Duration;
  uint32_t u32Filtered = 18000;
  uint32_t u32LastTrigger = 18000;
  bool bTriggered = false;
  bool bAllowTrigger = true;

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
    bTriggered = false;
    bAllowTrigger = true;
    u32Filtered = u32LastTrigger = u32PreviousTrigger = 18000;
    u32Previous = millis( );
  } // void begin( )


  /*
   * Check if triggered and clear is requested
   */
   bool checkAndClearTriggered( bool bTheClear ) {
    bool returnVal = bTriggered;
    if( bTheClear ) {
      bTriggered = false;
    }
    return returnVal;
  } // bool checkAndClearTriggered( bool )


  /*
   * Return the triggered value
   */
  uint32_t u32GetTriggered( ) {
    return u32LastTrigger;
  } // uint32_t u32GetTriggered( bool )


  /*
   * Filter: 3 quarters of previous and 1/4 of new
   * If not triggered, also determine if new filtered value is 25% away from last triggered value
   */
  void filter( bool bTheTriggerForce ) {
    if( u32Filtered == 0 ) {
      u32Filtered = u32Duration;
    } else {
      u32Filtered = u32Filtered / 2 + u32Filtered / 4 + u32Duration / 4;
    } // if

    if( bTheTriggerForce ) {
      if( !bTriggered ) {
        uint32_t u32TwentyPct = u32LastTrigger / 4;
        //Serial.println( ); Serial.print( u32Filtered ); Serial.print( F( ", " ) ); Serial.print( u32LastTrigger ); Serial.print( F( " 25%: " ) ); Serial.println( u32TwentyPct );
        if( u32Filtered >= u32LastTrigger ) {
          if( u32Filtered - u32LastTrigger > u32TwentyPct ) {
            //Serial.print( u32Filtered ); Serial.print( F( ", " ) ); Serial.print( u32LastTrigger ); Serial.print( F( ", " ) ); Serial.println( u32TwentyPct );
            bTriggered = true;
            bAllowTrigger = false;
            u32PreviousTrigger = millis( );
            u32LastTrigger = u32Filtered;
          } // if bigger by 20%
        } else {
          if( u32LastTrigger - u32Filtered > u32TwentyPct ) {
            //Serial.print( u32LastTrigger ); Serial.print( F( ", " ) ); Serial.print( u32Filtered ); Serial.print( F( ", " ) ); Serial.println( u32TwentyPct );
            bTriggered = true;
            bAllowTrigger = false;
            u32PreviousTrigger = millis( );
            u32LastTrigger = u32Filtered;
          } // if smaller by 20%
        } // if
      } // if not triggered
    } // if allow trigger
  } // void filter( bool )


  /*
   * Update makes a measurement and update the filtered value
   */
  void update( ) {
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
      u32Duration = pulseIn( u8EchoPin, HIGH );

      if( bAllowTrigger ) {
        filter( true );
      } else {
        //Serial.print( u32Duration ); Serial.print( F( ", " ) );
        if( u32Now - u32PreviousTrigger > 10000 ) {
          filter( true );
          bAllowTrigger = true;
        } else {
          filter( false );
        }
      } // if trigger allowed
      //Serial.print( u32Filtered ); Serial.print( F( ", " ) ); Serial.println( bTriggered ? "y" : "n" );
    } // if time
  } // void update( )
}; // class DistanceSencor
