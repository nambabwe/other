#define VERSION_STR "008_STL_DFPlayer_with_Beacon ver 0.001"

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

#define BAUDRATE              115200
#define DFPLAYER_BAUDRATE       9600

#define TIMEOUT                  500

#define SWRXPIN                    2
#define SWTXPIN                    3
#define LEDPIN                    13
#define BUTTONPIN                 A3

#define MAXVOLUME                 30

#define LEDPIN1                   13
#define TIMEOFF                  617
#define TIMEON                    50

#define BCNPIN1                   A5
#define BCNTIMEOFF              1950
#define BCNTIMEON                 50

#define ECHOPIN                    4 // attach pin D4 Arduino to pin Echo of 2nd HC-SR04
#define TRIGPIN                    5 // attach pin D5 Arduino to pin Trig of 2nd HC-SR04

SoftwareSerial mySoftwareSerial( SWRXPIN, SWTXPIN );  // RX pin and TX pin to DFPlayer
DFRobotDFPlayerMini myDFPlayer;

Heartbeat myHeart = Heartbeat( LEDPIN1, TIMEON, TIMEOFF );
Heartbeat myBeacon = Heartbeat( BCNPIN1, BCNTIMEON, BCNTIMEOFF );

// defines variables
byte volume = ( 2 * MAXVOLUME ) / 3;              // 2/3rd of max, change this to a value you like
byte incomingByte;
bool notReady = true;
uint16_t u16CurrentFile = 1;

int32_t i32Duration; // variable for the 1st duration of sound wave travel
int16_t i16Distance;  // variable for the 1st distance measurement

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
  
  delay( 500 );
  myDFPlayer.volume( volume );                    // set volume value, 0 to 30
  delay( 500 );
  Serial.println( "Play first file..." );
//  myDFPlayer.start( );                           // play the first file
  myDFPlayer.play( u16CurrentFile );                           // play the first file
} // setup( )


void loop( ) {
  if( myDFPlayer.available( ) ) {
    printDetail( myDFPlayer.readType( ), myDFPlayer.read( ) ); //Print the detail message from DFPlayer to handle different errors and states.
  } // if
  
  // Check if pin is low, then play next 
  if( digitalRead( BUTTONPIN ) == LOW ) {
    myDFPlayer.next( );                          // play next file
    Serial.println( "Next..." );
    delay( 1000 );                               // crude button debounce
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
      //u16CurrentFile = myDFPlayer.readCurrentFileNumber( myDFPlayer.device );
      Serial.print( "next..." );
      Serial.println( u16CurrentFile, DEC );
    } // if next    
  } // if serial available  

  myHeart.update( );
  myBeacon.update( );
} // loop( )  


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
