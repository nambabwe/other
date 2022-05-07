#define VERSION_STR    "020_Temple Traffic Lights, 2022.05.07"

#include "TrafficLite.h"

// We are lazy, putting 4, 3, 2, etc straigh in...
TrafficLite *trafficLiteNS = new TrafficLite( 4,  3,  2, HIGH ); // create the first TrafficLite object using pins 4, 3 and 2 
TrafficLite *trafficLiteEW = new TrafficLite( 7,  6,  5, HIGH ); // create the other TrafficLite object using pins 7, 6 and 5

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
