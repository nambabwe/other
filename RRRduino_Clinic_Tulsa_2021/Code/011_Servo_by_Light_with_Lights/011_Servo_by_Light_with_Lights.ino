/*
 * Tulsa 2021.10.08 Arduino for Beginners Make and Take Clinic
 * 
 * When the boom is going down, simulated with a pin going low on pin 8, or the light on a photoresistor indicating occupancy,
 *   turn the tip of the boom's light on, and flash the two other lights on the boom (as well as the ones under the hoods).
 * 
 * Author: Speed Muller, Date: 2021.10.08, 08:11
 */

#include "light.h"

#define VERSION_STR "011_Enable_by_Light, 2021.10.08, 08:11, ver 0.01"
#define BAUD      115200

// heartbeat
#define LED1          13

// Boom LEDs
#define LED2          12
#define LED3          11
#define LED4          10

// Enable pin
#define INPUTPIN       8

// Light sensor pin
#define ANAPIN        A0

#define ONTIME1      100
#define ONTIME2      500
#define ONTIME3      500
#define ONTIME4      500

#define OFFTIME1     900
#define OFFTIME2       0
#define OFFTIME3     500
#define OFFTIME4     500

#define DELAY          0

#define TIMEOUT     1000

// 10-bit A/D, 0-1023
#define MAXPOINT    1023  
#define SETPOINT     250

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
  
  light1.begin( );
  delay( DELAY );
  
  light2.begin( );
  delay( DELAY );
  
  light3.begin( );
  delay( ONTIME3 );
  
  light4.begin( );

  Serial.println( );
  Serial.println( VERSION_STR );

  now = before = millis( );
} // setup( )


void loop( ) {
  now = millis( );
  if( now - before > TIMEOUT ) {
    before = now;

    analogValue = analogRead( ANAPIN );
    Serial.print( analogValue );
    
    servoValue = map( analogValue, 0, 1023, 0, 255 );
    Serial.print( "," ); 
    Serial.println( servoValue );
  } // if time to read
  
  if( ( digitalRead( INPUTPIN ) == LOW ) || ( analogValue < SETPOINT ) ) {
    light2.poweron( );
    light3.poweron( );
    light4.poweron( );    
  } else {
    light2.poweroff( );
    light3.poweroff( );
    light4.poweroff( );      
  } // if pin is active (which is LOW )
  
  light1.update( );
  light2.update( );
  light3.update( );
  light4.update( );

} // loop( )
