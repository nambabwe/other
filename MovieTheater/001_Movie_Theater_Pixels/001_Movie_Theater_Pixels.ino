#define VERSION_STR "001_Movie_Theater_Pixels, 2023.10.25"

/*
 * Sequences at the theater front door
 * 
 * Author: Speed Muller, Date: 2023.10.25, 12:00
 */


#include <Adafruit_NeoPixel.h>

#define STRIP_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT    6

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip( LED_COUNT, STRIP_PIN, NEO_GRB + NEO_KHZ800 );


#include "Heartbeat.h"
#include "Light.h"

#define BAUD      115200

// heartbeat
#define LED0          13

// Light sensor pin
#define ANAPIN        A0
// 10-bit A/D, 0-1023

#define DELAY        125

#define TIMEOUT       10

Heartbeat heart( LED0, 50, 617 );

unsigned long now, before;
uint16_t analogValue;

uint8_t lights[ LED_COUNT ][ 3 ] = { 0 };

void setup( ) {
  Serial.begin( BAUD );

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for( uint8_t i = 0; i < LED_COUNT; i++ ) {
    
    strip.setPixelColor( i, 20, 20, 20 );
  } // for i

  pinMode( ANAPIN, INPUT );

  heart.begin( );  

  Serial.println( );
  Serial.println( VERSION_STR );

  now = before = millis( );
} // setup( )

uint8_t red = 0;

void loop( ) {
  uint32_t magenta = strip.Color( 255, 0, 255 );


  now = millis( );
  if( now - before > TIMEOUT ) {
    before = now;

    strip.setPixelColor( random( 0, LED_COUNT ), red++, 0, 0 );
    strip.show( );
    analogValue = analogRead( ANAPIN );
    Serial.print( "Light:" ); Serial.println( analogValue );
  } // if time to read sensor

  heart.update( );
} // loop( )
