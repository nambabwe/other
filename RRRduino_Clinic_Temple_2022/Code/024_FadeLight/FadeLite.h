// FadeLite.h

#define UPDATETIME      20

class FadeLite {
  // Class Member Variables
  // These are initialized at startup
  uint8_t       u8LedPin;         // the number of the LED pin
  unsigned long ulFadeTime;       // how long to spend fading
  uint8_t       u8StepSize;       // size of steps to take to fade
  unsigned long ulUpdateTime;     // how often to update

  // These maintain the current state
  uint8_t u8LedState;             // ledState used to set the LED, 0 off, 1 on, 2 fading to on, 3 fading to off
  uint8_t u8LedValue;             // 0 thru 255 for full off thru full on
  unsigned long ulPrevious;       // will store last time LED was updated

  void output( ) {
    analogWrite( u8LedPin, u8LedValue );   
  } // Outout

  public:
  // Constructor - creates a Fader 
  // and initializes the member variables and state
  FadeLite( int thePin, long int theFadeTime ) {
    u8LedPin = thePin;
    pinMode( u8LedPin, OUTPUT ); 
    ulUpdateTime = UPDATETIME;
    ulFadeTime = theFadeTime;
    u8StepSize = uint8_t( 256 * ( (1.0 * UPDATETIME ) / ( 1.0 * ulFadeTime ) ) ); 
    if( u8StepSize < 1 ) { 
      u8StepSize = 1; 
      ulUpdateTime = ulFadeTime / 255;
    } // if
        
    fullOff( );
    ulPrevious = millis( );
  } // FadeLite constructor


  void fullOn( ) {      
    u8LedValue = 255;
    u8LedState = 1;
    output( );  
  } // fullOn( )


  void fullOff( ) { 
    u8LedValue = 0;
    u8LedState = 0;
    output( );  
  } // fullOff( )


  void reset( ) {
    fullOff( );
    ulPrevious = millis( );  
  } // reset( )


  bool isOn( ) {
    return u8LedState == 1;
  } // isOn( )


  bool isOff( ) {
    return u8LedState == 0;
  } // isOff( )


  void toggle( ) {
    if( u8LedState == 0 ) {
      //FullOn( );//
      turnOn( );
    } else {
      if( u8LedState == 1 ) {
      //FullOff( ); //
      turnOff( );
      } // if
    } // if      
  } // toggle( )


  void turnOn( ) {
    u8LedState = 2;
  } // turnOn( )


  void turnOff( ) {
    u8LedState = 3;
  } // turnOff( )


  void update( ) {
    // check to see if it's time to change the state of the LED
    unsigned long ulNow = millis( );
     
    if( ulNow - ulPrevious >= ulUpdateTime ) {
      ulPrevious = ulNow;

      switch( u8LedState ) {
      case 0:
        break;

      case 1:        
        break;

      case 2:  
        u8LedValue += u8StepSize;
        if( u8LedValue >= 255 ) {
          u8LedValue = 255;   
          u8LedState = 1;
        } // if on 
        break;

      case 3:        
        u8LedValue -= u8StepSize;
        if( u8LedValue <= 0 ) {
          u8LedValue = 0;   
          u8LedState = 0;
        } // if of       
        break;          
      }; // switch

      output( );                    // Update the actual LED
    } // if, time for change yet?
  } // update( )
}; // FadeLite

