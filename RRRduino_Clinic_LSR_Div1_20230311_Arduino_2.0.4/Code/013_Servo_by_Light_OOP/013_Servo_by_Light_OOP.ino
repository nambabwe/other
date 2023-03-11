/*
 * LSR Div1 2023.03.11 RRRduino for Beginners
 *
 * The light level into the resistive lignt sensor (10k resistor to GND)
 * determines how far the servo moves
 *
 * Servo into a "tilde" pin, like D11
 *
 * Author: Speed Muller, Date: 2023.03.04, 15:23
 */

// include the Servo library
#include <Servo.h>

#include "Heartbeat.h"
#include "Light.h"
#include "RoosterServo.h"

#define VERSION_STR "013_Servo_by_Light, LSR Div1, 2021.10.08, ver 0.01"
#define BAUD      115200

// heartbeat
#define LED0          13

// Boom LEDs
#define LED1           3
#define LED2           5
#define LED3           6
#define LED4           9

// Enable pin
#define INPUTPIN      A1

// Light sensor pin
#define ANAPIN        A0
// 10-bit A/D, 0-1023

// Servo pin
#define SERVOPIN      11

#define ONTIME1     2000
#define ONTIME2     2000
#define ONTIME3     4000
#define ONTIME4      100

#define OFFTIME1    6000
#define OFFTIME2    6000
#define OFFTIME3    4000
#define OFFTIME4     900

#define DELAY       2000

#define TIMEOUT      100

#define MIN_SERVO      0
#define MAX_SERVO    180

#define MIN_ANA      250
#define MAX_ANA      900

RoosterServo myRooster( SERVOPIN, 200, MIN_SERVO, MAX_SERVO, 90 );  // create a servo object

Heartbeat heart( LED0, 50, 617 );
Light light1( LED1, ONTIME1, OFFTIME1 );
Light light2( LED2, ONTIME2, OFFTIME2 );
Light light3( LED3, ONTIME3, OFFTIME3 );
Light light4( LED4, ONTIME4, OFFTIME4 );

unsigned long now, before;
uint16_t analogValue;
uint8_t servoValue;


void setup( ) {
  Serial.begin( BAUD );

  pinMode( INPUTPIN, INPUT_PULLUP );
  pinMode( ANAPIN, INPUT );

  myRooster.begin( ); // Attach the servo to the pin

  heart.begin( );

  light1.begin( );
  delay( DELAY );
  light2.begin( );
  delay( DELAY );
  delay( DELAY );
  light3.begin( );
  
  light4.begin( );

  Serial.println( );
  Serial.println( VERSION_STR );

  now = before = millis( );
} // setup( )


void loop( ) {
  now = millis( );
  if( now -before > TIMEOUT ) {
    before = now;

    analogValue = analogRead( ANAPIN );
    Serial.print( analogValue );
    servoValue = map( analogValue, 0, 1023, 0, 255 );
    Serial.print( "," );
    Serial.println( servoValue );

    // Set the servo
    myRooster.command( servoValue );
  } // if time to read

  if( digitalRead( INPUTPIN ) == LOW ) {
    light1.poweron( );
    light2.poweron( );
    light3.poweron( );
    light4.poweron( );
  } else {
    light1.poweroff( );
    light2.poweroff( );
    light3.poweroff( );
    light4.poweroff( );
  } // if pin is active (which is LOW )

  light1.update( );
  light2.update( );
  light3.update( );
  light4.update( );
  heart.update( );
  myRooster.update( );
} // loop( )
