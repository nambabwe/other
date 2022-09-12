/* Tulsa 2021.10.08 Arduino for Beginners Make and Take Clinic
 * When the boom is going down, simulated with a pin going low on pin 8,
 *   turn the tip of the boom's light on,
 *   and flash the two other lights on the boom (as well as the ones
 *   under the hoods).
 * 
 * Author: Speed Muller, Date: 2021.10.08, 08:08
 */

#include "light.h"

#define VERSION_STR "008_Enable_Boom_LEDs, 2021.10.08, 08:08, ver 0.01"
#define BAUD 115200


// heartbeat
#define LED1       13

// Boom LEDs
#define LED2       12
#define LED3       11
#define LED4       10

// Enable pin
#define INPUTPIN    8

#define ONTIME1   100
#define ONTIME2   500
#define ONTIME3   500
#define ONTIME4   500

#define OFFTIME1  900
#define OFFTIME2    0
#define OFFTIME3  500
#define OFFTIME4  500

#define DELAY       0

Light light1( LED1, ONTIME1, OFFTIME1 );
Light light2( LED2, ONTIME2, OFFTIME2 );
Light light3( LED3, ONTIME3, OFFTIME3 );
Light light4( LED4, ONTIME4, OFFTIME4 );


void setup( ) {
  Serial.begin( BAUD );

  pinMode( INPUTPIN, INPUT_PULLUP );
  
  light1.begin( );
  delay( DELAY );
  
  light2.begin( );
  delay( DELAY );
  
  light3.begin( );
  delay( ONTIME3 );
  
  light4.begin( );

  Serial.println( );
  Serial.println( VERSION_STR );
} // setup( )


void loop( ) {
  if( digitalRead( INPUTPIN ) == LOW ) {
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

//
// Notice that your heartbeat is still going, so the code must be running. Speed rule 202: Always have a heartbeat!
//
