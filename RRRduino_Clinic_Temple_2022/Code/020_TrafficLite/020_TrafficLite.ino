#define VERSION_STR    "020_Temple Traffic Lights, 2022.05.07"

#include "TrafficLite.h"

// We are lazy, putting 2, 3, 4, etc straigh in...
TrafficLite *trafficLiteNS = new TrafficLite( 2,  3,  4, HIGH ); // create the first TrafficLite object using pins 2, 3 and 4 
TrafficLite *trafficLiteEW = new TrafficLite( 5,  6,  7, HIGH ); // create the other TrafficLite object using pins 5, 6 and 7

bool bFirstTime;        // let the first lite run and when it goes red, start the
                        // second one with the inter-lite red delay first

void setup( ) {
  Serial.begin( 115200 );
  Serial.println( );
  Serial.println( VERSION_STR );
  
  bFirstTime = true; 
  trafficLiteNS->begin( );
} // setup


void loop( ) {
  if( bFirstTime == 1 ) { // only the first time, wait for red and delayStart lite2
    if( trafficLiteNS->isRed( ) ) { 
      trafficLiteEW->delayStart( );
      bFirstTime = false;
    } // if isRed 
  } // if firstTime
  
  // and this is all that runs after the first time...
  trafficLiteNS->update( );
  trafficLiteEW->update( );
} // loop
