/*
 * A relay to be controlled by a push button from the other side of an ESP-NOW connection.
 * For a 3.3 Vdc ESP system, please use a 3.3 Vdc relay to control your 110 Vac gadget.
 * 
 * The MAC address of this system is needed by the controlling side, so it is printed on 
 * startup.
 * 
 * Author: Speed Muller
 * 
 * reference: https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/
 */

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <espnow.h>

#define VERSION_STR "RRRduino ESP Relay v0.01, 2022.09.10"
#define BAUDRATE           115200
#define HEARTBEAT            1000

// D3 on Lolin/NodeMCU
#define RELAY_PIN               0

// D4 on Lolin/NodeMCU
#define LED_PIN                 2
#define TOGGLECOMMAND          't'

// Structure to receive data
typedef struct struct_message {
  char cLetter[4];
  bool bState;
} struct_message;

struct_message stctMyData;

uint32_t u32Now, u32Before = 0;


// OnEspNowDataReceive( ): Called with message received
void OnEspNowDataReceive( uint8_t *u8MacAddr, uint8_t *u8IncomingData, uint8_t u8Len ) {
  memcpy( &stctMyData, u8IncomingData, sizeof( stctMyData ) );
  Serial.print( F( "Bytes received = " ) );
  Serial.print( u8Len );
  Serial.print( F( ": [ " ) );
  Serial.print( stctMyData.cLetter );
  Serial.print( F( ", " ) );
  Serial.print( stctMyData.bState ? "true" : "false" );
  Serial.println( F( " ]" ) );
  
  if( stctMyData.cLetter[0] == TOGGLECOMMAND ) {
    setRelay( stctMyData.bState ); // Update relay to new command 
  } // if password good
} // void OnEspNowDataReceive( uint8_t *, uint8_t *, uint8_t )


// setRelay( bool ): Set the RELAY_PIN to the bool state
void setRelay( bool bRelayState ) {
  digitalWrite( RELAY_PIN, bRelayState );
} // void setRelay( bool )


// setup( ): Print version+mac, init ESP-NOW, register receive callback, 
// setup pins up and capture time
void setup( ) {
  Serial.begin( BAUDRATE );
  
  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW );          // LED off

  pinMode( RELAY_PIN, OUTPUT );
  digitalWrite( RELAY_PIN, LOW );        // Relay off
  delay( 100 );
  
  Serial.println( );
  Serial.println( F( VERSION_STR ) );    // Print the version

  WiFi.mode( WIFI_STA );
  Serial.println( WiFi.macAddress( ) );  // Print the MAC address like "8C:CE:4E:E3:EB:39"
                                         // And this is needed by the Sender in the form of
                                         // { 0x8C, 0xCE, 0x4E, 0xE3, 0xEB, 0x39 };

  if( esp_now_init( ) == 0 ) { // Initialize ESP-NOW
    Serial.println( F( "ESP NOW initialized..." ) );
  } else {
    Serial.println( F( "ESP NOW initialize failed..." ) );
    ESP.restart( );
    delay( 10 );               // not a delay in real use!
  } // if esp_now init

  esp_now_register_recv_cb( OnEspNowDataReceive ); // Register callback for receiving

  u32Now = u32Before = millis( );
} // setup( )


// loop( ): Only control the heartbeat on the LED
void loop( ) {
  u32Now = millis( );
  
  if( u32Now - u32Before > HEARTBEAT ) {
    digitalWrite( LED_PIN, !digitalRead( LED_PIN ) );
    u32Before = u32Now;
  } // if time for heartbeat
} // loop( )

// Sketch uses 267240 bytes (35%) of program storage space. Maximum is 761840 bytes.
// Global variables use 27252 bytes (33%) of dynamic memory,
// leaving 54668 bytes for local variables. Maximum is 81920 bytes.
