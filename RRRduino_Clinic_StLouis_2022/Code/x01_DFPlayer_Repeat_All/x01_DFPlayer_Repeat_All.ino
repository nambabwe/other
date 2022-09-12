#define VERSION_STR "x01_Repeat_All, St Louis, ver 0.01, 2022.08.09"

/*
 * Play all sounds from the DFMiniPlayer in a loop
 *    
 * Author: Someone on the Internet
 */
 
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_RX = 2; // Connects to module's TX 
static const uint8_t PIN_MP3_TX = 3; // Connects to module's RX 

SoftwareSerial softwareSerial( PIN_MP3_RX, PIN_MP3_TX );

// Create the Player object
DFRobotDFPlayerMini player;

void setup( ) {
  Serial.begin( 57600 );
  softwareSerial.begin( 9600 );
  delay( 1000 );
  
  Serial.println( VERSION_STR );
  
  if( player.begin( softwareSerial ) ) {
    Serial.println( F( "OK" ) );
    Serial.print( F( "Volume: " ) );
    Serial.println( player.readVolume( ), DEC );
    // Set volume to maximum (0 to 30).
    //player.volume( 20 ); //30 is very loud
    Serial.print( F( "Volume: " ) );
    Serial.println( player.readVolume( ), DEC );
    Serial.print( F( "Volume: " ) );
    Serial.println( player.readVolume( ), DEC );
  } else {
    Serial.println( F( "Connecting to DFPlayer Mini failed!" ) );
  } // if
} // void setup( )


void loop( ) {
  Serial.println( F( "Playing next..." ) );
  player.next( );
  delay( 20000 );            // wait 20 seconds
} // void loop( )
