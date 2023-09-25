/*
 * Servo moving from a current position slowly to a commanded position
 * 
 * Author: Speed Muller, 2023.10.07
 */

// include the Servo library
#include <Servo.h>

class SmoreServo {
private:
  uint8_t  u8ServoPin          =    0;
  uint16_t u16Timeout          = 1000;
  uint8_t  u8Position          =   90;
  uint8_t  u8CommandedPosition =   90;
  uint8_t  u8ServoMinimum      =    0;
  uint8_t  u8ServoMaximum      =  180;

  bool    bPower               = false;
  unsigned long ulNow, ulBefore;
  Servo   myServo;           // the real servo

public:
  /* Constructor */
  SmoreServo( uint8_t theServoPin, uint16_t theTimeToUpdate,
              uint8_t theServoMinimum, uint8_t theServoMaximum, uint8_t theCommandedPosition ) {
    u8ServoPin = theServoPin;
    u16Timeout = theTimeToUpdate;

    if( ( theCommandedPosition <= theServoMaximum ) && ( theCommandedPosition >= theServoMinimum ) ) {
      u8CommandedPosition = theCommandedPosition;
      // send servo to the start-up
      u8Position = u8CommandedPosition;
    } // if valid position

  } // constructor SmoreServo( uint8_t, uint16_t, uint8_t, uint8_t, uint8_t )

  /* Destructor */
  ~SmoreServo( ) {
    bPower = false;
    myServo.detach( );
  } // destructor ~SmoreServo( )


  /* begin( ): set the start time */
  void begin( ) {
    ulNow = ulBefore = millis( );
    if( u8ServoPin > 0 ) {
      bPower = true;
      myServo.attach( u8ServoPin ); // Attach the servo to the pin
      output( );
    } else {
      bPower = false;
    } // if pin valid
  } // void begin( )


  /* command( ): set the new position to go to */
  void command( uint8_t theCommandedPosition ) {
   u8CommandedPosition = theCommandedPosition;
  } // void command( uint8_t )


  /* getCommand( ): get the position to go to */
  uint8_t getCommand( ) {
    return u8CommandedPosition;
  } // uint8_t getCommand( )


  /* getPosition( ): get the current position */
  uint8_t getPosition( ) {
    return u8Position;
  } // uint8_t getPosition( )


  /* isAtCommand( ): return true if at commanded position */
  bool isAtCommand( ) {
    return( u8Position == u8CommandedPosition );
  } // bool isAtCommand( )


  /* output( ): move the servo */
  void output( ) {
//    Serial.print( "Servo:" );
//    Serial.println( u8Position );
    if( bPower ) {
      myServo.write( u8Position );
    } // if bPower
  } // void output( )


  /* update( ): from the current position, move closer to the commanded position if different */
  void update( ) {
    ulNow = millis( );
    if( ulNow - ulBefore >= u16Timeout ) {
      ulBefore = ulNow;
      if( u8Position == u8CommandedPosition ) {
        // do nothing
      } else {
        if( u8Position < u8CommandedPosition ) {
          u8Position++;
          output( );
        } else {
          u8Position--;
          output( );
        } // decrease
      } // not there yet
    } // // if time to update
  } // void update( )
}; // class SmoreServo( )

/* end of file */
