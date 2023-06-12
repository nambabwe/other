#define VERSION_STR "RMC Coal Unload Stepper Controller, ver 0.05, 2023.06.12"

/*
 * TODO: Serial and EEPROM to save MAXPOS.
 */

#include "Heartbeat.h"
#include "StepperDM542.h"

// define constants
#define DEBUG                   true
#define BAUDRATE              115200

#define LEDPIN                    13
#define GOHOMEBUTTONPIN           A0
#define GOBOTTBUTTONPIN           A1
#define ESTOPBUTTONPIN            A2
#define HOMEPIN                   A3
//A1

#define DIRPIN                     5
#define STEPPIN                   12

#define LEDPIN1                   13

#define TIMEOFF                  950
#define TIMEON                    50

#define STEPTIMEON                20
#define STEPTIMEOFF                2

unsigned long ulNow;
unsigned long ulPrevious;

Heartbeat myHeart = Heartbeat( LEDPIN1, TIMEON, TIMEOFF );
StepperDM542 myStepper = StepperDM542( STEPPIN, HOMEPIN, DIRPIN, STEPTIMEON, STEPTIMEOFF );

bool bEStop;
bool bWaitForEstopRelease = false;
bool bWaitForHomeRelease = false;
bool bWaitForAwayRelease = false;
bool bAwayPressed = false;
bool bHomePressed = false;
bool bEStopPressed = false;


void setup( ) {
  Serial.begin( BAUDRATE );      // baud rate to PC, (Ctrl+Shift+M)

  pinMode( GOHOMEBUTTONPIN, INPUT_PULLUP );
  pinMode( GOBOTTBUTTONPIN, INPUT_PULLUP );
  pinMode( ESTOPBUTTONPIN, INPUT_PULLUP );

  myHeart.begin( );
 
  myStepper.begin( );

  Serial.println( );
  Serial.println( VERSION_STR );

  ulNow = ulPrevious = millis( );

  // Since we lost home
  myStepper.goHome( );
  Serial.println( "Going Home!" );

  bEStop = false;
} // void setup( )


void loop( ) {
  bool buttonHome   = !digitalRead( GOHOMEBUTTONPIN );
  bool buttonAway   = !digitalRead( GOBOTTBUTTONPIN );
  bool bButtonEstop = !digitalRead( ESTOPBUTTONPIN );

  ulNow = millis( );
  if( ulNow - ulPrevious > 2500 ) {
    ulPrevious = ulNow;
    if( DEBUG ) {
      myStepper.printState( );
    } // if DEBUG
  } // if time to do something


  if( myStepper.isHoming( ) ) {
    // do nothing with buttons while homing    
  } else {
    if( buttonHome ) {
      if( !bWaitForHomeRelease ) {
        myStepper.stop( );      
        delayMicroseconds( 50 );
      } // if !bWaitForHomeRelease
      bWaitForHomeRelease = true;
    } // if( buttonHome )
  
    if( !buttonHome && bWaitForHomeRelease ) {
      myStepper.stop( );
      bWaitForHomeRelease = false;
      bHomePressed = true;
      delay( 10 );
    } // if !buttonHome && bWaitForHomeRelease
    
    if( bHomePressed ) {
      bEStop = false;
      bHomePressed = false;
      myStepper.goHome( );
    } // if bHomePressed
    
    if( buttonAway ) {
      if( !bWaitForAwayRelease ) {
        myStepper.stop( );
        delayMicroseconds( 50 );
      } // if( !bWaitForAwayRelease )
      bWaitForAwayRelease = true;
    } // if buttonAway
  
    if( !buttonAway && bWaitForAwayRelease ) {
      myStepper.stop( );      
      bWaitForAwayRelease = false;
      bAwayPressed = true;
      delay( 10 );
    } // if !buttonAway && bWaitForAwayRelease
    
    if( bAwayPressed ) {
      bEStop = false;
      bAwayPressed = false;
      myStepper.goToAway( );
    } // if bAwayPressed 
  } // if not homing

  // check E-stop
  if( bButtonEstop ) {
    bEStop = true;    // do E-stop
    Serial.println( F("\nE-Stop enabled!!!") );
    bWaitForEstopRelease = true;
    myStepper.stop( );
    delay( 1000 );    // delays are bad, but E-stop is more important
  } // if bButtonEstop
  
  if( bWaitForEstopRelease && !bButtonEstop ) {
    bEStop = false;
    bWaitForEstopRelease = false;
    Serial.println( F("\nE-Stop released!") );
    delay( 200 );    // only a debounce, since nothing else is happening
  } // if bWaitForEstopRelease && !bButtonEstop

  // Always beat the heart and show some debug on serial port
  myHeart.update( myStepper.getStepsToGo( ) );

  if( !bEStop ) { 
    myStepper.update( );
  } // if not E-stop
} // void loop( )
