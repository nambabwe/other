/*
 * Tulsa 2021.10.08 Arduino for Beginners Make and Take Clinic
 * 
 * To flash three LEDs, at pin 13, pin 11 and pin 10 in sequence
 * 
 * Author: Speed Muller, Date: 2021.10.08, 08:05
 */


#define VERSION_STR "005_sequence_OOP, 2021.10.08, 08:05, ver 0.01"

/*
 * Light with time on and time off specified
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


#define BAUD 115200

#define LED1       13
#define LED2       11
#define LED3       10

#define ONTIME1   250
#define ONTIME2   250
#define ONTIME3   250

#define OFFTIME1  500
#define OFFTIME2  500
#define OFFTIME3  500

#define DELAY     250

Light light1( LED1, ONTIME1, OFFTIME1 );
Light light2( LED2, ONTIME2, OFFTIME2 );
Light light3( LED3, ONTIME3, OFFTIME3 );

void setup( ) {
  Serial.begin( BAUD );
  
  light1.begin( );
  delay( DELAY );
  
  light2.begin( );
  delay( DELAY );
  
  light3.begin( );
  
  Serial.println( );
  Serial.println( VERSION_STR );
} // setup( )

void loop( ) {
  light1.update( );
  light2.update( );
  light3.update( );
} // loop( )

//
// And now Light can move to light.h and we can use all kinds of lights: random, fading, flickering, etc!
//
