# Servo Library

This library can control a great number of servos. It makes careful use
of timers: the library can control 12 servos using only 1 timer.
Derived from the Arduino Servo library v1.8.0.


This library has a slightly diffent user interface than the usual singleton
libraries. This allows for handling more than one servo per Sketch but it
requires some more attention when porting an existing sketch from C++ to C.

Each Servo instance is identified by a channel-ID. In order to match the C++
class constructor syntax it is defined as a type `Servo`.

This channel-ID is used as a file-descriptor-like value and need to be
passed to all API functions except for `Servo_attach()`. This function
claimes the next free channel number.

## Example

Read a potentiometer on analog input 0 and set a servo pulse length between
1000us and 2023us:
```c
#include <Arduino.h>
#include <Servo.h>

Servo myservo;          // just a simple unsigned char to hold the channel-ID

int val;                // variable to read the value from the analog pin

void setup() {
  myservo = Servo_attach(9);    // attaches the servo on pin 9, returns channel-ID
}

void loop() {
  val = analogRead(0);          // reads the value of the potentiometer
  Servo_write(myservo, val+1000);// sets the servo position
  delay(15);
}
```


Original Arduino C++-Sytax:
```c
#include <Servo.h>

Servo myservo;          // create servo object to control a servo

int val;                // variable to read the value from the analog pin

void setup() {
  myservo.attach(9);    // attaches the servo on pin 9 to the servo object
}

void loop() {
  val = analogRead(0);          // reads the value of the potentiometer
  myservo.write(val+1000);      // sets the servo position
  delay(15);
}

```



## API


data type `Servo`: A type definition for a simple unsigned char to hold the
channel number returned my `Servo_attach()`. Needed for every servo. Syntax
identical to the Arduino class constructor.

Arduino syntax			|sduino syntax
--------------------		|---------------------
`Servo myservo;`		|`Servo myservo;`
`myservo.attach(pin);`		|`myservo = Servo_attach(pin);`
`myservo.attach(pin,min,max);`	|`Servo_attach_minmax(pin,min,max);`
`myservo.detach();`		|`Servo_detach(byte channel);`
`myservo.write(val);`		|`Servo_write(myservo, val);`
`myservo.writeMicroseconds(val);`|`Servo_writeMicroseconds(myservo, val);`
`val = myservo.read();`		|`val = Servo_read(myservo);`
`val = myservo.readMicroseconds();`|`val = Servo_readMicroseconds(myservo);`
`myservo.attached()`		|`Servo_attached(myservo);`



`uint8_t Servo_attach(int pin);`
attach the given pin to the next free channel, sets pinMode, returns channel
number or 0 if failure.

`uint8_t Servo_attach_minmax(int pin, int min, int max);`
as above but also sets min and max values for writes. 

`void Servo_detach(byte channel);`

`void Servo_write(byte channel, int value);` if value is < 200 it is treated
as an angle and scaled according the minimum and maximum pulsewidth defined
using the attach() function earlier, otherwise as pulse width in
microseconds, unscaled.

`void Servo_writeMicroseconds(byte channel, int value);`
Write pulse width in microseconds, unscaled.

`int Servo_read(byte channel);`
returns current pulse width as an angle between 0 and 180 degrees.

`int Servo_readMicroseconds(byte channel);`
returns current pulse width in microseconds for this servo (was read_us() in
first Arduino release).

`bool Servo_attached(byte channel);`
return true if this servo is attached, otherwise false .


## Relationship between PWM/analog output and Servo output

It is not possible to use a timer for PWM and the Servo Library at the same
time. Since this library currently uses timer1, the PWM function
(`analogWrite()`) is disabled for the pins connected to timer1 (for the
STM8S103 this is pin PC3 and PC4 or digital pin 5 and 6).

Pins connected to timer2 (PA3, PD3, PD4, digital pin 2,12,13) are still
usable for PWM output.




## Possible improvements


### A more sophisticated pseudo-OO API

Define a set of preprocessor macros that more OO-like definitions like this
become possible:

Current syntax			|OO-like syntax		|Arduino syntax
--------------------		|---------------------	|--------------
myservo = Servo_attach(pin);	|myservo_attach(pin);	|myservo.attach(pin);
Servo_write(myservo, val);	|myservo_write(val);	|myservo_write(val);
val = Servo_read(myservo);	|val = myservo_read();	|val = myservo_read();


### Optimizing the handle_interrupts() function

SDCC compiles this function very inefficiently. The code size is around 500
Bytes, and it is executed as part of the CC interrupt routine. Expected CPU
load for a full servo group of 12 servos is approx. 2%.

(Calculated for 16MHz CPU clock, 13 interrupts every 20ms = 650
interrupts/sec, approx. 500 clock cycles each)


### Using more than one CC channel per timer

it might be possible to use all capture+compare (CC) channels of one timer
at the same time, attaching one servo group to each CC-channel. This way it
would be possible to serve up to 48 servos using the four CC-channels of
timer TIM1. Monitoring the repetion period might become a little complex, as
it must be ensured that all servos on all channels have finshed before.


[Back to the main page](../index.html)
