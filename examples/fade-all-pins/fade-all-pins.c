/*
 * fade-all-pins
 *
 * This example shows how to fade an LED on all PWM-capable pins
 * using the analogWrite() function. (Derived from 01.Basics/Fade)
 *
 * This example code is in the public domain.
 */

#include <Arduino.h>

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
char i;

// the setup routine runs once when you press reset:
void setup()  {
  // declare all pin to be an output:
  for (i=0; i<NUM_DIGITAL_PINS; i++) pinMode(i,OUTPUT);
} 

// the loop routine runs over and over again forever:
void loop()  {
  // set the brightness of all pins:
  for (i=0; i<NUM_DIGITAL_PINS; i++) analogWrite(i,brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
  }     
  // wait for 30 milliseconds to see the dimming effect    
  delay(30);                            
}

