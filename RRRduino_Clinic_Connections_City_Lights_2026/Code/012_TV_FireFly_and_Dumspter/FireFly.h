/*
 * FireFly class
 * Blink and fade out.
 *    
 * Author: Speed Muller, 2023.10.07
 */

#define DISABLED            0
#define FADING            254
#define READY             255

#define MAXVAL            255
#define UPDATETIME          5
#define DECREASEBY          3

class FireFly {
private:
  uint8_t       u8PWMPin;
  uint8_t       u8RandomMin;
  uint8_t       u8Mode;
  uint8_t       u8OutputVal;
  unsigned long ulOffRandomDelayMin;
  unsigned long ulOffRandomDelayMax;
  unsigned long ulNextOnDelay;
  unsigned long ulPrevious;

public:
  // Constructor
  FireFly( uint8_t u8ThePWMPin, unsigned long ulTheOffRandomDelayMin, unsigned long ulTheOffRandomDelayMax, uint8_t uTheRandomMin ) {
    u8PWMPin = u8ThePWMPin;
    ulOffRandomDelayMin = ulTheOffRandomDelayMin;
    ulOffRandomDelayMax = ulTheOffRandomDelayMax;
    u8RandomMin = uTheRandomMin;
    u8Mode = READY;
    u8OutputVal = 0;
    ulNextOnDelay = ulTheOffRandomDelayMin;
  } // constructor void FireFly( uint8_t, unsigned long, unsigned long )


  // calculate the next random ON event
  void calcNextRandom( ) { 
    ulNextOnDelay = random( ulOffRandomDelayMin, ulOffRandomDelayMax );
  } // void calcNextRandom( )


  // Set the pin and time up
  void begin( ) {
    calcNextRandom( );
    pinMode( u8PWMPin, OUTPUT );
    writeLo( );                   // let's begin with off
    ulPrevious = millis( );
  } // begin ( )


  // Write HI
  void writeHi( ) {
    u8OutputVal = MAXVAL;
    output( );
  } // void writeHi( )


  // Write LO
  void writeLo( ) {
    u8OutputVal = 0; 
    output( );
  } // void writeLo( )


  // output PWM value to ~pin
  void output( ) {
    analogWrite( u8PWMPin, u8OutputVal );
  } // void output( )


  // Check fading, if zero calculate next random time and next random brightness
  void update( ) {
    unsigned long ulNow = millis( );

    if( u8Mode == FADING ) {
      if( ulNow - ulPrevious > UPDATETIME ) {
        ulPrevious = ulNow;
        if( u8OutputVal > DECREASEBY ) {
          u8OutputVal -= DECREASEBY;
        } else {
          u8OutputVal = 0; // zero if close enough 
        } // if able to decrease else off
        output( );

        if( u8OutputVal == 0 ) {
          calcNextRandom( );  
          
          u8Mode = READY;
          //.Serial.println( );
          //.Serial.println( ulNextOnDelay );
        } // if off
      } // if time to update
    } // if mode decreasing (FADING)

    if( u8Mode == READY ) {
      if( ulNow - ulPrevious > ulNextOnDelay ) {
        ulPrevious = ulNow;
        u8Mode = FADING; 
        u8OutputVal = random( u8RandomMin, MAXVAL );
        //.Serial.println( );
        //.Serial.println( u8OutputVal );
        output( );
      } // if time
    } // if mode READY
  } // void update( )
}; // class Firefly

/* end of file */
