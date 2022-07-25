#define VERSION_STR "006_STL_DFPlayer Mini ver 0.001"

/*
 * Play sounds from the DFMiniPlayer
 *    
 * Code used from https://starthardware.org/en/dfplayer-mini-mp3-player-for-arduino/
 * StartHardware by Stefan Hermann
 * 
 * Author: Gert 'Speed' Muller, 2022.08.09
 */

#include "SoftwareSerial.h"
#include <DFMiniMp3.h>

#include "Heartbeat.h"

#define BAUDRATE              115200
#define DFPLAYER_BAUDRATE       9600

#define TIMEOUT                  500

#define SWRXPIN                    2
#define SWTXPIN                    3
#define LEDPIN                    13
#define ButtonPIN                 A4

#define MAXVOLUME                 30

#define LEDPIN1                   13
#define TIMEOFF                  617
#define TIMEON                    50

SoftwareSerial mySoftwareSerial( SWRXPIN, SWTXPIN );  // RX pin and TX pin to DFPlayer

// forward declare the notify class, just the name
class Mp3Notify; 

// define a handy type using serial and our notify class
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3; 

// instance a DfMp3 object
DfMp3 myDFPlayer( mySoftwareSerial );

#include "Mp3Notify.h"

Heartbeat myHeart = Heartbeat( LEDPIN1, TIMEON, TIMEOFF );

byte volume = ( 2 * MAXVOLUME ) / 3;              // 2/3rd of max, change this to a value you like
byte incomingByte;
bool notReady = true;
uint16_t u16CurrentFile = 1;
uint16_t fileCount = 0;

void setup( ) {
  mySoftwareSerial.begin( DFPLAYER_BAUDRATE );    // DFPlayer default baud rate
  Serial.begin( BAUDRATE );                       // baud rate to PC, (Ctrl+Shift+M)
  delay( 100 );
  myHeart.begin( );
  pinMode( ButtonPIN, INPUT_PULLUP );
  
  Serial.println( );
  Serial.println( VERSION_STR );

  myDFPlayer.begin( );
  myDFPlayer.reset();
  delay( 1000 );
//  while( !myDFPlayer.isOnline( ) ) {
//    Serial.println( "DFPlayer begin( ) failed:" );
//    Serial.println( "Check the wiring and/or insert a uSD card" );
//    delay( 1000 );                              // wait 1 second to try again
//  } // while !ready
  
  fileCount = myDFPlayer.getTotalTrackCount( DfMp3_PlaySource_Sd );
  Serial.println( "DFPlayer ready!" );      

//  if( myDFPlayer.getStatus( ) ) {
//    printDetail( myDFPlayer.readType( ), myDFPlayer.read( ) ); // Print the detail message from DFPlayer
//  } // if
  
  delay( 500 );
  myDFPlayer.setVolume( volume );                    // set volume value, 0 to 30
  delay( 500 );
  Serial.print( F( "Files  : " ) ); Serial.println( myDFPlayer.getTotalTrackCount( DfMp3_PlaySource_Sd ) );
  Serial.print( F( "Folders: " ) ); Serial.println( myDFPlayer.getFolderTrackCount( DfMp3_PlaySource_Sd ) ); 

  Serial.println( "Play first..." );
  myDFPlayer.start( );                           // play the first file
  u16CurrentFile = myDFPlayer.getCurrentTrack( DfMp3_PlaySource_Sd );
  myDFPlayer.loop();
  printMenu( );
} // setup( )


void printMenu( ) {
  Serial.println( F( "\nMenu:" ) );
  Serial.println( F( "+    volume up" ) );
  Serial.println( F( "-    volume down" ) );
  Serial.println( F( "c    current" ) );
  Serial.println( F( "n    next" ) );
  Serial.println( F( "p    previous" ) );
  Serial.println( F( "s    stop\n" ) );    
} // void printMenu( )

void loop( ) {
  myDFPlayer.loop();
//  if( myDFPlayer.available( ) ) {
//    printDetail( myDFPlayer.readType( ), myDFPlayer.read( ) ); //Print the detail message from DFPlayer to handle different errors and states.
//  } // if
  
  // Check if pin is low, then play next 
  if( digitalRead( ButtonPIN ) == LOW ) {
    myDFPlayer.nextTrack( );                     // play next file
    Serial.println( "Next..." );
    delay( 1000 );                               // crude button debounce
  } // if

  // send '+', '-' or 'n' through serial port from PC
  if( Serial.available( ) ) {
    incomingByte = Serial.read( );
    if( incomingByte == '-' ) {
      myDFPlayer.decreaseVolume( );              // volume Down
    } // if -
    
    if( incomingByte == '+' ) {
      myDFPlayer.increaseVolume( );              // volume Up
    } // if +
    
    if( incomingByte == 'p' ) {
      myDFPlayer.stop( );                        // stop
      myDFPlayer.prevTrack( );                   // play prior file
      u16CurrentFile = myDFPlayer.getCurrentTrack( DfMp3_PlaySource_Sd );
      Serial.print( "previous..." );
      Serial.println( u16CurrentFile, DEC );
    } // if previous
    
    if( incomingByte == 'c' ) {
      myDFPlayer.stop( );                        // stop
      myDFPlayer.playGlobalTrack( u16CurrentFile );         // play file
      Serial.println( "current..." );
    } // if current
 
    if( incomingByte == 'n' ) {
      myDFPlayer.stop( );                        // stop
      myDFPlayer.nextTrack( );                   // play next file
      u16CurrentFile = myDFPlayer.getCurrentTrack( DfMp3_PlaySource_Sd );
      Serial.print( "next..." );      
      Serial.println( u16CurrentFile, DEC );
    } // if next    
    
    if( incomingByte == 's' ) {
      myDFPlayer.stop( );                        // stop
      Serial.println( "stop!" );
    } // if stop
  } // if serial available  

  myHeart.update( );
} // loop( )  
