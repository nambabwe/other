/* Tulsa 2021.10.08 Arduino for Beginners Make and Take Clinic
 * When the boom is down, turn the tip of the boom's light on,
 *   and flash the two other lights on the boom (as well as the ones  under the hoods).
 * Author: Speed Muller, Date: 2021.10.08, 08:07
 */

#define VERSION_STR "007_Boom_LEDs, 2021.10.08, 08:07, ver 0.01"
#define BAUD 115200

// heartbeat
#define LED1       13

// Boom LEDs
#define LED2       12
#define LED3       11
#define LED4       10

#define ONTIME1   100
#define ONTIME2   500
#define ONTIME3   500
#define ONTIME4   500

#define OFFTIME1  900
#define OFFTIME2    0
#define OFFTIME3  500
#define OFFTIME4  500

#define DELAY       0

/* 
 *  Light with time on and time off specified
 */
class Light {
private:
  uint8_t pin;
  uint16_t timeOn, timeOff;
  bool state = false;
  unsigned long now, before;
  
public:
  /*
   * Constructor
   */
   Light( uint8_t thePin, uint16_t theTimeOn, uint16_t theTimeOff ) {
    pin = thePin;
    timeOn = theTimeOn;
    timeOff = theTimeOff;
    pinMode( pin, OUTPUT );
  } // constructor Light( uint8_t, uint16_t, uint16_t )

  /*
   * Destructor
   */
  ~Light( ) { } // destructor ~Light( )

 
  /*
   * begin( ): set the start time
   */  
  void begin( ) {
    now = before = millis( );
  } // void begin( )


  /*
   * on( ): turn pin HIGH and set state to true
   */  
  void on( ) {
    digitalWrite( pin, HIGH );
    state = true;
  } // void on( )

  
  /*
   * off( ): turn pin LOW and set state to false
   */   
  void off( ) {
    digitalWrite( pin, LOW );
    state = false;
  } // void off( )


  /*
   * update( ): from the state it is in, determine if the time has expired and flip the state
   */  
  void update( ) {
    now = millis( );
    if( state == true ) {
      if( now - before >= timeOn ) {
        before = now;
        off( );
      } // if time on is over
    } else {
     if( now - before >= timeOff ) {
        before = now;
        on( );
      } // if time off is over       
    } // if on or off
  } // void update( )
  
}; // class Light( )


Light light1( LED1, ONTIME1, OFFTIME1 );
Light light2( LED2, ONTIME2, OFFTIME2 );
Light light3( LED3, ONTIME3, OFFTIME3 );
Light light4( LED4, ONTIME4, OFFTIME4 );


void setup( ) {
  Serial.begin( BAUD );
  
  light1.begin( );
  delay( DELAY );
  
  light2.begin( );
  delay( DELAY );
  
  light3.begin( );
  delay( ONTIME3 );
  
  light4.begin( );
  
  Serial.println( );
  Serial.println( VERSION_STR );
} // setup( )


void loop( ) {
  light1.update( );
  light2.update( );
  light3.update( );
  light4.update( );
} // loop( )

//
// Connect the "hood" LEDs with their own resistors to the same LED2 and LED3 pins and GND
//
