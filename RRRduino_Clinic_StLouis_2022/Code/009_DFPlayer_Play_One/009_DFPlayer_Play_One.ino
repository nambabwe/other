#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_RX = 2; // Connects to module's TX 
static const uint8_t PIN_MP3_TX = 3; // Connects to module's RX 

const byte funBtn = 7;
byte pb;

SoftwareSerial softwareSerial( PIN_MP3_RX, PIN_MP3_TX );
// Create the Player object
DFRobotDFPlayerMini player;

void setup( ) {
  pinMode( funBtn, INPUT_PULLUP );
  Serial.begin( 19200 );
  softwareSerial.begin( 9600 );
  delay( 1000 );
  if( player.begin( softwareSerial ) ) 
  {
    Serial.println( F( "OK" ) );
    Serial.print( F( "Volume: " ) );
    Serial.println( player.readVolume( ), DEC );
    // Set volume to maximum (0 to 30).
    //player.volume( 20 ); //30 is very loud
    Serial.print( F( "Volume: " ) );
    Serial.println( player.readVolume( ), DEC );
    Serial.print( F( "Volume: " ) );
    Serial.println( player.readVolume( ), DEC );
  } 
  else 
  {
    Serial.println( F( "Connecting to DFPlayer Mini failed!" ) );
  }
} // void setup( )


void loop( ) {
  Serial.println( F( "Playing #1" ) );
  player.next( );
  delay( 20000 );
} // void loop( )
