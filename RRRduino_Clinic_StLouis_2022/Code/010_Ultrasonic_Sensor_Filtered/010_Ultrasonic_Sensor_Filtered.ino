#define VERSION_STR "009_Ultrasonic_Sensor, St Louis, ver 0.01, 2022.08.09"

/*
 * Measure the time the Ultrasonic Sensor outputs on the ECHO pin and filter it
 *    
 * Code used from https://starthardware.org/en/dfplayer-mini-mp3-player-for-arduino/
 * StartHardware by Stefan Hermann
 * 
 * Author: Gert 'Speed' Muller, 2022.08.09
 */

#include "SoftwareSerial.h"
#include "Heartbeat.h"
#include "DistanceSensor.h"

// function prototypes (the Arduino tool does this by default, other C compiler setups may not!
void printMenu( );

// define constants
#define BAUDRATE               57600

#define LEDPIN1                   13
#define TIMEOFF                  617
#define TIMEON                    50

#define ECHOPIN                    7 // attach pin D4 Arduino to pin Echo of the HC-SR04
#define TRIGPIN                    8 // attach pin D5 Arduino to pin Trig of the HC-SR04
#define MEASUREHOWOFTEN          250 // milliseconds

// defines objects
Heartbeat myHeart = Heartbeat( LEDPIN1, TIMEON, TIMEOFF );
DistanceSensor myDistanceSensor = DistanceSensor( TRIGPIN, ECHOPIN, MEASUREHOWOFTEN );

// defines variables
bool notReady = true;

// byte
uint8_t u8IncomingByte;

// int
int16_t i16Distance;  // variable for the 1st distance measurement

// long
int32_t i32Duration; // variable for the 1st duration of sound wave travel

/*
 * setup( ) called once from main.cpp
 */
void setup( ) {
  Serial.begin( BAUDRATE );                       // baud rate to PC, (Ctrl+Shift+M)
  delay( 100 );
  myHeart.begin( );
  
  myDistanceSensor.begin( );

  // Plotting, only want to se label
  // Serial.println( );
  // Serial.println( VERSION_STR );

  // Print label
  //Serial.println( );
  Serial.println( F( "Pulse," ) );  
} // setup( )


/*
 * loop( ) called from main.cpp in a for(;;) loop. Keeps calling until code is blocked, reset, or power removed
 */
void loop( ) {
  myHeart.update( );
  myDistanceSensor.update( );
} // loop( )
