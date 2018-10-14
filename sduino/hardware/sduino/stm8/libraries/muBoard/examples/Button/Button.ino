/*
  muBoard Library - Hello World

 Simple project to show muBoard usage
 
 Mirror automode switch and user button to red & green LEDs

 This example code is in the public domain.

*/

// include the library code:
#include <muBoard.h>


void setup() {
  
  // init communication to PC
  Serial_begin(115200);
  
  // configure button pin (=IO_1)
  pinMode(BUTTON, INPUT_PULLUP);

} // setup()


void loop() {

  // mirror auto-mode switch (ON=low) to red LED (low=ON)
  digitalWrite(LED_RED, digitalRead(AUTO_MODE));

  // mirror user button (ON=low) to green LED (low=ON)
  digitalWrite(LED_GREEN, digitalRead(BUTTON));

} // loop()
