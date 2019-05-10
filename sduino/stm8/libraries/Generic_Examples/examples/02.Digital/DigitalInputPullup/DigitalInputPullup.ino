/*
  Input Pull-up Serial

  This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a digital
  input on pin 2 and prints the results to the Serial Monitor.

  The circuit:
  - momentary switch attached from pin 2 to ground
  - built-in LED on pin 3

  Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
  20K-ohm resistor is pulled to 3.3V. This configuration causes the input to read
  HIGH when the switch is open, and LOW when it is closed.

  created 14 Mar 2012
  by Scott Fitzgerald
  modified 13 Feb 2017 for use with sduino
  by Michael Mayer

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/InputPullupSerial
*/

#include <Serial.h>

void setup() {
  //start serial connection
  Serial_begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);

}

void loop() {
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(2);
  //print out the value of the pushbutton
  Serial_println_u(sensorVal);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 3 when the
  // button's pressed, and off when it's not:
  if (sensorVal == HIGH) {
    digitalWrite(3, LOW);
  } else {
    digitalWrite(3, HIGH);
  }
}
