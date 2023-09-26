#define VERSION_STR "002_Movie_Theater_FastLED, 2023.10.25"

/*
 * Sequences at the theater front door
 * 
 * Author: Speed Muller, Date: 2023.10.25, 12:00
 */


#include <FastLED.h>

#define STRIP_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT    6

// Declare our LEDs:
CRGB strip[LED_COUNT];


#include "Heartbeat.h"
#include "Light.h"

#define BAUD      115200

// heartbeat
#define LED0          13

// Light sensor pin
#define ANAPIN        A0
// 10-bit A/D, 0-1023

#define DELAY        125

#define TIMEOUT      300

Heartbeat heart( LED0, 50, 617 );

unsigned long now, before;
uint16_t analogValue;

#define CHIPSET     NEOPIXEL

enum State { slideleftyr, sliderightyr, slideleftbg, slidertbg };
enum State state = slideleftyr;

void setup( ) {
  Serial.begin( BAUD );

  FastLED.addLeds< CHIPSET, STRIP_PIN >( strip, LED_COUNT );

  for( uint8_t i = 0; i < LED_COUNT; i++ ) {
    
    //strip.setPixelColor( i, 20, 20, 20 );
  } // for i

  
  pinMode( ANAPIN, INPUT );

  heart.begin( );  

  Serial.println( );
  Serial.println( VERSION_STR );

  strip[LED_COUNT-1] = CRGB::White;
  strip[LED_COUNT-2] = CRGB::White;
  strip[LED_COUNT-3] = CRGB::White;
  strip[LED_COUNT-4] = CRGB::Grey;
  strip[LED_COUNT-5] = CRGB::Black;
  strip[LED_COUNT-6] = CRGB::Grey;

  FastLED.setBrightness( 10 );
  FastLED.show( );
  now = before = millis( );
} // setup( )

bool toggle = true;

void slide( uint8_t u8Size, bool bLeftToRight = true, uint8_t u8Step = 1 ) {
  CRGB temp;

  for( uint8_t j=0; j<u8Step; j++ ) {
    if( u8Size <= LED_COUNT ) {
      if( u8Size > 0 ) {
        if( bLeftToRight ) {
          temp = strip[0];
          for( uint8_t i=0; i<u8Size; i++ ) {
            if( i+1 < u8Size ) {
              strip[i] = strip[i+1];
            } else {
              strip[i] = temp;
            }
          }
        } else {
          temp = strip[u8Size-1];
          for( uint8_t i=u8Size-1; i>0; i-- ) {
            //Serial.print( "Light:" ); Serial.println( i ); 
            strip[i] = strip[i-1];
          }
          strip[0] = temp;
        }
      } // if big enough to slide    
    }
  } // for number of steps
}

uint8_t counter = 255;

void loop( ) {
  now = millis( );
  if( now - before > TIMEOUT - counter ) {
    before = now;

    slide( LED_COUNT, 1, 1 );

    if( toggle ) {
      //strip[1] = CRGB::Red;
      counter--;
      if( counter == 100 ) {
        toggle = false;
      }
      
    } else {
      //strip[5] = CRGB::Green;
      counter++;
      if( counter == 254 ) {
        toggle = true;
        counter = 254;
      }
    }
    FastLED.show( );
    //analogValue = analogRead( ANAPIN );
    //Serial.print( "Light:" ); Serial.println( counter ); //Serial.println( analogValue );
  } // if time to read sensor

  heart.update( );
} // loop( )
