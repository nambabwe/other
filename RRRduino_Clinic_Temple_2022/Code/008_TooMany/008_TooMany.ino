#define VERSION_STR    "008_Temple Too Many Blinking things, 2022.05.07"

/*
 * Making many LEDs work! Need a computer to do the math!
 * All sequences need to be the same length to repeat.
 *
 * And how do you add random???
 */
 
// \_________________/----  ,  \_________________/----  ,  \______
// --\_________________/--  ,  --\_________________/--  ,  --\____
// ----\_________________/  ,  ----\_________________/  ,  ----\__
// _________/----\________  ,  _________/----\________  ,  _______
// \_/-\_/-\_/-\_/-\_/-\/-  ,  \_/-\_/-\_/-\_/-\_/-\/-  ,  \_/-\_/
// x x x x xxx x x x x xxx  ,  x x x x xxx x x x x xxx  ,  x x x x
 
 
  
#define LEDPIN1         12
#define LEDPIN2         11
#define LEDPIN3         A3
#define LEDPIN4         13
#define LEDPIN5         10

#define TIME01          25
#define TIME02          25
#define TIME03          25
#define TIME04          25
#define TIME05          12
#define TIME06          12
#define TIME07          25
#define TIME08          25
#define TIME09          25
#define TIME10          25
#define TIME11          25
#define TIME12          12
#define TIME13          12
#define TIME14          12


// Code in setup( ) will run once on power up or after a reset
// setup( ) is called by main.cpp
void setup( ) {
  // ... 10 lines to init
} // setup( )


// Code in loop( ) will execute over and over, forever after setup( ) was called
// The "for (;;) { }" repeat forever loop in main.cpp calls loop over and over
void loop( ) {
  // ... 14 times 2 lines to control
} // loop( )

