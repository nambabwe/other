 /*
 * Control up to 16 servos (until providing more input pins in the array)
 * 
 * Toggle the pin and LED on pin 13 every 0.1 seconds, without blocking in loop()
 * v0.01 moving up to NUMSERVO servos
 * v0.02 user input to MIN and MAX with #, M and m commands:
 *   Example:   #00  (selects first servo)
 *              M48  (sets Servo 00 MAX to 72
 * v0.03 nnn and ttt commands, fixing M22 to throw as well. New T and N commands, case
 *       does not matter. Show HELP now.
 */

#define DEBUG    
//#undef  DEBUG
// VERSION tells us about this code, see it on the Serial Monitor (Ctrl+shift+M)
// and know what to search for on you computer...if you were looking for the code
#define VERSION "2020.01.05 Wilson Servo v0.04"
#define BAUDRATE   115200


#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver servoDriver = Adafruit_PWMServoDriver();

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMINPULSE  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAXPULSE  575 // this is the 'maximum' pulse length count (out of 4096)

#define EEPROMSERVO      2 // location of first SERVOMAX

// TIMEOUT in milliseconds, the now-previous will tell if it is time to do something
#define TIMEOUT        100
#define BEATTIMEOUT    500

#define COLUMNWIDTH     80

// LEDPIN tells which in are we toggling
#define LEDPIN    13

long int now;                     // keep the time we read now
long int previous;                // keep last time, the difference is compared to the TIMEOUT
long int previousBeat;            // keep last time, the difference is compared to the TIMEOUTBEAT

// is it THROWN ... thus false for NORMAL
#define F false
#define T true

typedef enum { NORMAL, THROWN, MID } STATE;

char NUMSERVOS = 16; // MAX 7 according to the lines below, add more if needed in each row
unsigned int currentPosition[ ]   = {  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90 };
unsigned int commandedPosition[ ] = {  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90 };
STATE servoState[]                 = { MID, MID, MID, MID, MID, MID, MID, MID, MID, MID, MID, MID, MID, MID, MID, MID };
//STATE servoState[]                 = { NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL };
bool servoPower[]                 = {   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T,   T };
const char INPUTS[ ]              = {  A0,  A1,  A2,  A3,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,   9,   9 };
unsigned char SERVOMAX[ ]         = {  70,  95, 110, 110,  90, 112,  90, 110, 102, 110, 110,  90,  90,  90,  90,  90 };
unsigned char SERVOMIN[ ]         = { 110,  78,  66,  70, 102,  95,  78,  80,  80,  70,  70,  90,  90,  90,  90,  90 };

int currentServo = -1;
String inbound;
int dotCounter   = 0;

// This runs once
void setup( ) { 
  pinMode( LEDPIN, OUTPUT );      // make the pin an OUTPUT

  for( char i=0; i < NUMSERVOS; i++ ) {
    pinMode( INPUTS[ i ], INPUT_PULLUP ); // make the pin an INPUT with the built in pullup
  } // for

  for( int i=0; i<10; i++ ) {
    digitalWrite( LEDPIN, !digitalRead( LEDPIN ) );    
    delay( 150 );
  }

  Serial.begin( BAUDRATE );         // set the serial baud rate. remember to do the same on the computer application

  unsigned char firstTime = EEPROM.read( 0 );
  Serial.println( VERSION );      // show the version, helpful when looking for code later

  if( firstTime == 0x5D ) {
    // read all our numbers
    Serial.println( "Not first time..." );
    NUMSERVOS = EEPROM.read( 1 );
    for( int i = 0; i<NUMSERVOS; i++ ) {
      SERVOMAX[i] = EEPROM.read( EEPROMSERVO + (i*2) );
      SERVOMIN[i] = EEPROM.read( EEPROMSERVO + (i*2) + 1 );
    } // for   
  } else {
    // create a fresh set of numbers
    Serial.println( "First time!" );
    EEPROM.write( 1, NUMSERVOS );
    for( int i = 0; i<NUMSERVOS; i++ ) {
      Serial.print( SERVOMAX[i] ); Serial.print( " " );
      EEPROM.write( EEPROMSERVO + (i*2), SERVOMAX[i]);
      Serial.println( SERVOMIN[i] );
      EEPROM.write( EEPROMSERVO + (i*2)+1, SERVOMIN[i]);
    } // for
    EEPROM.write( 0, 0x5D );
  } // if
  
  servoDriver.begin();
  servoDriver.setPWMFreq( 50 );  // This is the maximum PWM frequency
  
  showTable( );
  showHelp( );
  Serial.println( "\nServo number (0 based) please (#dd)? " );

  digitalWrite( LEDPIN, HIGH );   // turn the pin on
  now = previous = previousBeat = millis( );      // get the millis since starup 
} // setup( )


void servoOn( char servoNum ) {
  if( servoNum < NUMSERVOS ) {
    servoPower[ servoNum ] = T;
  } // if valid
} // servoOn( char )


void servoOff( char servoNum ) {
  if( servoNum < NUMSERVOS ) {
    servoPower[ servoNum ] = F;
  } // if valid  
} //servoOff( char )

void showTable( ) {
  Serial.println( String( "ServoNum: " ) + /*Dec(Thrown, Normal) */ String( "Hex[Thrown, Normal]" ) );
  for( int i = 0; i<NUMSERVOS; i++ ) {
    Serial.print( i, DEC ); Serial.print( ", pin:" ); Serial.print( INPUTS[ i ], DEC ); Serial.print( ", [" );
//    Serial.print( SERVOMAX[i], DEC ); Serial.print( ", " );
//    Serial.print( SERVOMIN[i], DEC ); Serial.print( ") [" );
    Serial.print( SERVOMAX[i], HEX ); Serial.print( ", " );
    Serial.print( SERVOMIN[i], HEX ); Serial.println( "]" );
  } // for
} // showTable( )

void showEEPROM( ) {
  Serial.print( "Check: " ); Serial.println( EEPROM.read( 0 ), HEX );
  Serial.print( "NumServos: " ); Serial.println( EEPROM.read( 1 ), DEC );
  
  for( int i = 0; i<NUMSERVOS; i++ ) {
    Serial.print( i, DEC ); Serial.print( ": (" );
    Serial.print( EEPROM.read( EEPROMSERVO + (i*2) ), DEC );
    Serial.print( ", " );
    Serial.print( EEPROM.read( EEPROMSERVO + (i*2) + 1 ), DEC );
    Serial.print( ") [" );    
    Serial.print( EEPROM.read( EEPROMSERVO + (i*2) ), HEX );
    Serial.print( ", " );
    Serial.print( EEPROM.read( EEPROMSERVO + (i*2) + 1 ), HEX );
    Serial.println( "]" );
  } // for
} // showEEPROM( )

void reportServo( unsigned char myServo ) {
  Serial.print( "Using Servo @ pos ");
  Serial.print( myServo+1, DEC );
  Serial.print( " [#");
  Serial.print( myServo, DEC );
  Serial.print( "], "); //DEC: THR: ");
//  Serial.print( SERVOMAX[ myServo ], DEC );
//  Serial.print( ", NOR: ");
//  Serial.print( SERVOMIN[ myServo ], DEC );
//  Serial.print( ", 
  Serial.print( "HEX: THR: ");
  Serial.print( SERVOMAX[ myServo ], HEX );
  Serial.print( ", NOR: ");
  Serial.print( SERVOMIN[ myServo ], HEX );
  Serial.print( ", @: ");
  Serial.print( currentPosition[ myServo ], HEX );
  Serial.print( ", state: ");
  switch( servoState[ myServo ] ) {
    case NORMAL:
      Serial.print( "Normal" );
      break;
    case THROWN:
      Serial.print( "Thrown" );
      break;
    case MID:
      Serial.print( "Middle" );
      break;
    default:
      Serial.print( "Unknown!!!" );
      break;
  } // switch
  Serial.print( ", pwr: ");
  Serial.println( servoPower[ myServo ] ? "On" : "Off" );
  
  Serial.println( "To update THROWN(max): txx  or NORMAL(min): nxx  both in hexadecimal." );
} // reportServo

unsigned char hexCharsToValue( unsigned char first, unsigned char second ) {
  unsigned char value = 0;
  if( ( first >= '0' ) && ( first <= '9') ) {
    value = 16 * (first - '0');
  } else
    if( ( first >= 'a' ) && ( first <= 'f') ) {
      value = 16 * (10 + first - 'a');
    } else
      if( ( first >= 'A' ) && ( first <= 'F') ) {
        value = 16 * (10 + first - 'A');
      }
  
  if( ( second >= '0' ) && ( second <= '9') ) {
    value = value + (second - '0');
  } else
    if( ( second >= 'a' ) && ( second <= 'f') ) {
      value += (10 + second - 'a');
    } else
      if( ( second >= 'A' ) && ( second <= 'F') ) {
        value += (10 + second - 'A');
      }
   
  return value;
} // hexCharsToValue( unsigned char, unsigned char )


// this determines the position to go to based on the state, and increment it there one degree at a time, until the 
// commanded position is reached.
void setServo( char servoNum ) {
  if( servoNum < NUMSERVOS ) {
//    Serial.print( servoNum, DEC ); 
//    Serial.print( " " ); 
    if( servoPower[ servoNum ] ) {
      switch( servoState[ servoNum ] ) {
        case NORMAL: 
          commandedPosition[ servoNum ] = SERVOMIN[ servoNum ];
          break;
        case THROWN: 
          commandedPosition[ servoNum ] = SERVOMAX[ servoNum ];
          break;
        case MID: 
          commandedPosition[ servoNum ] = ( SERVOMAX[ servoNum ] + SERVOMIN[ servoNum ] ) / 2;
          break;     
        default:
          break;
      }
      if( currentPosition[ servoNum ] < commandedPosition[ servoNum ] ) { // if smaller
        servoDriver.setPWM( servoNum, 0, angleToPulse( ++currentPosition[ servoNum ] ) );
      } else { // if bigger
        if( currentPosition[ servoNum ] > commandedPosition[ servoNum ] ) {
          servoDriver.setPWM( servoNum, 0, angleToPulse( --currentPosition[ servoNum ] ) );
        } else { 
          // do nothing
        } // if else
      } // if
    } else { // if not powered
      servoDriver.setPWM( servoNum, 0, 0 ); 
    }
  } // if a real servo number
} // setServo( char )


/*
 * from http://robojax.com/learn/arduino/?vid=robojax-PCA6985
 * angleToPulse(int ang)
 * gets angle in degree and returns the pulse width
 * also prints the value on seial monitor
 * written by Ahmad Nejrabi for Robojax, Robojax.com
 */
int angleToPulse( int angle ){
  int pulse = map( angle, 0, 180, SERVOMINPULSE, SERVOMAXPULSE ); // map angle of 0 to 180 to Servo min and Servo max 
  //Serial.print( "Angle: " ); Serial.print( angle );
  //Serial.print( " pulse: " ); Serial.println( pulse );
  return pulse;
} // angleToPulse( int )


void showHelp( ) {
  Serial.println( "\nWe need #dd, then ttt (throw), nnn (normal), txx (set throw) or nxx (set normal)! \n??? for help, EEE for EEPROM table");
} // showHelp( )


// This runs repeatedly, forever
void loop( ) {
  if( Serial.available( ) ) {
    inbound = Serial.readString( );
    inbound.trim( );
    if( inbound.length( ) == 3 ) {
      Serial.println( inbound );
      if( inbound[0] == '?' ) {
        showTable( );
        showHelp( );
      } else if( inbound[0] == 'E' ) {
        showEEPROM( );
        showHelp( );
        
      } else if( ( inbound[0] == '!' ) && ( inbound[1] == '~' ) && ( inbound[2] == '!' ) ) { // reset
            EEPROM.write( 1, 16 );
            Serial.println( "\n\nThe big factory reset about to happen!!!\n\n" );
            // do more to factory reset!
      } else if( inbound[0] == '#' ) {
        if( inbound[1] == '1' ) {
          currentServo = 10;     
        } else {
          if( inbound[1] == '0' ) {
            currentServo = 0;
          } else {
          currentServo = -1;
          }          
        }  
        if( currentServo != -1 ) {
          if( ( inbound[2] >= '0' ) && ( inbound[2] <= '9' ) ) {
            currentServo += inbound[2] - '0';
            if( currentServo < NUMSERVOS ) {
              reportServo( currentServo );
            } else {
              Serial.print( "We need to set a Servo < " );
              Serial.print( NUMSERVOS, DEC );
              Serial.print( " " );      
            }
          } else {
            Serial.println( "err: We got no Servo to set!" );
          } // valid servo
        } else {
          Serial.println( "err: We got no Servo to set!" );
        } // valid servo
      } else {
        if( currentServo != -1 ) {   // we check if we have a valid servo in hand
          if( ( toupper( inbound[0] ) == 'G' ) && ( toupper( inbound[1] ) == 'G' ) ) { // toGGle
            // toggle state
            if( servoState[ currentServo ] == NORMAL ){
              servoState[ currentServo ] = THROWN; // toGGle state
            } else {
              servoState[ currentServo ] = NORMAL; // toGGle state
            }
            reportServo( currentServo );
            // TODO: turn servo 
          } else
          if( ( toupper( inbound[0] ) == 'O' ) && ( toupper( inbound[1] ) == 'O' ) ) { // ON
            // set normal
            servoOn( currentServo );  // Turn on
            reportServo( currentServo );
            // TODO: turn servo 
          } else          
          if( ( toupper( inbound[0] ) == 'F' ) && ( toupper( inbound[1] ) == 'F' ) ) { // OFF
            // turnOff
            servoOff( currentServo ); // Turn off
            reportServo( currentServo );
            // TODO: turn servo off
          } else          
          if( ( toupper( inbound[0] ) == 'N' ) && ( toupper( inbound[1] ) == 'N' ) ) { // NORMAL
            // set normal
            servoState[ currentServo ] = NORMAL;  // Normal
            reportServo( currentServo );
          } else          
          if( ( toupper( inbound[0] ) == 'T' ) && ( toupper( inbound[1] ) == 'T' ) ) { // THROWN
            servoState[ currentServo ] = THROWN;  // Thrown
            reportServo( currentServo );
          } else
          if( ( toupper( inbound[0] ) == 'M' ) && ( toupper( inbound[1] ) == 'M' ) ) { // MID
            servoState[ currentServo ] = MID;  // MID
            reportServo( currentServo );
          } else
          if( ( toupper( inbound[0] ) == 'P' ) && ( toupper( inbound[1] ) == 'P' ) ) { // Power
            servoPower[ currentServo ] != servoPower[ currentServo ];  // Power
            Serial.print( "\nToggling power to " );
            Serial.println( servoPower[ currentServo ] ? "On" : "Off" );
            reportServo( currentServo );
          } else
          if( toupper( inbound[0] ) == 'T' ) {
              Serial.println( "Updating MAX or Thrown...");  

              // get inbound[1] * 16 + inbound[2] as hexadecimal, make it decimal and save to EEPROM after loading it ino SERVOMAX[ currentServo ]
              unsigned char value = hexCharsToValue( inbound[1], inbound[2] );             
              SERVOMAX[ currentServo ] = value;
              EEPROM.write( EEPROMSERVO + (2 * currentServo), SERVOMAX[currentServo] );
              servoState[ currentServo ] = THROWN;  // True is Thrown
//              commandedPosition[currentServo] = value;
//              Serial.print( currentServo ); Serial.print( ":" ); Serial.println( commandedPosition[currentServo] );

              reportServo( currentServo );
          } else {
            if( inbound[0] == 'n' ) {
              Serial.println( "Updating MIN or Normal...");  

              // get inbound[1] * 16 + inbound[2] as hexadecimal, make it decimal and save to EEPROM after loading it ino SERVOMAX[ currentServo ]
              unsigned char value = hexCharsToValue( inbound[1], inbound[2] );             
              SERVOMIN[ currentServo ] = value;
              EEPROM.write( EEPROMSERVO + (2 * currentServo + 1), SERVOMIN[currentServo] );
              servoState[ currentServo ] = NORMAL;  // False is Normal

//              Serial.print( currentServo ); Serial.print( ":" ); Serial.println( commandedPosition[currentServo] );
              reportServo( currentServo );
            } else {
              Serial.println( "Not a valid command in use yet!");  
            }
          }
        } else {
          Serial.println( "Servo not selected yet!" );
        } // servo -1
      } // if 
    } else { // if len == 3
      showHelp( );      
    } // not len == 3
  } // if len == 3
  now = millis();
  
  if ( now - previous >= TIMEOUT ) {
    previous = now;
/*    Serial.print( '.' );
    ++dotCounter;
    if(dotCounter >= COLUMNWIDTH ) {
      dotCounter = 0;
      Serial.println( );
    } // if dotCounter
*/
    for( char i = 0; i < NUMSERVOS; i++ ) {
      setServo( i );
    } // for
  } // if TIMEOUT  

  if ( now - previousBeat >= BEATTIMEOUT ) {
    previousBeat = now;
    digitalWrite( LEDPIN, !digitalRead( LEDPIN ) );    
  } // if TIMEOUT  

  bool toggle = false;
  for( char i = 0; i < NUMSERVOS; i++ ) {
    if( digitalRead( INPUTS[ i ] ) == LOW ) {
      Serial.print( "\nButton toggling ");
      Serial.print( i+1, DEC );
      Serial.print( " to ");      
      if( servoState[ i ] == NORMAL ) {
        servoState[ i ] = THROWN;
        Serial.println( "Thrown");      
      } else {
        servoState[ i ] = NORMAL;
        Serial.println( "Normal");      
      }
      toggle = true;
      
    } // if  
  } // for
  if( toggle ) delay( 500 ); // not a Speed favorite, debouncing needed in a differenct way
} // loop( )

// End of File
