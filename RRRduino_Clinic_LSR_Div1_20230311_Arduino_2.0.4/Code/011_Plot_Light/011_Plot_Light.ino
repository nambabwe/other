/*
 * LSR Div1 2023.03.11 RRRduino for Beginners.
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

#define VERSION_STR "011_Plot_Light, LSR Div 1, 2023.03.11"
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

#define ONTIME1      100
#define ONTIME2      100
#define ONTIME3      100
#define ONTIME4      100

#define OFFTIME1     700
#define OFFTIME2     800
#define OFFTIME3     900
#define OFFTIME4    1200

#define DELAY        125

#define TIMEOUT      100

Heartbeat heart( LED0, 50, 617 );

Light light1( LED1, ONTIME1, OFFTIME1 );
Light light2( LED2, ONTIME2, OFFTIME2 );
Light light3( LED3, ONTIME3, OFFTIME3 );
Light light4( LED4, ONTIME4, OFFTIME4 );

unsigned long now, before;
uint16_t analogValue;


void setup( ) {
  Serial.begin( BAUD );

  pinMode( INPUTPIN, INPUT_PULLUP );
  pinMode( ANAPIN, INPUT );

  light1.begin( );
  delay( DELAY );
  
  light2.begin( );
  delay( DELAY );
  
  light3.begin( );
  delay( DELAY );
  
  light4.begin( );

  heart.begin( );  

  Serial.println( );
  Serial.println( VERSION_STR );

  now = before = millis( );
} // setup( )


void loop( ) {
  now = millis( );
  if( now -before > TIMEOUT ) {
    before = now;

    analogValue = analogRead( ANAPIN );
    Serial.print( "Light:" ); Serial.println( analogValue );
  } // if time to read sensor
  
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
  } // if pin is active (LOW)
  
  light1.update( );
  light2.update( );
  light3.update( );
  light4.update( );
  heart.update( );
} // loop( )
