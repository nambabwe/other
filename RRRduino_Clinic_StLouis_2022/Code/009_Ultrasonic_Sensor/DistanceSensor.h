class DistanceSensor {

private:
  uint8_t u8TrigPin;
  uint8_t u8EchoPin;
  uint32_t u32Previous;
  uint32_t u32Timeout;
  uint32_t u32Duration;
//  uint32_t u32Distance;

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
  } // void begin( )


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

      // Calculating distance from time is homework!
      
      Serial.println( u32Duration );
    } // if time
  } // void update( )
}; // class DistanceSencor
