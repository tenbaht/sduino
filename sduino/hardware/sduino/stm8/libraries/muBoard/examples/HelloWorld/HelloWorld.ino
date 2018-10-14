/*
  muBoard Library - Hello World

 Simple project to show muBoard usage
 
 Print to console and toggle LED

 This example code is in the public domain.

*/
 
// include the library code:
#include <muBoard.h>

// for scheduler
uint32_t  lastCall = 0L;


void setup() {
  
  // init communication to PC
  Serial_begin(115200);
  
} // setup()


void loop() {

  // toggle red LED every 500ms
  if ((millis() - lastCall) >= 500L) {
    lastCall += 500L;
    Serial_print_s("Hello World. The time is ");
    Serial_print_i(millis());
    Serial_println_s(" ms");
    digitalWrite(LED_RED, !digitalRead(LED_RED));
  }

} // loop()
