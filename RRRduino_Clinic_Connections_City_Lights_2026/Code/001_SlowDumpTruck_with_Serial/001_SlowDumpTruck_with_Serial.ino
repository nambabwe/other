#define VERSION_STR    "002_SlowDumpTruckFaster, 2026.06.20"

/*
 * Blink by setting the ONTIME and the OFFTIME separately
 * \_________________/----  ,  \_________________/----  ,  \__
 * x                 x      ,  x                 x      ,  x  
 *     
 * Author: Gert 'Speed' Muller, 2026.06.20
 */

#define LEDPIN    LED_BUILTIN    // this is 13 on an UNO
#define ONTIME            100
#define OFFTIME          1900 

// Comments
void setup( ) {
  Serial.begin( 115200 );  
  Serial.println( );
  Serial.println( VERSION_STR );

  pinMode( LEDPIN, OUTPUT );
  digitalWrite( LEDPIN, HIGH );
} // setup( )

// Comments
void loop( ) {
  digitalWrite( LEDPIN, LOW );
  Serial.println( "Off" );
  delay( OFFTIME );
  
  digitalWrite( LEDPIN, HIGH );
  Serial.println( "On" );
  delay( ONTIME );
}
 // loop( )
