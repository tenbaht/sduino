/*
  LED bar graph

  Turns on a series of LEDs based on the value of an analog sensor.
  This is a simple way to make a bar graph display. Though this graph uses 10
  LEDs, you can use any number by changing the LED count and the pins in the
  array.

  This method can be used to control any series of digital outputs that depends
  on an analog input.

  The circuit:
  - 10k Potentiometer, center pin attached to analog input 0
  - one side pin (either one) to ground
  - the other side pin to +3.3V
  - LEDs on pins 0-2, 5 and 7-12 to ground. DO NOT USE the i2c-pins 3 and 4!

  The i2c pins lack the capability to drive the output high on their own.
  A LED connetected to ground will not work on these pins. You could connect
  A LED to Vcc, though. Than it will be lit when a low signal is written to
  the pin.

  created 4 Sep 2010
  by Tom Igoe
  modified 1 Mar 2017 for use with sduino
  by Michael Mayer

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/BarGraph
*/

// these constants won't change:
const int analogPin = A0;   // the pin that the potentiometer is attached to
const int ledCount = 10;    // the number of LEDs in the bar graph

int ledPins[] = {
  0, 1, 2, 5, 7, 8, 9, 10, 11, 12
};   // an array of pin numbers to which LEDs are attached


void setup() {
  // loop over the pin array and set them all to output:
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT);
  }
}

void loop() {
  // read the potentiometer:
  int sensorReading = analogRead(analogPin);
  // map the result to a range from 0 to the number of LEDs:
  int ledLevel = map(sensorReading, 0, 1023, 0, ledCount);

  // loop over the LED array:
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    // if the array element's index is less than ledLevel,
    // turn the pin for this element on:
    if (thisLed < ledLevel) {
      digitalWrite(ledPins[thisLed], HIGH);
    }
    // turn off all pins higher than the ledLevel:
    else {
      digitalWrite(ledPins[thisLed], LOW);
    }
  }
}
