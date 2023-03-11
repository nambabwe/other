/*
 * Servo with two positions, up and down, to move to on command with a delay inbetween steps
 */

class RoosterServo {
private:
  uint8_t servoPin = 0;
  uint16_t timeout = 1000;
  uint8_t myPosition = 90;
  uint8_t commandedPosition = 90;
  uint8_t servoMinimum = 0;
  uint8_t servoMaximum = 180;

  bool power = false;
  unsigned long now, before;
  Servo myServo;               // the real servo

public:
  /* Constructor */
  RoosterServo( uint8_t theServoPin, uint16_t theTimeToUpdate,
                uint8_t theServoMinimum, uint8_t theServoMaximum, uint8_t theCommandedPosition ) {
    servoPin = theServoPin;
    timeout = theTimeToUpdate;


    if( ( theCommandedPosition <= theServoMaximum ) && ( theCommandedPosition >= theServoMinimum ) ) {
      commandedPosition = theCommandedPosition;
      // send servo to the start-up
      myPosition = commandedPosition;
    } // if valid position

  } // constructor RoosterServo( uint8_t, uint16_t, uint8_t, uint8_t )

  /* Destructor */
  ~RoosterServo( ) {
    power = false;
    myServo.detach( );
   } // destructor ~RoosterServo( )


  /* begin( ): set the start time */
  void begin( ) {
    now = before = millis( );
    if( servoPin > 0 ) {
      power = true;
      myServo.attach( servoPin ); // Attach the servo to the pin
      output( );
    } else {
      power = false;
    } // if pin valid
  } // void begin( )


  /* command( ): see the new position to go to */
  void command( uint8_t theCommandedPosition) {
   commandedPosition = theCommandedPosition;
  } // bool atTop( )


  /* atCommand( ): return true if at commanded position */
  bool atCommand( ) {
    return( myPosition == commandedPosition );
  } // bool atCommand( )


  /* output( ): move the servo */
  void output( ) {
//    Serial.print( "Servo:" );
//    Serial.println( myPosition );
    if( power) {
      myServo.write( myPosition );
    } // if power
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