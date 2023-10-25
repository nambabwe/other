#define VERSION_STR "011_Servo_by_Light_OOP, ZoomTRAK, 2023.10.28"

/*
 * 2023 TE, 2023.08.20 RRRduino for Beginners
 *
 * The light into the resistive light sensor (10k resistor to GND)
 * determines how far the servo moves
 *
 * Servo into a "tilde" pin, like D11
 *
 * Author: Speed Muller, Date: 2023.03.04, 15:23
 */


#include "Heartbeat.h"
#include "Light.h"
#include "SmoreServo.h"

#define BAUD                115200

// heartbeat
#define HEARTBEATPIN   LED_BUILTIN
#define HEARTBEATONTIME         50
#define HEARTBEATOFFTIME       617

// Light sensor pin
#define ANAPIN                  A0
// 10-bit A/D, 0-1023

// Servo pin
#define SERVOPIN                 9

#define DELAY                 2000

#define TIMEOUT                100

#define MIN_SERVO                0
#define MAX_SERVO              180

#define MIN_ANA                250
#define MAX_ANA                900

SmoreServo mySmore    = SmoreServo( SERVOPIN, 200, MIN_SERVO, MAX_SERVO, 90 );  // create a servo object

Heartbeat  myHeart    = Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

unsigned long now, before;
uint16_t      analogValue;
uint8_t       servoValue;


void setup( ) {
  Serial.begin( BAUD );

  pinMode( ANAPIN, INPUT );

  mySmore.begin( ); // Attach the servo to the pin
  myHeart.begin( );

  Serial.println( );
  Serial.println( VERSION_STR );

  Serial.println( "light, command, actual" );

  now = before = millis( );
} // setup( )


void loop( ) {
  now = millis( );
  if( now - before > TIMEOUT ) {
    before = now;

    analogValue = analogRead( ANAPIN );
    Serial.print( "light:" );
    Serial.print( analogValue );
    servoValue = map( analogValue, 0, 1023, 0, 255 );
    //Serial.print( "," );
    //Serial.print( servoValue );

    // Set the servo destination
    mySmore.command( servoValue );

    // request and print the commanded position
    Serial.print( ", cmnd:" );
    Serial.print( mySmore.getCommand( ) );

    // request and print the actual current position
    Serial.print( ", actual:" );
    Serial.println( mySmore.getPosition( ) );
  } // if time to read

  myHeart.update( );
  mySmore.update( );
} // loop( )
