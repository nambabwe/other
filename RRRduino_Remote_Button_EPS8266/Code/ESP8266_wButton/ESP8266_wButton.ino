/*
 * A button to press to change the state of a relay on the other side
 * of the ESP-NOW connection. An LED will show the current commanded 
 * state of the relay.
 * 
 * The MAC address of the ESP with the relay is needed in the 
 * broadcastAddress to send the data to.
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

#define VERSION_STR "RRRduino ESP Button v0.01, 2022.09.10"
#define BAUDRATE           115200
#define HEARTBEAT            1000
#define UPDATEATLEAST       10000

#define TOGGLECOMMAND          't'

#define BUTTON_PIN              0
#define LED_PIN                 2

//receiver: "8C:CE:4E:E3:EB:39"
uint8_t u8ArrBroadcastAddress[] = { 0x8C, 0xCE, 0x4E, 0xE3, 0xEB, 0x39 };

// Structure to send data
typedef struct struct_message {
  char cLetter[4];
  bool bState;
} struct_message;

struct_message stctMyData;

uint32_t u32Now, u32LastUpdate = 0;

bool bState = false;
bool bChanged = true;

// OnEspNowDataReceive( ): Called when message delivered
void OnEspNowDataSent( uint8_t *mac_addr, uint8_t u8Status ) {
  Serial.print( F( "Last packet send status: Delivery " ) );
  Serial.println( u8Status == 0 ? "Success" : "Failed" );
} // OnDataSent( const uint8_t *, esp_now_send_status_t )


// setup( ): Print version+mac, init ESP-NOW, register send callback, 
//           setup pins up and capture time
void setup( ) {
  Serial.begin( BAUDRATE );
  
  pinMode( BUTTON_PIN, INPUT_PULLUP );

  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW );  
  delay( 100 );

  Serial.println( );
  Serial.println( F( VERSION_STR ) );

  WiFi.mode( WIFI_STA );
  Serial.println( WiFi.macAddress( ) );

  if( esp_now_init( ) == 0 ) { // Initialize ESP-NOW
    Serial.println( F( "ESP NOW initialized..." ) );
  } else {
    Serial.println( F( "ESP NOW initialize failed..." ) );
    ESP.restart( );
    delay( 10 );
  } // if esp_now init

  esp_now_set_self_role( ESP_NOW_ROLE_CONTROLLER );
  esp_now_register_send_cb( OnEspNowDataSent );  // Register callback for sending

  // Add peer
  esp_now_add_peer( u8ArrBroadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0 );

  bState = false;     // Default state
  bChanged = true;    // Force update at power up
  u32Now = u32LastUpdate = millis( );
} // setup( )


// loop( ): Check for a Serial port command, check if the button is pressed,
// Keep track of the relay state and send the command when state changed
//   and also at a regular interval (UPDATEATLEAST)
void loop( ) {
  String strIncoming;
  String strBefore;

  u32Now = millis( );
  
  if( Serial.available( ) > 0 ) {
    strBefore = Serial.readStringUntil( ':' );
    Serial.print( strBefore );
    Serial.print( ":" );
    strIncoming = Serial.readStringUntil( ';' );
    Serial.print( strIncoming );
    Serial.println( ";" );
    if( strIncoming.length( ) > 0 ) {
      if( strIncoming.charAt( 0 ) == TOGGLECOMMAND ) {
        bState = !bState;
        bChanged = true;
      } // if "press" command :p;
    } // if something
  } // if data available

  if( digitalRead( BUTTON_PIN ) == LOW ) {
    delay( 100 ); // Bebounce
    Serial.println( "." );
    
    while( digitalRead( BUTTON_PIN ) == LOW ) {
      delay( 10 ); // Bebounce
    } // while LOW
    bState = !bState;
    bChanged = true;        
  } // if button pressed

  
  if( bChanged || ( u32Now - u32LastUpdate > UPDATEATLEAST ) ) {
    u32LastUpdate = u32Now;
    bChanged = false;
    stctMyData.cLetter[0] = TOGGLECOMMAND;
    stctMyData.cLetter[1] = 0x00;  // terminate string
    //strcpy( myData.a, "t" );     // 't'oggle command, use for strings
    stctMyData.bState = bState;
    
    esp_now_send( u8ArrBroadcastAddress, (uint8_t *)&stctMyData, sizeof( stctMyData ) );
    Serial.print( F( "Sent: [" ) );
    Serial.print( stctMyData.cLetter );
    Serial.print( F( ", " ) );
    Serial.print( stctMyData.bState ? "true" : "false" );
    Serial.println( F( "]" ) );    
  } // if a change, then send  
} // loop( )

// Sketch uses 270136 bytes (35%) of program storage space. Maximum is 761840 bytes.
// Global variables use 27324 bytes (33%) of dynamic memory, 
//   leaving 54576 bytes for local variables. Maximum is 81920 bytes.
