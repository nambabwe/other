#include "SlowDoors.h"
#include "BreweryLites.h"

#define NUM_DOORS 3

enum DoorState {
  POST = 0,
  DoorOpening,
  DoorOpen,
  DoorClosing,
  DoorClosed
};

DoorState breweryDoorStates[ NUM_DOORS ];

// SlowDoor( uint8_t servoPin, unint8_t buttonPin, uint16_t start, uint16_t stop, uint16_t updateRate )
SlowDoor *breweryDoors[ NUM_DOORS ] = {
  new SlowDoor( 3, A0, 70, 120, 50 ),
  new SlowDoor( 5, A1, 75, 125, 50 ),
  new SlowDoor( 6, A2, 60, 180, 30 )  // servo moves a little faster, since further travel
};

//BreweryLite( uint8_t redPin, uint8_t greenPin, uint16_t flashRate )
BreweryLite *doorLites[ NUM_DOORS ] = {
  new BreweryLite(  7,  8, 500 ),
  new BreweryLite(  9, 10, 400 ),  // flash a little faster
  new BreweryLite( 11, 12, 500 )
};

void setup( ) {
  Serial.begin( 115200 );
  Serial.println( F("\nRolling Doors version 1.0") );
  
  for( uint8_t i; i=0; i<NUM_DOORS ) {
    breweryDoorStates[i] = POST;
    breweryDoors[i]->Setup( );  // attach servo pin and INPUT_PULLUP button pin
    doorLites[i]->Setup( );     // pinMode both LED pins as OUTPUTs
  } // for  
} // setup( ) 

void loop() {  
  // For each door, use switch to manage the state machine
  for( uint8_t i; i=0; i<NUM_DOORS ) {
    switch( breweryDoorStates[ i ] ) {
      case( POST ):
        breweryDoors[i]->Open( );               // command the door to ...
        doorLites[i]->FlashingRed( );           // command the light to ...
        breweryDoorStates[i] = DoorOpening;     // new state is ...   
        break;
      case( DoorOpening ):
        if( breweryDoors[i]->isOpen( ) ) {      // check if the door is ...
          doorLites[i]->Green( );               // set the light to ...
          breweryDoorStates[i] = DoorOpen;      // new state is ...
        } // if
        break;
      case( DoorOpen ):
        if( breweryDoors[i]->wasButtonPushed( ) ) {
          breweryDoors[i]->Close( );
          doorLites[i]->FlashingRed( );
          breweryDoorStates[i] = DoorClosing;   // new state is ... 
        } // if
        break;
      case( DoorClosing ):
        if( breweryDoors[i]->isClose( ) ) {
          doorLites[i]->Red( );
          breweryDoorStates[i] = DoorClosed;    // new state is ... 
        } // if
        break;
      case( DoorClosed ):
        if( breweryDoors[i]->wasButtonPushed( ) ) {
          // count number of closed doors
          uint8_t closedCount = 0;
          for( uint8_t k; k=0; k<NUM_DOORS ) {   
            if( breweryDoors[k]->isClose( ) ) closedCount++;
          } // for
          // and only open if all are closed
          if( closedCount == NUM_DOORS ) {  
            breweryDoors[i]->Open( );
            doorLites[i]->FlashingRed( );
            breweryDoorStates[i] = DoorOpening; // new state is ... 
          } // if all closed
        } // if
        break;

      default: // should never get here
        Serial.println( F("oops...") );
        break;      
    } // switch

    doorLites[i]->Update( );        // update the lights, green, red or flashing
    breweryDoors[ i ]->Update( );   // update the servo to move a step closer 
                                    // or stay at its commanded position
  } // for
} // loop( )
