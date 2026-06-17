#define VERSION_STR    "003_Two_Beacon_Blink, 2026.06.20"

/*
 * Making two beacons work! Need external LEDs and resistors on pins 5 and 6
 *     
 * Author: Gert 'Speed' Muller, 2022.08.09
 */
 
// \_________________/----  ,  \_________________/----  ,  \____
// --\_________________/--  ,  --\_________________/--  ,  --\__
// x x               x x    ,  x x               x x    ,  x x  
 
#define LEDPIN1          5
#define LEDPIN2          6

#define TIME1          250
#define TIME2         1850
#define TIME3          250
#define TIME4           50

// Comments
void setup( ) {
  Serial.begin( 115200 );  
  Serial.println( );
  Serial.println( VERSION_STR );

  pinMode( LEDPIN1, OUTPUT );
  pinMode( LEDPIN2, OUTPUT );
  digitalWrite( LEDPIN1, HIGH );
} // setup( )

// Comments
void loop( ) {
  digitalWrite( LEDPIN1, LOW );
  Serial.println( "Off 1" );
  delay( TIME1 );

  digitalWrite( LEDPIN2, LOW );
  Serial.println( "Off 2" );
  delay( TIME2 );

  digitalWrite( LEDPIN1, HIGH );
  Serial.println( "On 1" );
  delay( TIME3 );

  digitalWrite( LEDPIN2, HIGH );
  Serial.println( "On 2" );
  delay( TIME4 );
} // loop( )

