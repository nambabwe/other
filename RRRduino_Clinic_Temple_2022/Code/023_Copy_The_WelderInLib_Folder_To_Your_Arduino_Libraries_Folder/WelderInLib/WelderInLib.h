// WelderInLib.h

#define SPARKSRANDOM        100
#define SMALLRANDOM         100
#define WELDEROFFRANDOM     150
#define LONGRANDOM          160

class WelderInLib {
private:
  uint8_t  u8State;         // 'o' short on, 'f' short off, 'F' long off
  uint16_t u16RandomOn;
  uint16_t u16RandomOnTime;
  uint16_t u16RandomOffTime;
  uint16_t u16RandomLongOffTime;

  uint16_t u16SparksRANDOM;
  uint16_t u16SmallRANDOM;
  uint16_t u16OffRANDOM;
  uint16_t u16LongRANDOM;

  bool     bOnWhen;
  uint8_t  u8Pin;
  unsigned long ulNow;
  unsigned long ulPrevious;

protected:
  bool bEnabled = false;

public:
  WelderInLib( uint8_t thePin, uint16_t theOFFRANDOM, bool theTurnOnWhenSetting, uint16_t theSPARKSRANDOM = SPARKSRANDOM,
          uint16_t theSMALLRANDOM = SMALLRANDOM, uint16_t theLONGRANDOM = LONGRANDOM ) {
    u16SparksRANDOM  = theSPARKSRANDOM;
    u16SmallRANDOM   = theSMALLRANDOM;
    u16OffRANDOM     = theOFFRANDOM;
    u16LongRANDOM    = theLONGRANDOM;

    setPinAndMode( thePin, theTurnOnWhenSetting );
    
    randomSeed( analogRead( A2 ) );
  } // WeWelderInLiblder( uint8_t, uint16_t, bool, [[[uint16_t], uint16_t], uint16_t] )

  void setPinAndMode( uint8_t thePin, bool bTheTurnOnWhenSetting ) {
    u8Pin = thePin;
    bOnWhen = bTheTurnOnWhenSetting;
  } // setPinMode( uint8_t, bool )


  void begin( ) {
    enable( );
    u8State          = 'f';

    u16RandomOn      = random( 0, u16SparksRANDOM );
    u16RandomOnTime  = random( 0, u16SmallRANDOM );
    u16RandomOffTime = random( 0, u16SmallRANDOM );

    pinMode( u8Pin, OUTPUT );
    //..Serial.print( pin, DEC ); Serial.print( F(":") );Serial.println( bOnWhen, DEC );
    delay( 10 );
  } // begin( )


  void on( ) {
    digitalWrite( u8Pin, bOnWhen );
  } // on( )


  void onPrint( ) {
    Serial.print( F("p[") ); Serial.print( u8Pin, DEC ); Serial.print( F("]:") );
    Serial.println( F("On") );
    on( );
  } // onPrint( )


  void off( ) {
    digitalWrite( u8Pin, !bOnWhen );
  } // off( )


  void offPrint( ) {
    Serial.print( F("p[") ); Serial.print( u8Pin, DEC ); Serial.print( F("]:") );
    Serial.println( F("Off") );
    off( );
  } // offPrint( )


  void enable( ) {
    bEnabled = true;
    ulNow = ulPrevious = millis( );
  } // enable( )


  void disable( ) {
    digitalWrite( u8Pin, !bOnWhen );
    bEnabled = false;
  } // disable( )


  void setEnabled( bool bNewState ) {
    if( bNewState ) {
      enable( );
    } else
      disable( );
  } // setEnabled( bool )


  void update( ) {
    ulNow = millis( );
    if( bEnabled ) {
      switch( u8State ) {
        case 'f': // we turn on here if onTime expired
           if( ulNow - ulPrevious >= u16RandomOnTime ) {
             ulPrevious = ulNow;
             u16RandomOffTime = random( 0, u16SmallRANDOM );
             on( );
             u8State = 'o';
           } // if
          break;
        case 'o': // we turn off here if offTime expired,
                  // and check if it is time to rest
          if( ulNow - ulPrevious >= u16RandomOffTime ) {
            ulPrevious = ulNow;
            u16RandomOnTime = random( 0, u16SmallRANDOM );
            off( );
            u16RandomOn--;
            if( u16RandomOn == 0 ) {
              u16RandomLongOffTime = random( 0, u16OffRANDOM ) * random( 0, u16LongRANDOM );
              u8State = 'F';
            } else { u8State = 'f'; }
          } // if
          break;
        case 'F':
           if( ulNow - ulPrevious >= u16RandomLongOffTime ) {
             ulPrevious = ulNow;
             u16RandomOnTime = random( 0, u16SmallRANDOM );
             u16RandomOn = random( 0, u16SparksRANDOM );
             off( );
             u8State = 'f';
           } // if
          break;
        default:
          break;
      } // switch
    } else {
      off( );
    } // if enabled
  } // update( )
}; // class WelderInLib


