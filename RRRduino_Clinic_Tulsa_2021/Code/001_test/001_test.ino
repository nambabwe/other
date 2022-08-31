// Tulsa 2021.10.08 Arduino for Beginners Make and Take Clinic

void setup( ) {
  pinMode( 13, OUTPUT );
} // setup( )

void loop( ) {
  digitalWrite( 13, HIGH );
  delay( 750 );
  
  digitalWrite( 13, LOW );
  delay( 250 );
} // loop( )
