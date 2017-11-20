/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
 modified to C syntax on 30 Jan 2017 for use with the sdunio environment
 by Michael Mayer
*/

#include <Servo.h>

Servo(myservo);         // create servo "object" to control a servo
                        // twelve servo objects can be created on most boards

int pos = 0;            // variable to store the servo position

void setup() {
  myservo_attach(9);    // attaches the servo on pin 9 to the servo object
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
                                // in steps of 1 degree
    myservo_write(pos);         // tell servo to go to position in variable 'pos'
    delay(15);                  // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo_write(pos);         // tell servo to go to position in variable 'pos'
    delay(15);                  // waits 15ms for the servo to reach the position
  }
}

