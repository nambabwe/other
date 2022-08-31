/*
 * Tulsa 2021.10.08 Arduino for Beginners Make and Take Clinic
 * 
 * When the boom is going down, simulated with a pin going low on pin 8, or the light on a photoresistor indicating occupancy,
 *   turn the tip of the boom's light on, and flash the two other lights on the boom (as well as the ones under the hoods).
 *   
 *   Making an object with the servo, so it can slowly move between two positions.
 *   
 *   Keep the lights on until the servo is at the top
 *   
 *   Enable sound pin
 * 
 * Servo into a "tilde" pin, like D6
 * 
 * Author: Speed Muller, Date: 2021.10.08, 08:10
 */

// include the Servo library
#include <Servo.h>

#include "light.h"
#include "tulsaservo.h"

#define VERSION_STR "013_Gate_Crossing_Controller, 2021.10.08, 08:13, ver 0.01"
#define BAUD          115200

// heartbeat
#define LED1              13

// Boom LEDs
#define LED2              10
#define LED3              11
#define LED4              12

// Enable sound pin
#define SOUNDPIN           7

// Enable pin
#define INPUTPIN           8

// Light sensor pin
#define ANAPIN            A0

// Servo pin
#define SERVOPIN           6

#define ONTIME1          100
#define ONTIME2          500
#define ONTIME5          500

#define OFFTIME1         900
#define OFFTIME2           0
#define OFFTIME5         500

#define SERVOUPDATETIME  100

#define TIMEOUT         1000

// 10-bit A/D, 0-1023
#define MAXPOINT        1023  
#define SETPOINT         250

Light light1( LED1, ONTIME1, OFFTIME1 );
Light light2( LED2, ONTIME2, OFFTIME2 );
Light light3( LED3, ONTIME5, OFFTIME5 );
Light light4( LED4, ONTIME5, OFFTIME5 );

// TulsaServo( uint8_t theServoPin, uint16_t theTime, uint8_t theUpPosition, uint8_t theDownPosition ) 

TulsaServo boomServo( SERVOPIN, SERVOUPDATETIME, 80, 200 );

unsigned long now, before;
uint16_t analogValue;
uint8_t servoValue;


void setup( ) {
  Serial.begin( BAUD );

  pinMode( INPUTPIN, INPUT_PULLUP );
  pinMode( ANAPIN, INPUT );  
  pinMode( SOUNDPIN, OUTPUT );

  boomServo.begin( );
  
  light1.begin( );
  light2.begin( );
  light3.begin( );
  delay( ONTIME5 );  
  light4.begin( );

  Serial.println( );
  Serial.println( VERSION_STR );

  now = before = millis( );
  boomServo.raise( );  
} // setup( )


void loop( ) {
  now = millis( );
  if( now - before > TIMEOUT ) {
    before = now;
    analogValue = analogRead( ANAPIN );
    Serial.println( analogValue );
  } // if time to read
  
  if( ( digitalRead( INPUTPIN ) == LOW ) || ( analogValue < SETPOINT ) ) {
    light2.poweron( );
    light3.poweron( );
    light4.poweron( );
    boomServo.lower( );
    digitalWrite( SOUNDPIN, HIGH );
  } else {
    boomServo.raise( );
    if( boomServo.atTop( ) == true ) {
      light2.poweroff( );
      light3.poweroff( );
      light4.poweroff( );  
      digitalWrite( SOUNDPIN, LOW );
    } // if done
  } // if pin is active (which is LOW )
  
  light1.update( );
  light2.update( );
  light3.update( );
  light4.update( );
  boomServo.update( );
} // loop( )

// Can do better than ALWAYS telling it to raise or lower it. 
// Create a state and set or reset it.
