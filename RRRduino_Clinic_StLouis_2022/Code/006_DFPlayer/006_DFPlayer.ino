#define VERSION_STR "006_STL_DFPlayer Mini ver 0.001"

#define BAUDRATE              115200
#define DFPLAYER_BAUDRATE       9600

#define TIMEOUT                  500

#define SWRXPIN                    2
#define SWTXPIN                    3
#define LEDPIN                    13
#define ButtonPIN                  4

#define MAXVOLUME                 30
#define ONE

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial( SWRXPIN, SWTXPIN );  // RX pin and TX pin to DFPlayer
DFRobotDFPlayerMini myDFPlayer;

unsigned long int now, previous;                  // heartbeat, and avoiding delay( )

byte volume = ( 2 * MAXVOLUME ) / 3;              // 2/3rd of max, change this to a value you like
byte incomingByte;
bool notReady = true;

void setup( ) {
  mySoftwareSerial.begin( DFPLAYER_BAUDRATE );    // DFPlayer default baud rate
  Serial.begin( BAUDRATE );                       // baud rate to PC, (Ctrl+Shift+M)
  delay( 100 );
  pinMode( LEDPIN, OUTPUT );
  pinMode( ButtonPIN, INPUT_PULLUP );
  
  Serial.println( );
  Serial.println( VERSION_STR );

  while( notReady ) {
    if( !myDFPlayer.begin( mySoftwareSerial ) ) { // use softwareSerial to communicate with mp3
      Serial.println( "DFPlayer begin( ) failed:" );
      Serial.println( "Check the wiring and/or insert a uSD card" );
      notReady = true;
      delay( 1000 );                              // wait 1 second to try again
    } else {
      notReady = false;
    } // if ready
  } // while 
  Serial.println( "DFPlayer ready!" );      

  myDFPlayer.volume( volume );                    // set volume value, 0 to 30
  myDFPlayer.play( 1 ) ;                          // play the first file

  now = previous = millis( );
} // setup( )


void loop( ) {
  now = millis( );
  
  if( now - previous >= TIMEOUT ) {
    digitalWrite( LEDPIN, !digitalRead( LEDPIN ) );  // heartbeat
    previous = now;
  } // if

  // Check if pin is low, then play next 
  if( digitalRead( ButtonPIN ) == LOW ) {
    myDFPlayer.next( );                           // play next file
    Serial.println( "Next..." );
    delay( 1000 );                                // crude button debounce
  } // if

  // send '+', '-' or 'n' through serial port from PC
  if( Serial.available( ) ) {
    incomingByte = Serial.read( );
    if( incomingByte == '-' ) {
      myDFPlayer.volumeDown( );                  // volume Down
    } // if -
    
    if( incomingByte == '+' ) {
      myDFPlayer.volumeUp( );                    // volume Up
    } // if +
    
    if( incomingByte == 'n' ) {
      myDFPlayer.next( );                        // play next file
      Serial.println( "next..." );
    } // if next    
  } // if serial available  
} // loop( )  
