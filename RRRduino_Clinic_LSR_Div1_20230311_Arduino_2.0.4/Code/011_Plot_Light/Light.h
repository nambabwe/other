/*
 * Light class with time on and time off specified
 * And poweron and poweroff to enable and disable
 */

class Light {
private:
  uint8_t pin;
  uint16_t timeOn, timeOff;
  bool state = false;
  bool power = false;
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
    power = true;
  } // void begin( )


  /*
   * on( ): turn pin HIGH and set state to true, if there is power
   */
  void on( ) {
    if( power ) {
      digitalWrite( pin, HIGH );
    }
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
   * poweron( ): turn power back on, state is still last state
   */
  void poweron( ) {
    power = true;
  } // void poweron( )


  /*
   * poweroff( ): turn pin LOW, but keep state
   */
  void poweroff( ) {
    digitalWrite( pin, LOW );
    power = false;
  } // void poweroff( )

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

// end of file //
