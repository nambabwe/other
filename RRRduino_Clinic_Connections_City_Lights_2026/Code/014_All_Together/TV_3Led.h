#ifndef TV3LED
#define TV3LED

/*
 * TVLed
 * Simulates the flickering glow of a television on one RGB LED. Each instance
 * drives three PWM pins and cycles through NORMAL / DARK / HOLD scenes, with
 * big per-frame colour changes and small per-jitter brightness flicker.
 * 
 * Author: Speed Muller, 2026.06.10
 */

class TVLed {

public:

  /*
   * TVLed: Constructor - creates a TVLed on three PWM pins
   */
  TVLed( int rPin, int gPin, int bPin )
    : pinR( rPin ), pinG( gPin ), pinB( bPin ) {
  } // TVLed( int, int, int ) constructor


  /*
   * begin:
   */
  void begin( ) {
    pinMode( pinR, OUTPUT );
    pinMode( pinG, OUTPUT );
    pinMode( pinB, OUTPUT );

    startNewScene( );
  } // void begin( )


  /*
   * update:
   */
  void update( ) {
    unsigned long now = millis( );

    // Scene change
    if( now - sceneStartTime >= sceneDuration ) {
      startNewScene( );
    } // if

    // Frame update (big changes)
    if( now - lastFrameUpdate >= frameInterval ) {
      lastFrameUpdate = now;

      if( currentScene == NORMAL ) {
        generateColor( );
        baseBrightness = random( 40, 200 );
      } // if

      if( currentScene == NORMAL && random( 0, 10 ) > 7 ) {
        frameInterval = random( 10, 40 );
      } else if( currentScene == NORMAL ) {
        frameInterval = random( 30, 120 );
      } // if
    } // if

    // Jitter update (small flicker)
    if( now - lastJitterUpdate >= jitterInterval ) {
      lastJitterUpdate = now;

      int jitter;

      if( currentScene == DARK ) {
        jitter = random( -5, 5 );
      } else if( currentScene == HOLD ) {
        jitter = random( -10, 10 );
      } else {
        jitter = random( -20, 20 );
      } // if

      currentBrightness = constrain( baseBrightness + jitter, 5, 255 );
      jitterInterval = random( 5, 25 );
    } // if

    // Apply output
    applyOutput( );
  } // void update( )


private:
  int pinR, pinG, pinB;

  enum SceneType { NORMAL, DARK, HOLD };
  SceneType currentScene;

  unsigned long lastFrameUpdate =   0;
  unsigned long frameInterval   =  50;

  unsigned long lastJitterUpdate =  0;
  unsigned long jitterInterval   = 10;

  unsigned long sceneStartTime =    0;
  unsigned long sceneDuration  =    0;

  int r, g, b;
  int baseBrightness;
  int currentBrightness;


  /*
   * startNewScene:
   */
  void startNewScene( ) {
    int choice = random( 0, 15 );
    sceneStartTime = millis( );

    if( choice == 0 ) {
      currentScene = DARK;
      sceneDuration = random( 400, 800 );

      r = 0;
      g = 0;
      b = random( 2, 20 );
      baseBrightness = random( 5, 30 );

      frameInterval = random( 80, 160 );
      jitterInterval = random( 15, 40 );
    } else if( choice <= 3 ) {
      currentScene = HOLD;
      sceneDuration = random( 500, 3000 );

      generateColor( );
      baseBrightness = random( 40, 180 );

      frameInterval = random( 60, 120 );
      jitterInterval = random( 8, 25 );
    } else {
      currentScene = NORMAL;
      sceneDuration = random( 80, 250 );

      generateColor( );
      baseBrightness = random( 40, 200 );

      frameInterval = random( 30, 120 );
      jitterInterval = random( 5, 20 );
    } // if

    currentBrightness = baseBrightness;
  } // void startNewScene( )


  /*
   * generateColor:
   */
  void generateColor( ) {
    int mode = random( 0, 4 );

    switch( mode ) {
      case 0:
        r = random(   0,  40 );
        g = random(   0,  80 );
        b = random( 100, 150 );
        break;
      case 1:
        r = random( 150, 255 );
        g = random(  80, 180 );
        b = random(   0,  60 );
        break;
      case 2:
        r = random( 50, 100 );
        g = random( 50, 100 );
        b = random( 50, 100 );
        break;
      case 3:
        r = g = b = random( 180, 200 );
        break;
    } // switch
  } // void generateColor( )


  /*
   * applyOutput:
   */
  void applyOutput( ) {
    int rOut = ( r * currentBrightness ) / 255;
    int gOut = ( g * currentBrightness ) / 255;
    int bOut = ( b * currentBrightness ) / 255;

    analogWrite( pinR, rOut );
    analogWrite( pinG, gOut );
    analogWrite( pinB, bOut );
  } // void applyOutput( )

}; // class TVLed

#endif /* TV3LED */
