#define VERSION_STR "010_DFPlayer_MP3, St Louis, ver 0.01"

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

// function prototypes (the Arduino tool does this by default, other C compiler setups may not!
void setMP3Volume( uint8_t );
void printMenu( );
void printPlayerInfo( );
void playTrackNum( uint8_t );
void playTrackX( uint8_t u8Track );
void printDetail( uint8_t u8Type, int iValue );

// define constants
#define BAUDRATE               57600
#define DFPLAYER_BAUDRATE       9600

#define LEDPIN                    13
#define SWRXPIN                    2
#define SWTXPIN                    3

#define MAXVOLUME                 30

#define LEDPIN1                   13
#define TIMEOFF                  617
#define TIMEON                    50

#define BCNPIN1                   A5
#define BCNTIMEOFF              1950
#define BCNTIMEON                 50

#define ECHOPIN                    7 // attach pin D4 Arduino to pin Echo of the HC-SR04
#define TRIGPIN                    8 // attach pin D5 Arduino to pin Trig of the HC-SR04

// defines objects
SoftwareSerial mySoftwareSerial( SWRXPIN, SWTXPIN );  // RX pin and TX pin to DFPlayer
DFRobotDFPlayerMini myDFPlayer;
Heartbeat myHeart = Heartbeat( LEDPIN1, TIMEON, TIMEOFF );
Heartbeat myBeacon = Heartbeat( BCNPIN1, BCNTIMEON, BCNTIMEOFF );

// defines variables
bool notReady = true;

// byte
uint8_t volume = 8; //= ( 2 * MAXVOLUME ) / 3;              // 2/3rd of max, change this to a value you like
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
  myBeacon.begin( );
  pinMode( BUTTONPIN, INPUT_PULLUP );
  
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
  
  delay( 100 );
  
  setMP3Volume( volume );                        // set volume value, 0 to 30
  delay( 500 );
  printPlayerInfo( );
  
  Serial.println( "Play first..." );
  myDFPlayer.start( );                           // play the first file
  i16CurrentFile = myDFPlayer.readCurrentFileNumber( DFPLAYER_DEVICE_SD );
  
  printMenu( );

  Serial.println( );
  Serial.println( F( "Current, Filtered" ) );  
} // setup( )


/*
 * loop( ) called from main.cpp in a for(;;) loop. Keeps calling until code is blocked, reset, or power removed
 */
void loop( ) {
  if( myDFPlayer.available( ) ) {
    printDetail( myDFPlayer.readType( ), myDFPlayer.read( ) ); //Print the detail message from DFPlayer to handle different errors and states.
  } // if

  // send commands through serial port from PC (the USB cable provides a COMx, ttyUSBx or ttyACMx interface)
  if( Serial.available( ) ) {
// processing of ALL incoming commands are listed in the include file:
#include "serialinterface.h"
  } // if serial available  

  myHeart.update( );
  myBeacon.update( );
  myDistanceSensor.update( );
#ifdef ONE
  if( myDistanceSensor.checkAndClearTriggered( false ) ) {
    uint32_t u32Trigger = myDistanceSensor.u32GetTriggered( );
    Serial.println( u32Trigger );
    if( u32Trigger > 15000 ) {
      //playTrackNum( 1 );
    } else {
      if( u32Trigger > 10000 ) {
        playTrackNum( 2 );
      } else {
        if( u32Trigger > 5000 ) {
          playTrackNum( 3 );
        } else {
          if( u32Trigger > 2000 ) {
            playTrackNum( 4 );
          } else {
            if( u32Trigger > 1000 ) {
              playTrackNum( 5 );
            } else {
              if( u32Trigger > 500 ) {
                playTrackNum( 7 );
              }
            }
          }
        }
      }
    }    
    (void)myDistanceSensor.checkAndClearTriggered( true );
  } // if
#endif
} // loop( )


void setMP3Volume( uint8_t u8TheVolume ) {
  if( u8TheVolume < 31 ) {
    uint8_t u8CurrentVolume = myDFPlayer.readVolume( );
    while( u8CurrentVolume != u8TheVolume ) {
      if( u8CurrentVolume > u8TheVolume ) {
        myDFPlayer.volumeDown( );                  // volume Down
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


void printDetail( uint8_t u8Type, int iValue ) {
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
