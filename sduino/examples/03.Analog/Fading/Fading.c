/*
 Fading

 This example shows how to fade an LED using the analogWrite() function.

 If the used board doesn't include a buildin LED:
 * LED attached from a digital pin to ground.
 * assign the used pin number to the variable ledPin

 Created 1 Nov 2008
 By David A. Mellis
 modified 30 Aug 2011
 By Tom Igoe
 modified 28 Feb 2017 for use with sduino
 by Michael Mayer

 http://www.arduino.cc/en/Tutorial/Fading

 This example code is in the public domain.

 */

#include <Arduino.h>

int ledPin = LED_BUILTIN;    // define the LED pin

void setup() {
  // nothing happens in setup
}

void loop() {
  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}


