// TrafficLight.h V0.01
//
// Two ways to do this, each pole runs on its own cycle, you just need to start them at the right time 
// or a higher power change the states and each light just light the right light, right?
//
// I added a "delayStart" , which sends the 2nd light into a short red wait, before going green, when the 
// first lite goes red. So there is some time for both to be red, safety fizshhhht offisah!
//
// Something to check out http://playground.arduino.cc/Code/FiniteStateMachine 

#define UPDATETIME         50          // [ ] Change this to 100 after demo
#define INTER_RED_DELAY    30          // 30x50 = 1.5 seconds
#define GREEN_TIME        100          // 100x50 = 5.0 seconds
#define YELLOW_TIME        30          // 30x50 = 1.5 seconds
#define RED_TIME           20+GREEN_TIME+YELLOW_TIME+INTER_RED_DELAY

// The Class contains all the variables and code to operate a TrafficLite 
// In the newer Arduino IDEs, you can put all this code in a TrafficLite.h // file, so you can use it somewhere else too. 

class TrafficLite {
private:
  // Class Member Variables
  // These are initialized at startup
  uint8_t u8RedPin;         // the pin number of the red LED
  uint8_t u8YelPin;         // the pin number of the yellow LED
  uint8_t u8GrnPin;         // the pin number of the green LED
  uint8_t u8State;          // u8State used to set the LED,
                            // 0 off, 1 red, 2 yellow, 3 green, 4 delay start while red 
  
  bool bWhenOn = HIGH;      // LED is ON when in this state
                            // This allows you to source or sink current
                          
  uint8_t u16CountDown;     // counting down in time units of UPDATETIME
  
  unsigned long ulPrevious; // stores the last time the u16CountDown was updated

  // turn the led on for the current state, to avoid keeping track of which light to turn off,
  // we turn them all off, and then the needed one on.
  // Since the Anode is connected to 5V, we turn the LED on by making the LED pin LOW 
  void output( ) {
    digitalWrite( u8RedPin, !bWhenOn ); // turn all off
    digitalWrite( u8YelPin, !bWhenOn ); 
    digitalWrite( u8GrnPin, !bWhenOn ); 
    switch ( u8State ) {
      case 0:     // stay off
        break;    
      case 4:     // red, while the other light is red too
        // fall through
      case 1:     // red
        digitalWrite( u8RedPin, bWhenOn ); 
        break; 
      case 2:     // yellow
        digitalWrite( u8YelPin, bWhenOn ); 
        break; 
      case 3:     // green
        digitalWrite( u8GrnPin, bWhenOn ); 
        break;
      default: 
        break;
    }; // switch 
  } // void output( )


public:
  // Constructor - creates a Traficlite
  // and initializes the member variables, state and pins 
  TrafficLite( uint8_t u8TheRedPin, uint8_t u8TheYelPin, uint8_t u8TheGrnPin, bool bTheWhenOn = HIGH ) {
    u8RedPin = u8TheRedPin; 
    pinMode( u8RedPin, OUTPUT );
    u8YelPin = u8TheYelPin; 
    pinMode( u8YelPin, OUTPUT );
    u8GrnPin = u8TheGrnPin; 
    pinMode( u8GrnPin, OUTPUT );

    u16CountDown = 0; 
    u8State = 0;
    bWhenOn = bTheWhenOn;
    output( );
    ulPrevious = millis( );
  } // TrafficLite( int, int, int ) constructor


  // start off in the green state
  void begin( ) {
    u8State = 1;   // start red 
    u16CountDown = RED_TIME;
    output( );
  } // void begin( )


  // the only way to get into state 4, and we go red and wait the 
  // inter_red_delay, while the other lite is red.
  void delayStart( ) {
    u8State = 4;     // go red and wait 
    u16CountDown = INTER_RED_DELAY;
    output( );
  } // void delayStart( ) 


  // true if the lite is in the red state, use to turn the other lite on at the right time
  bool isRed( ) { 
    return u8State == 1; 
  } // bool isRed( )
  
  
  // to avoid using "delay", we rather check the current time and so how far we are from // last time, to decide if a lite needs to change color 
  void update( ) {
    unsigned long ulNow = millis( );
    if( ulNow - ulPrevious >= UPDATETIME ) {
      ulPrevious = ulNow;
      if ( u16CountDown > 0 ) {
        u16CountDown--;
      } else {
        u8State--; // 4 goes to 3 only once, so no need to switch case 4:
        switch ( u8State ) { 
          case 0: 
            u8State = 3;
            // fall through
          case 3: 
            u16CountDown = GREEN_TIME; 
            break; 
          case 2: 
            u16CountDown = YELLOW_TIME; 
            break; 
          case 1: 
            u16CountDown = RED_TIME; 
            break;
          case 4:
            // fall through
          default:
            break;
        }; // switch
        
        output( ); // Update the actual LEDs
      } // time for change yet?
    } // if
  } // void update( )
}; // TrafficLite Class

