#define VERSION_STR    "022_Temple All Together with Sensor, 2022.05.07"

/*
 * Campfires, Beacons, Welder, TrafficLites and Sensor, with Heartbeat
 */

#include "Beacon.h"
#include "CampfireLite.h"
#include "Heartbeat.h"
#include "TrafficLite.h"
#include "Welder.h"

#define CAMPFIRE1PIN          9
#define CAMPFIRE2PIN         10

// How often to evaluate
#define CAMPFIRE1TIMEOUT     10
#define CAMPFIRE2TIMEOUT     15

// How often to trigger  (0-100)
#define CAMPFIRE1TRIGGER     80
#define CAMPFIRE2TRIGGER     60

// Dimmest levels
#define CAMPFIRE1MIN         20
#define CAMPFIRE2MIN         10

// Brightest levels
#define CAMPFIRE1MAX        100
#define CAMPFIRE2MAX         50

#define HEARTBEATPIN         13
#define HEARTBEATTIME       500

#define BEACON1PIN           12
#define BEACON1ONTIME        50
#define BEACON1OFFTIME     3500

#define BEACON2PIN           11
#define BEACON2ONTIME        50
#define BEACON2OFFTIME     2500

#define WELDERPIN            A3
#define WELDERONTIME        250
#define WELDERONWHEN       HIGH

#define LIGHTSENSOR          A0
#define LIGHTTRIGGERLEVEL   800

Beacon     *myBeacon1   = new Beacon( BEACON1PIN, BEACON1ONTIME, BEACON1OFFTIME );
Beacon     *myBeacon2   = new Beacon( BEACON2PIN, BEACON2ONTIME, BEACON2OFFTIME );
Welder     *myWelder    = new Welder( WELDERPIN, WELDEROFFRANDOM, WELDERONWHEN );
Heartbeat  *myHeart     = new Heartbeat( HEARTBEATPIN, HEARTBEATTIME );
CampfireLite *campfire1 = new CampfireLite( CAMPFIRE1PIN, CAMPFIRE1TIMEOUT, CAMPFIRE1TRIGGER,
                                           CAMPFIRE1MIN, CAMPFIRE1MAX ); // create the first CampfireLite object using PWM pins 9 
CampfireLite *campfire2 = new CampfireLite( CAMPFIRE2PIN, CAMPFIRE2TIMEOUT, CAMPFIRE2TRIGGER,
                                           CAMPFIRE2MIN, CAMPFIRE2MAX ); // create the first CampfireLite object using PWM pins 9 
TrafficLite *trafficLite1 = new TrafficLite( 2,  3,  4, HIGH ); // create the first TrafficLite object using pins 2, 3 and 4 
TrafficLite *trafficLite2 = new TrafficLite( 5,  6,  7, HIGH ); // create the other TrafficLite object using pins 5, 6 and 7

bool bFirstTime;        // let the first lite run and when it goes red, start the
                        // second one with the inter-lite red delay first

bool bWaitForDark = true;
bool bWasDark = false;

void setup( ) {  
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );

  pinMode( LIGHTSENSOR, INPUT );

  bFirstTime = true; 
  trafficLite1->begin( );
  
  campfire1->begin( );
  campfire2->begin( );
  myBeacon1->begin( );
  myBeacon2->begin( );
  myWelder->begin( );
  myHeart->begin( );  
} // setup


void loop( ) { 
  if( bFirstTime == 1 ) { // only the first time, wait for red and delayStart lite2
    if( trafficLite1->isRed( ) ) { 
      trafficLite2->delayStart( );
      bFirstTime = false;
    } // if isRed 
  } // if firstTime
  
  // and this is all that runs after the first time...
  trafficLite1->update( );
  trafficLite2->update( );
  campfire1->update( );
  campfire2->update( );
  myBeacon1->update( );
  myBeacon2->update( );
  myWelder->update( );
  myHeart->update( );

  uint16_t u16AnalogLight = analogRead( LIGHTSENSOR );
  Serial.println ( u16AnalogLight, DEC );
  
  if( u16AnalogLight > LIGHTTRIGGERLEVEL ) {
    if( bWasDark ) {
      myHeart->doubleBeat( true );
      myWelder->enable( );
      bWasDark = false;
    } // check if we need to switch
  } else {
    if( !bWasDark ) {
      myHeart->doubleBeat( false );
      myWelder->disable( );
      bWasDark = true;
    } // check if we need to switch
  } // if SENSORPIN
} // loop
