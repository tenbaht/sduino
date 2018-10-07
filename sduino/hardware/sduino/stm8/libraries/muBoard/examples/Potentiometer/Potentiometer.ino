/*
  muBoard Library - Hello World

 Simple project to show muBoard usage
 
 Read potentiometer and print via console.
 Can be displayed e.g. via via Serial Plotter.

 This example code is in the public domain.

*/

// include the library code:
#include <muBoard.h>


void setup() {
  
  // init communication to PC
  Serial_begin(115200);
  
} // setup()


void loop() {

  // print poti reading to console
  Serial_println_i(analogRead(POTI));
  delay(50);

} // loop()
