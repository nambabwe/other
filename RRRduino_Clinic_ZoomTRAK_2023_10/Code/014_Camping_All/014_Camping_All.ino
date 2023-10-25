#define VERSION_STR    "014_Camping_All, ZoomTRAK, 2023.10.28"

/*
 * Campfire on PWM pins 5 and 6, with heartbeat on pin 13, a marshmellow moving with servo on PWM 9, 
 *   and up to two fireflies on PWM pins 3 and 11, where the servo only moves when an analog light sensor 
 *   on pin A0 drops below a specified level. Fire will also fade out when light gets bright again.
 */

#define TWOFIREFLIES

#include "SmoreServo.h"
#include "Campfire.h"
#include "Heartbeat.h"
#include "FireFly.h"

#define HEARTBEATPIN    LED_BUILTIN

// Light sensor pin
#define LIGHTSENSORPIN           A0
// 10-bit A/D, 0-1023

#define FIREFLY1PIN               3
#define FIREFLY2PIN              11

#define WELDERPIN                 4

#define CAMPFIRE1PIN              5
#define CAMPFIRE2PIN              6

// How often to evaluate
#define CAMPFIRE1TIMEOUT         10
#define CAMPFIRE2TIMEOUT         15

// How often to trigger (0-100)
#define CAMPFIRE1TRIGGER         80
#define CAMPFIRE2TRIGGER         60

// Dimmest levels
#define CAMPFIRE1MIN              5
#define CAMPFIRE2MIN              5

// Brightest levels
#define CAMPFIRE1MAX            200
#define CAMPFIRE2MAX            250

// How often to check light sensor
#define DARKTIMECHECK           500

// Below what light level to start fire
#define LIGHTLEVEL4FIRE         400

// Servo pin
#define SERVOPIN                  9
#define SERVOUPDATE              25

// Servo min and max positions
#define MIN_SERVO                50
#define MAX_SERVO               120

// Light sensor limits
#define MIN_ANA                 200
#define MAX_ANA                 700

// Welder settings
#define WELDERONTIME            250
#define WELDERONWHEN           HIGH

// Heartbeat settings
#define HEARTBEATONTIME          50
#define HEARTBEATOFFTIME        617

// Firefly settings
#define FLYOFFDELAYMIN          500
#define FLYOFFDELAYMAX        15000
#define FLYMIN                   50

Heartbeat    *myHeart    = new Heartbeat( HEARTBEATPIN, HEARTBEATONTIME, HEARTBEATOFFTIME );

SmoreServo   myRooster  = SmoreServo( SERVOPIN, SERVOUPDATE, MIN_SERVO, MAX_SERVO, MIN_SERVO );  // create a servo object

Campfire     campfire1  = Campfire( CAMPFIRE1PIN, CAMPFIRE1TIMEOUT, CAMPFIRE1TRIGGER,
                                        CAMPFIRE1MIN, CAMPFIRE1MAX ); // create the first CampfireLite object using a PWM pin
Campfire     campfire2  = Campfire( CAMPFIRE2PIN, CAMPFIRE2TIMEOUT, CAMPFIRE2TRIGGER,
                                       CAMPFIRE2MIN, CAMPFIRE2MAX ); // create the second CampfireLite object using a PWM pin

FireFly      myFly      = FireFly( FIREFLY1PIN, FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );

#ifdef TWOFIREFLIES
FireFly      myFly2     = FireFly( FIREFLY2PIN, FLYOFFDELAYMIN, FLYOFFDELAYMAX, FLYMIN );
#endif

// Need time
unsigned long ulNow, ulBefore;

// Store the light value
uint16_t u16AnalogLightSensorValue;

// Keep the commanded servo value
uint8_t u8CommandedServoValue;

void setup( ) {
  randomSeed( analogRead( 1 ) );
  Serial.begin( 115200 );
  pinMode( LIGHTSENSORPIN, INPUT );

  campfire1.begin( );
  campfire2.begin( );
  myHeart->begin( );
  myFly.begin( );

  #ifdef TWOFIREFLIES
  myFly2.begin( );
  #endif

  myRooster.begin( ); // Attach the servo to the pin

  Serial.println( );
  Serial.println( VERSION_STR );

  Serial.println( "light, smores, command, actual" );

  myRooster.command( MIN_SERVO );

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
    if( campfire1.bIsHot( ) ) {
      Serial.print( campfire1.getFlickering( ) );
      u8CommandedServoValue = map( u16AnalogLightSensorValue, MIN_ANA, MAX_ANA, MAX_SERVO, MIN_SERVO );
      // Set the servo destination
      myRooster.command( u8CommandedServoValue );

      // request and print the commanded position
      Serial.print( ", cmnd:" );
      Serial.print( myRooster.getCommand( ) );

      // request and print the actual current position
      Serial.print( ", actual:" );
      Serial.println( myRooster.getPosition( ) );
    } else {
      myRooster.command( MIN_SERVO );
      Serial.print( "0, cmnd: 0" );
      // request and print the actual current position
      Serial.print( ", actual:" );
      Serial.println( myRooster.getPosition( ) );
    } // if running
  }

  campfire1.update( );
  campfire2.update( );
  myHeart->update( );
  myRooster.update( );

  myFly.update( );

  #ifdef TWOFIREFLIES
  myFly2.update( );
  #endif
} // loop

/* end of file */
