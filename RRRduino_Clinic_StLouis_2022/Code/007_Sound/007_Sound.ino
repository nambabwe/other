#define VERSION_STR "007_Sound, St Louis, ver 0.01"

/*
 * Play first sound on the DFMiniPlayer
 *    
 * Code used from https://starthardware.org/en/dfplayer-mini-mp3-player-for-arduino/
 * StartHardware by Stefan Hermann
 * 
 * Author: Gert 'Speed' Muller, 2022.08.09
 */

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Heartbeat.h"

// function prototypes (the Arduino tool does this by default, other C compiler setups may not!
void setMP3Volume( uint8_t );
void printMenu( );
void printCurrentFileNum( bool );
void printPlayerInfo( );

// define constants
#define BAUDRATE               57600
#define DFPLAYER_BAUDRATE       9600

#define SWRXPIN                    2
#define SWTXPIN                    3
#define LEDPIN                    13

#define MAXVOLUME                 30

#define LEDPIN1                   13
#define TIMEOFF                  617
#define TIMEON                    50

// defines objects
SoftwareSerial mySoftwareSerial( SWRXPIN, SWTXPIN );  // RX pin and TX pin to DFPlayer
DFRobotDFPlayerMini myDFPlayer;
Heartbeat myHeart = Heartbeat( LEDPIN1, TIMEON, TIMEOFF );

// defines variables
bool notReady = true;

// byte
uint8_t volume = ( 2 * MAXVOLUME ) / 3;              // 2/3rd of max, change this to a value you like
uint8_t u8IncomingByte;

// int
int16_t i16CurrentFile = 1;


/*
 * setup( ) called once from main.cpp
 */
void setup( ) {
  mySoftwareSerial.begin( DFPLAYER_BAUDRATE );    // DFPlayer default baud rate
  Serial.begin( BAUDRATE );                       // baud rate to PC, (Ctrl+Shift+M)
  delay( 100 );
  myHeart.begin( );
  
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

  delay( 100 );  
  setMP3Volume( volume );                        // set volume value, 0 to 30
  delay( 500 );
  printPlayerInfo( );
  
  Serial.println( "Play first..." );
  myDFPlayer.start( );                           // play the first file
} // setup( )


/*
 * loop( ) called from main.cpp in a for(;;) loop. Keeps calling until code is blocked, reset, or power removed
 */
void loop( ) {
  // Send commands through serial port from PC (the USB cable provides a COMx, ttyUSBx or ttyACMx interface)
  if( Serial.available( ) ) {
    Serial.write( Serial.read( ) );    // print anything we receive
  } // if serial available  

  myHeart.update( );
} // loop( )


void setMP3Volume( uint8_t u8TheVolume ) {
  if( u8TheVolume < 31 ) {
    uint8_t u8CurrentVolume = myDFPlayer.readVolume( );
    while( u8CurrentVolume != u8TheVolume ) {
      if( u8CurrentVolume > u8TheVolume ) {
        myDFPlayer.volumeDown( );                    // volume Down
      } else {
        if( u8CurrentVolume < u8TheVolume ) {
          myDFPlayer.volumeUp( );                    // volume Up
        } // if less
      } // if else
      delay( 1 );
      u8CurrentVolume = myDFPlayer.readVolume( );
    } // while
  } // if valid volume
} // void setMP3Volume( uint8_t )


void printCurrentFileNum( bool bPrintLabel ) {
  i16CurrentFile = myDFPlayer.readCurrentFileNumber( DFPLAYER_DEVICE_SD );
  if( bPrintLabel ) 
    Serial.print( F( "Curent : " ) );  
  Serial.println( i16CurrentFile, DEC );
} // void printCurrentFileNum( )


void printPlayerInfo( ) {
  Serial.print( F( "Files  : " ) ); Serial.println( myDFPlayer.readFileCounts( ) );
  Serial.print( F( "Folders: " ) ); Serial.println( myDFPlayer.readFolderCounts( ) );
  printCurrentFileNum( true );
  Serial.print( F( "Volume : " ) ); Serial.println( myDFPlayer.readVolume( ) );  
  Serial.println( );
} // printPlayerInfo( )
