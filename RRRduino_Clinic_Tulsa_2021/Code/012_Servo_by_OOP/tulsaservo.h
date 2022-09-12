/*
 * Servo with two positions, up and down, to move to on command with a delay inbetween steps
 */
class TulsaServo {
private:
  uint8_t servoPin;
  uint16_t timeout;
  uint8_t myPosition;
  uint8_t commandedPosition;
  uint8_t upPosition;
  uint8_t downPosition;

  bool power = false;
  unsigned long now, before;
  Servo myServo;               // the real servo

public:
  /* Constructor */
   TulsaServo( uint8_t theServoPin, uint16_t theTime, uint8_t theUpPosition, uint8_t theDownPosition ) {
    servoPin = theServoPin;
    timeout = theTime;
    upPosition = theUpPosition;
    downPosition = theDownPosition;

    // send servo to the middle of the two points at start-up
    commandedPosition = myPosition = (int8_t)( ( (int16_t)theUpPosition + (int16_t)theDownPosition ) / 2 );
    myPosition = commandedPosition;
  } // constructor TulsaServo( uint8_t, uint16_t, uint8_t, uint8_t )

  /* Destructor */
  ~TulsaServo( ) { 
    myServo.detach( );   
   } // destructor ~TulsaServo( )


  /* begin( ): set the start time */  
  void begin( ) {
    now = before = millis( );
    power = true;
    myServo.attach( servoPin ); // Attach the servo to the pin
    output( );
  } // void begin( )


  /* lower( ): send the servo to the down position */  
  void lower( ) {
    commandedPosition = downPosition;
  } // void lower( )


  /* raise( ): send the servo to the raised position */  
  void raise( ) {
    commandedPosition = upPosition;
  } // void raise( )


  /* atTop( ): return true if at top */ 
  bool atTop( ) {
    return( myPosition == upPosition );
  } // bool atTop( )


  /* output( ): move the servo */  
  void output( ) {
//    Serial.print( "Servo:" );
//    Serial.println( myPosition );
    myServo.write( myPosition );
  } // void output( )


  /* update( ): from the state it is in, determine if the time has expired and flip the state */  
  void update( ) {
    now = millis( );
    if( now - before >= timeout ) {
      before = now;
      if( myPosition == commandedPosition ) {
        // do nothing 
      } else {
        if( myPosition < commandedPosition ) {
          myPosition++;
          output( );
        } else {
          myPosition--;
          output( );         
        } // decrease
      } // not there yet
    } // if time on is over    
  } // void update( )
}; // class TulsaServo( )

/* end of file */
