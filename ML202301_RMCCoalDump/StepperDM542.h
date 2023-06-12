/*
 * StepperDM542 class
 * 
 * Control a DM542 to move a Stepper Motor implemented with a home sensor
 * and position control after homed.   
 * 
 * Author: Speed Muller, 2023.05.14
 */

#define BOTPOS  1300
#define MAXPOS  24000

class StepperDM542 {
private:
  uint8_t u8StepOutputPin;
  uint8_t u8DirOutputPin;
  uint8_t u8HomeInputPin;
  bool bPinHigh;

  uint32_t u32StepsSoFar   = 0;
  uint32_t u32CommandedPos = 1;
  uint32_t u32CurrentPos   = 1;
  bool bRunning  = false;
  bool bHomed    = false;
  bool bIsHoming = false;    // continue to run backwords until "home" is found
  bool bAtHome   = false;    // away from home by default
  
  unsigned long ulTimeoutOn;
  unsigned long ulTimeoutOff;
  unsigned long ulPrevious;


  // Write pin HI
  void writeHi( ) {
    digitalWrite( u8StepOutputPin, HIGH );
    bPinHigh = true;
  } // void writeHi( )


  // Write pin LOW
  void writeLo( ) {
    digitalWrite( u8StepOutputPin, LOW );
    bPinHigh = false;
  } // void writeLo( )


  // Read and write the opposite
  void toggle( ) {
    digitalWrite( u8StepOutputPin, !digitalRead( u8StepOutputPin ) );
  } // void toggle( )

  
public:
  /* 
   * Constructor 
   */
  StepperDM542( uint8_t u8TheStepPin, uint8_t u8TheHomePin, uint8_t u8TheDirPin, unsigned long ulTheTimeoutOn, unsigned long ulTheTimeoutOff ) {
    u8StepOutputPin = u8TheStepPin;
    u8DirOutputPin = u8TheDirPin;
    u8HomeInputPin = u8TheHomePin;
    ulTimeoutOn  = ulTheTimeoutOn;
    ulTimeoutOff = ulTheTimeoutOff;
    bPinHigh = false;
    u32StepsSoFar = 0;
    u32CommandedPos = 2;
    u32CurrentPos = 2;
    bRunning = false;
    bIsHoming = false;
    bHomed = false;
    bAtHome = false;
  } // constructor void Heartbeat( uint8_t, unsigned long, unsigned long )


  /* 
   * Set the pins and variables up 
   */
  void begin( ) {
    ulPrevious = millis( );
    pinMode( u8StepOutputPin, OUTPUT );
    pinMode( u8DirOutputPin, OUTPUT );
    pinMode( u8HomeInputPin, INPUT_PULLUP );
    writeLo( );                   // let's begin with off
    bAtHome = false;              // don't assume at home
    bIsHoming = false;
    bHomed = false;
    u32StepsSoFar = 0;
    u32CommandedPos = 3;
    u32CurrentPos = 3;            // assume zero, but not at thome
  } // void begin ( )


  /*
   * Return true if homing
   */
  bool isHoming( ) {
    return bIsHoming;
  } // bool isHoming( )


  /*
   * Let's go home
   */
  void goHome( ) {
    u32StepsSoFar = 0;    
    if( bHomed ) {
      setCommandedPosition( 0 );
      Serial.println( "Go to zero!" );
    } else {      
      bIsHoming = true;
      u32CommandedPos = 0;
      u32StepsSoFar = 0;
      Serial.println( "Find home..." );
    } // if previously homed
  } // void goHome( )


  /*
   * void cancel homing
   */
  void cancelHome( ) {    
    bIsHoming = false;
  } // void cancelHome( )


  /*
   * Move away from home to away position
   */
  void goToAway( ) {
    u32StepsSoFar = 0;
    if( bHomed ) {
      bIsHoming = false;
      setCommandedPosition( BOTPOS );
    } // if and only if homed before
  } // void goToAway( )
  

  /*
   * Set position to go to (limited to MAXSTEPS)
   */
  int32_t setCommandedPosition( uint32_t u32ThePos ) {
    u32StepsSoFar = 0;
    Serial.println( u32CurrentPos );
    Serial.println( u32CommandedPos );
    Serial.println( u32ThePos );

    if( u32ThePos <= BOTPOS ) {
      u32CommandedPos = u32ThePos;
    } else {
      Serial.println( F( "Not moving!" ) );
      return 0;
    } // if room to move

    Serial.print( F( "Going from " ) );
    Serial.print( u32CurrentPos );
    Serial.print( F( " to " ) );   
    Serial.print( u32CommandedPos );
    Serial.print( F( ", w/steps: [" ) );

    int32_t i32Steps = (int32_t)u32CommandedPos - (int32_t)u32CurrentPos;
    char buffer[80];
    sprintf( buffer, "%li", i32Steps );
    Serial.print( buffer );

    Serial.println( F( "]" ) );
    
    return i32Steps;
  } // int32_t setCommandedPosition( uint32_t )
  
  /*
   * Stop right there and then
   */
  void stop( ) {
    bIsHoming = false;
    u32CommandedPos = u32CurrentPos;
    u32StepsSoFar = 0;   
  } // void stop( )


  /*
   * Get some debug info
   */
  void printState( ) {
    Serial.print( F( "At: " ) );
    Serial.print( u32CurrentPos );
    Serial.print( F( ", going to " ) );    
    Serial.print( u32CommandedPos );
    Serial.print( F( ", thus far: " ) );    
    Serial.print( u32StepsSoFar );
    Serial.print( F( ", homed: " ) );    
    Serial.println( bHomed ? "y" : "n" );
  } // void printState( )


  /*
   * Return how for to go
   */
  int32_t getStepsToGo( ) {
    return u32CommandedPos - u32CurrentPos;
  } // int32_t getStepsToGo( )


  /*
   * Check state, and then if it is time, and then toggle if so
   */ 
  void update( ) {
    bool bGoingForward = true; // moving away from home by default
    unsigned long ulNow = millis( );
    
    bAtHome = !digitalRead( u8HomeInputPin );  // active low
    if( bAtHome ) {
      if( u32CommandedPos > 0 ) { 
      } else {
        u32CurrentPos = 0;
      } // only zero if heading towards home
    } // if home
    

    if( bIsHoming ) {
      if( bAtHome ) {
        bIsHoming = false;
        bHomed = true;
        u32CurrentPos = 0;
      } else {
        u32CurrentPos = 4;
        u32CommandedPos = 0;
      } // if at home
    } // if homing

    if( u32StepsSoFar >= MAXPOS ) {
      u32CurrentPos = u32CommandedPos;
    } // if too far!
      
    bGoingForward = u32CommandedPos >= u32CurrentPos;
    digitalWrite( u8DirOutputPin, bGoingForward );

    if( u32CurrentPos == u32CommandedPos ) {
      if( bRunning ) {
        Serial.println( "done" );
        bRunning = false;
      }       
    } else {
      if( bPinHigh ) {
        if( ulNow - ulPrevious > ulTimeoutOn ) {
          ulPrevious = ulNow; 
          if( u32CurrentPos > u32CommandedPos ) {
            u32CurrentPos--;
          } else {
            u32CurrentPos++;
          }
          writeLo( );
        } // if
      } else {
        if( ulNow - ulPrevious > ulTimeoutOff ) {
          ulPrevious = ulNow; 
          writeHi( );
          u32StepsSoFar++;
        } // if      
      } // else
      bRunning = true;
    } // if pulses to go    
  } // void update( )
}; // class StepperDM542
