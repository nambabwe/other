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
#include "DFRobotDFPlayerMini.h"

#include "Heartbeat.h"

#define BAUDRATE               57600
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
DFRobotDFPlayerMini myDFPlayer;

Heartbeat myHeart = Heartbeat( LEDPIN1, TIMEON, TIMEOFF );

byte volume = ( 2 * MAXVOLUME ) / 3;              // 2/3rd of max, change this to a value you like
byte incomingByte;
bool notReady = true;
int16_t i16CurrentFile = 1;

void setup( ) {
  mySoftwareSerial.begin( DFPLAYER_BAUDRATE );    // DFPlayer default baud rate
  Serial.begin( BAUDRATE );                       // baud rate to PC, (Ctrl+Shift+M)
  delay( 100 );
  myHeart.begin( );
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

  if( myDFPlayer.available( ) ) {
    printDetail( myDFPlayer.readType( ), myDFPlayer.read( ) ); // Print the detail message from DFPlayer
  } // if
  
  delay( 500 );
  myDFPlayer.volume( volume );                    // set volume value, 0 to 30
  delay( 500 );
  printPlayerInfo( );
  
  Serial.println( "Play first..." );
  myDFPlayer.start( );                           // play the first file
  i16CurrentFile = myDFPlayer.readCurrentFileNumber( DFPLAYER_DEVICE_SD );
  
  printMenu( );
} // setup( )


void printMenu( ) {
  Serial.println( F( "\nMenu:" ) );
  Serial.println( F( "+    volume up" ) );
  Serial.println( F( "-    volume down" ) );
  Serial.println( F( "i    info" ) );
  Serial.println( F( "c    current" ) );
  Serial.println( F( "n    next" ) );
  Serial.println( F( "N    play next" ) );
  Serial.println( F( "p    previous" ) );
  Serial.println( F( "P    play previous" ) );
  Serial.println( F( "s    stop" ) );    
  Serial.println( F( "S    start" ) );    
  Serial.println( F( "R    reset\n" ) );    
} // void printMenu( )


void printPlayerInfo( ) {
  Serial.print( F( "Files  : " ) ); Serial.println( myDFPlayer.readFileCounts( ) );
  Serial.print( F( "Folders: " ) ); Serial.println( myDFPlayer.readFolderCounts( ) );
  printCurrentFileNum( true );
  Serial.print( F( "Volume : " ) ); Serial.println( myDFPlayer.readVolume( ) );  
  Serial.println( );
} // printPlayerInfo( )


void printCurrentFileNum( bool bPrintLabel ) {
  i16CurrentFile = myDFPlayer.readCurrentFileNumber( DFPLAYER_DEVICE_SD );
  if( bPrintLabel ) 
    Serial.print( F( "Curent : " ) );  
  Serial.println( i16CurrentFile, DEC );
} // printCurrentFileNum( )


/* 
 * We start and stop until we are at the track, then we don't stop.
 */
void playTrackNum( uint8_t u8Track ) {
  myDFPlayer.stop( );
  if( u8Track > 0 ) {
    myDFPlayer.start( );
    if( u8Track > 1 ) {
      for( uint8_t u8i=1; u8i < u8Track; ++u8i ) {
        myDFPlayer.stop( );
        myDFPlayer.next( );
      } // for
    } // if > 1
  } // if > 0
} // void playTrackNum( uint8_t )

void loop( ) {
  if( myDFPlayer.available( ) ) {
    printDetail( myDFPlayer.readType( ), myDFPlayer.read( ) ); //Print the detail message from DFPlayer to handle different errors and states.
  } // if
  
  // Check if pin is low, then play next 
  if( digitalRead( ButtonPIN ) == LOW ) {
    myDFPlayer.next( );                          // play next file
    Serial.println( F( "Next..." ) );
    delay( 1000 );                               // crude button debounce
  } // if

  // send '+', '-' or 'n' through serial port from PC
  if( Serial.available( ) ) {
    while( 1 ) {
      incomingByte = Serial.read( );
      if( incomingByte == '-' ) {
        myDFPlayer.volumeDown( );                  // volume Down
        break;
      } // if -
      
      if( incomingByte == '+' ) {
        myDFPlayer.volumeUp( );                    // volume Up
        break;
      } // if +
      
      if( incomingByte == '0' ) {
        playTrackNum( 0 );                         // should do nothing, just stop current
        printCurrentFileNum( false );
        break;
      } // if #

      if( incomingByte == '1' ) {
        playTrackNum( 1 );                         // play 1
        printCurrentFileNum( false );
        break;
      } // if #

      if( incomingByte == '2' ) {
        playTrackNum( 2 );                         // play 2
        printCurrentFileNum( false );
        break;
      } // if #
      
      if( incomingByte == '3' ) {
        playTrackNum( 3 );                         // play 3
        printCurrentFileNum( false );
        break;
      } // if #
      
      if( incomingByte == '4' ) {
        playTrackNum( 4 );                         // play 4
        printCurrentFileNum( false );
        break;
      } // if #
      
      if( incomingByte == '5' ) {
        playTrackNum( 5 );                         // play 5
        printCurrentFileNum( false );
        break;
      } // if #
      
      if( incomingByte == 'P' ) {
        myDFPlayer.stop( );                        // stop
        myDFPlayer.previous( );                    // play prior file
        Serial.print( F( "previous..." ) );
        printCurrentFileNum( false );
        break;
      } // if play previous
  
      if( incomingByte == 'p' ) {
        myDFPlayer.stop( );                        // stop
        myDFPlayer.previous( );                    // play prior file
        myDFPlayer.stop( );                        // stop
        Serial.print( F( "previous..." ) );
        printCurrentFileNum( false );
        break;
      } // if previous
      
      if( incomingByte == 'c' ) {
        myDFPlayer.stop( );                        // stop
        myDFPlayer.next( );
        myDFPlayer.stop( );
        myDFPlayer.previous( );
        
        //playTrackX( (uint8_t)i16CurrentFile );     // play current file
  
        Serial.print( F( "current..." ) );
        printCurrentFileNum( false );
        break;
      } // if current 
      
      if( incomingByte == 'N' ) {
        myDFPlayer.stop( );                        // stop
        myDFPlayer.next( );                        // play next file
        Serial.print( F( "next..." ) );      
        printCurrentFileNum( false );
        break;
      } // if play next  
        
      if( incomingByte == 'n' ) {
        myDFPlayer.stop( );                        // stop
        myDFPlayer.next( );                        // play next file
        myDFPlayer.stop( );                        // stop
        Serial.print( F( "next..." ) );      
        printCurrentFileNum( false );
        break;
      } // if next    
      
      if( incomingByte == 's' ) {
        myDFPlayer.stop( );                        // stop
        Serial.print( F( "stop..." ) );
        printCurrentFileNum( false );
        break;
      } // if stop

      if( incomingByte == 'S' ) {
        myDFPlayer.stop( );                        // stop
        myDFPlayer.start( );                       // start with the first file
        myDFPlayer.stop( );                        // stop
        Serial.print( F( "start..." ) );      
        printCurrentFileNum( false );
        break;
      } // if start 
      
      if( incomingByte == 'R' ) {
        myDFPlayer.stop( );                        // stop
        myDFPlayer.reset( );                       // reset
        Serial.print( F( "reset..." ) );
        printCurrentFileNum( false );
        break;
      } // if reset
  
      if( incomingByte == 'i' ) {
        printPlayerInfo( );
        break;
      } // if info
    } // while
  } // if serial available  

  myHeart.update( );
} // loop( )  


/* 
 * Routine to play a specific song
 * By Sonny22 on the https://forum.arduino.cc/
 * L=We are looking for a way to play mp3 file #x, this 
 * routine results in a Time Out Error
 */
void playTrackX( uint8_t u8Track ) {
  mySoftwareSerial.write( (byte)0x7E );
  mySoftwareSerial.write( (byte)0xFF );
  mySoftwareSerial.write( (byte)0x06 );
  mySoftwareSerial.write( (byte)0x03 );
  mySoftwareSerial.write( (byte)0x00 );
  mySoftwareSerial.write( (byte)0x00 );
  mySoftwareSerial.write( (byte)1 );
  mySoftwareSerial.write( (byte)0xEF );
  Serial.print("xxx");
} // playTrackX( uint8_t )


void printDetail( uint8_t u8Type, int iValue ){
  switch( u8Type ) {
    case TimeOut:
      Serial.println( F( "Time Out!" ) );
      break;
      
    case WrongStack:
      Serial.println( F( "Stack Wrong!" ) );
      break;
      
    case DFPlayerCardInserted:
      Serial.println( F( "Card Inserted!" ) );
      delay( 500 );
      printPlayerInfo( ); 
      break;
      
    case DFPlayerCardRemoved:
      Serial.println( F( "Card Removed!" ) );
      break;
      
    case DFPlayerCardOnline:
      Serial.println( F( "Card Online!" ) );
      break;
      
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
      
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
      
    case DFPlayerPlayFinished:
      Serial.print( F( "Number: " ) );
      Serial.print( iValue );
      Serial.println( F( " Play Finished!" ) );
      break;
      
    case DFPlayerError:
      Serial.print( F( "DFPlayerError: " ) );
      switch( iValue ) {
        case Busy:
          Serial.println( F( "Card not found" ) );
          break;
          
        case Sleeping:
          Serial.println( F( "Sleeping" ) );
          break;
          
        case SerialWrongStack:
          Serial.println( F( "Get Wrong Stack" ) );
          break;
          
        case CheckSumNotMatch:
          Serial.println( F( "Check Sum Not Match" ) );
          break;
          
        case FileIndexOut:
          Serial.println( F( "File Index Out of Bound" ) );
          break;
          
        case FileMismatch:
          Serial.println( F( "Cannot Find File" ) );
          break;
          
        case Advertise:
          Serial.println( F( "In Advertise" ) );
          break;
          
        default:
          break;
      } // switch iValue
      break;
      
    default:
      break;
  } // switch u8Type
} // void printDetail( uint8_t, int )
