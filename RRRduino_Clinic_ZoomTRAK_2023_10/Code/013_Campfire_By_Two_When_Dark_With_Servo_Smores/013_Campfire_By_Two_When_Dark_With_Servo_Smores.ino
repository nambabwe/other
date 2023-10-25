#define VERSION_STR    "013_CampFire_By_Two_When_Dark_With_Servo_Smores, ZoomTRAK, 2023.10.28"

/*
 * Campfire on PWM pins 5 and 6, with heartbeat on pin 13, a marshmellow moving with servo on PWM 9, 
 *   where the servo only moves when an analog light sensor on pin A0 drops below a specified level
 */

#include "SmoreServo.h"
#include "Campfire.h"
#include "Heartbeat.h"

// Light sensor pin
#define LIGHTSENSORPIN      A0
// 10-bit A/D, 0-1023

#define CAMPFIRE1PIN         5
#define CAMPFIRE2PIN         6

// How often to evaluate
#define CAMPFIRE1TIMEOUT    10
#define CAMPFIRE2TIMEOUT    15

// How often to trigger (0-100)
#define CAMPFIRE1TRIGGER    80
#define CAMPFIRE2TRIGGER    60

// Dimmest levels
#define CAMPFIRE1MIN        20
#define CAMPFIRE2MIN        10

// Brightest levels
#define CAMPFIRE1MAX       100
#define CAMPFIRE2MAX        50

// How often to check light sensor
#define DARKTIMECHECK     2000

// Below what light level to start fire
#define LIGHTLEVEL4FIRE    400

// Servo pin
#define SERVOPIN             9
#define SERVOUPDATE         25

// Servo min and max positions
#define MIN_SERVO           50
#define MAX_SERVO          120

// Light sensor limits
#define MIN_ANA            200
#define MAX_ANA            700

SmoreServo smore = SmoreServo( SERVOPIN, SERVOUPDATE, MIN_SERVO, MAX_SERVO, MIN_SERVO );  // create a servo object

Campfire campfire1 = Campfire( CAMPFIRE1PIN, CAMPFIRE1TIMEOUT, CAMPFIRE1TRIGGER,
                               CAMPFIRE1MIN, CAMPFIRE1MAX ); // create the first CampfireLite object using a PWM pin
Campfire campfire2  = Campfire( CAMPFIRE2PIN, CAMPFIRE2TIMEOUT, CAMPFIRE2TRIGGER,
                                CAMPFIRE2MIN, CAMPFIRE2MAX ); // create the second CampfireLite object using a PWM pin

#define HEARTBEATPIN  LED_BUILTIN
#define HEARTBEATONTIME        50
#define HEARTBEATOFFTIME      617

Heartbeat  myHeart    = Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

// Need time
unsigned long ulNow, ulBefore;

// Store the light value
uint16_t u16AnalogLightSensorValue;

// Keep the commanded servo value
uint8_t u8CommandedServoValue;

void setup( ) {  
  Serial.begin( 115200 );
  pinMode( LIGHTSENSORPIN, INPUT );

  campfire1.begin( );
  campfire2.begin( );
  myHeart.begin( );

  smore.begin( ); // Attach the servo to the pin

  Serial.println( );
  Serial.println( VERSION_STR );

  Serial.println( "light, smores, command, actual" );

  smore.command( MIN_SERVO );

  ulNow = ulBefore = millis( );
} // setup


void stopSmores( ) {
  campfire1.stop( );
  campfire2.stop( );
} // void stopSmores( ) 


void startSmores( ) {
  campfire1.start( );
  campfire2.start( );
} // void startSmores( ) 


void loop( ) {
  ulNow = millis( );

  if( ulNow - ulBefore > DARKTIMECHECK ) {
    ulBefore = ulNow;
    u16AnalogLightSensorValue = analogRead( LIGHTSENSORPIN );
    Serial.print( "light:" );
    Serial.print( u16AnalogLightSensorValue );

    if( u16AnalogLightSensorValue <= LIGHTLEVEL4FIRE ) {
      startSmores( );
    } else {
      stopSmores( );
    } // if dark enough
    
    Serial.print( ", smores:" );
    if( campfire1.bIsRunning( ) ) {
      Serial.print( "1" );
      u8CommandedServoValue = map( u16AnalogLightSensorValue, MIN_ANA, MAX_ANA, MAX_SERVO, MIN_SERVO );
      // Set the servo destination
      smore.command( u8CommandedServoValue );

      // request and print the commanded position
      Serial.print( ", cmnd:" );
      Serial.print( smore.getCommand( ) );

      // request and print the actual current position
      Serial.print( ", actual:" );
      Serial.println( smore.getPosition( ) );
    } else {
      smore.command( MIN_SERVO );
      Serial.print( "0, cmnd: 0" );
      // request and print the actual current position
      Serial.print( ", actual:" );
      Serial.println( smore.getPosition( ) );
    } // if running
  }
  campfire1.update( );
  campfire2.update( );
  myHeart.update( );
  smore.update( );
} // loop

/* end of file */
