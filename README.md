# sduino

Getting started on the STM8 CPU the easy way by using an Arduino-like
programming API.

The SPL (standard peripheral library) [offered by
ST](http://www.st.com/en/embedded-software/stsw-stm8069.html) is very
powerful and very similar to the one used for the STM32 CPU series offering
a relatively easy upgrade path in case a project outgrows the capabilities
of the 8-bit STM8 series. But using that library is not very intuitive and
still requires a fairly detailed knowledge of the CPU internals.

The Arduino project was very successful in offering a simplified API hiding
most of the complexity of embedded system programming while still allowing
for advanced programming technics.

This project makes the most important features of the Arduino API available
for the STM8S. I needed to port an existing project from an ATmega to a
better suited (read: cheaper) platform. As the project is based on some
Arduino libraries porting parts of the Arduino environment was the logical
first step. After doing that porting the firmware was finished in a
couple of days.

Find more in detail information about the supported boards, needed tools and
the library APIs on the
[project website](https://tenbaht.github.io/sduino/).

All you need to get stated is a simple STM8S103F breakout board and a
ST-Link V2 compatible flash programmer. Three boards and one flash
programmer together are available for well under five dollars on
http://www.aliexpress.com/ .

*Amazing!*



## Usage

If you have ever used the Arduino environment before you will feel at home
right away. The compilation is controlled by a makefile based on the amazing
[Arduino.mk makefile](https://github.com/sudar/Arduino-Makefile) by
[Sudar](http://sudarmuthu.com>).

Let's blink an LED using the Blink example from Arduino:

```
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
*/

#include <Arduino.h>

// Pin 13 has an LED connected on most Arduino boards.
// Pin 3 for the STM8S103 break out board
// give it a name:
int led = PIN_LED_BUILDIN;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
```

All we need for a full build is this very basic `Makefile`:

```make
BOARD_TAG	= stm8sblue

include ../../sduino/sduino.mk
```

Compile and upload it:

	make upload

Done! Your first STM8 based project is up and running!



## Tools used

This project is based on free tools that are available for Linux, MacOS, and
Windows. It uses the small devices C compiler (SDCC) for compiling,
[stm8flash](https://github.com/vdudouyt/stm8flash) for uploading the binary
to the CPU, and simple Makefiles for the build process.

SDCC support for the STM8 is still quite fresh and not very mature. It
improves significantly from version to version. Be sure to use
[the latest snapshot build](http://sdcc.sourceforge.net/snap.php) from the
[project site on sourceforge](http://sdcc.sourceforge.net/), not the older
version that might be included in your distribution. Version 3.5.0 as
included with ubuntu 16.04 is definitly too old and compilation will fail
due to some compiler errors.
[More information on installing SDCC](sdcc.md)

Support for the Cosmic compiler under Windows and integration into the ST
visual developer IDE might be possible, but is not done (yet?).



## Supported hardware

[STM8S103 breakout board](https://tenbaht.github.io/sduino/hardware/stm8blue.html):
works  

[ESP14 Wifi board](https://tenbaht.github.io/sduino/hardware/esp14.html):
untested, but expected to work.

[STM8S105 Discovery board](https://tenbaht.github.io/sduino/hardware/stm8disco.html):
not supported yet, but work in progress.



## Compatibility with the Arduino world

I adopted the Arduino core functionality for the STM8S to set up a simple
programming environment. But unfortunatly there is no free C++ compiler
for these CPUs. This makes it impossible to do a full port of the whole
enviroment and integrate it with the Arduino IDE and build system as is
has been done for the STM32 and the ESP8266.

This is not a drop-in replacement for an AVR, but the programming API is
still very, very similar. Adopting existing libraries from C++ to C for
use with the simplified C API is often easy and can be done in a matter of
minutes, depending on the degree of dependency on specific hardware
features.

The whole Arduino build system is deeply based on the assumption of C++
source files. I am not sure if it would be even possible to configure a
build process based only on C files. This makes a full IDE integration
very unlikely.

Using a converter/compiler like `cfront` to translate from C++ to C might be
an option.



## Current status and to-do list

#### tested and working
`pinMode()`  
`digitalWrite()`  
`analogRead()`  
`delay()`  
`analogWrite()`  
`ShiftOut()`  
WMath: `map()`  
HardwareSerial  
Print (without float)  
SPI: working, no interrupt support  
LiquidCrystal (for text LCD based on the HD44780 controller)  
PCD8544 (for Nokia 5110 type displays)  
[Stepper](Stepper.md) (multi-instance design for more than one stepper at a time)  
ssd1306 (for monochrome OLED displays based on SSD1306 controller)  

#### implemented and partly working
Wire/I2C: no deadlock protection yet  

#### tested, but not working
`alternateFunctions()`  

#### not tested
`ShiftIn()`  
`random()`  
`srandom()`  

#### not implemented
`yield()`  
`tone()`  
`noTone()`  
`pulseIn()`  
`pulseInLong()`  
module WCharacter  
module WString  


The compile environment needs to detect which interrupts are actively used
and link only the needed ones into the binary. See test/digitalWrite:
Compiling with the straight Makefile.classic does not add UART interrupt
routines. But when using the sduino.mk Makefile the two UART interrupt
routines are pulled into the binary by the interrupt table in main.c.



## Why use a STM8 instead of an ATmega?

The fairly new ESP-14 module includes a STM8S003F3P6. Wifi and a
programmable I/O-CPU for just over two dollars - that might be the most
compelling reason to get started on the STM8S series. Apart from pure
curiosity and eagerness to learn something new, of course.

The community support and the sheer number of existing libraries for all kinds
of sensors and hardware is outstanding in the Arduino world. If you just
want to get something done, go for an Arduino board. Nothing will give you
faster and easier results.

For commercial use the STM8S offers some interesting advantages:

**Motor control**: The STM8 has a strong focus on motor and position control
systems. Things you need to handle yourself on an ATmega are implemented in
hardware and work independently of the state of the software. There is even
hardware support for quadrature encoders as used in position sensors and
rotary encoders.

**Low power modes**: The numbers in the datasheets don't look that different,
but in real life the STM8 can be powered two or three times longer using the
same battery capacity due to the finer control on the power modes (very,
very careful programming required).

**Value for the money**: 40 to 60 cents for a STM8 with 14 I/O pins compared to
$1.60-$3.00 for an ATmega8.




## Further reading and application notes

[sduino project website](https://tenbaht.github.io/sduino/):
More in detail information about supported boards, tools and the API.

[PM0051](http://www.st.com/resource/en/programming_manual/cd00191343.pdf):
STM8AF Flash programming manual  
[UM0470](http://www.st.com/resource/en/user_manual/cd00173911.pdf):
STM8 SWIM protocol and debug manual  
[AN2658](http://www.st.com/resource/en/application_note/cd00176594.pdf):
Using the analog-to-digital converter of the STM8S microcontroller  

Many examples and presentations about the STM8S:  
https://github.com/VincentYChen/STM8teach  
https://github.com/VincentYChen/STM8teach/tree/master/code/Project/STM8S_StdPeriph_Examples

Using the ADC:  
http://blog.mark-stevens.co.uk/2012/09/single-scan-adc-on-the-stm8s/  

Example for RS-232 handling with SPL:  
https://sourceforge.net/p/oggstreamer/oggs-stm8-firmware-001/ci/master/tree/rx_ringbuffer.c  

[AN3139](http://www.st.com/resource/en/application_note/cd00262293.pdf):
Migration guideline within the STM8L familiy  




## Pin number mappings

The Arduino environment uses its own pin numbering scheme independent from
the physical CPU pin numbers. Many Arduino sketches and libraries contain
hard-coded assumptions about the number of pins with special functions.
Ideally, all these numbers would be the same and all programs could be
compiled without changes.

[Here](https://tenbaht.github.io/sduino/pin_mapping.md)
I discuss some possible pin mapping strategies and check how close we could
get the the ideal mapping. Unfortunatly, it turns out that a perfect mapping
is not possible.

In the end I chose a simple geometric numbering for the square UFQFPN20
package starting with port pin PA1 and counting up from 0. This results in
this mapping:

sduino pin	| STM8S103 CPU port pin
----------	| ---------------------
 0-2		| PA1-PA3
 3-4		| PB5-PB4 (reverse order)
 5-9		| PC3-PC7
10-15		| PD1-PD6

serial: 14,15  
SPI: 2,7,8,9  
I2C: 3,4  
Analog: 6,11,12,14,15  
PWM: 2,5,6,12 plus either only 13 or 7-9 but not 13 (via alternate mapping)  

 + Easy and logical for use on a breadboard
 + Very clear and logical port pin ordering
 - Analog pins are still scattered around
 + TX and RX would be the rarely used analog pin numbers A3/A4 at
   the end of the analog pin number list
 + At least the analog pins are in data sheet order
 - All functions use totally different pin numbers than Arduino

I am still not really happy with this mapping. Instead of simplifing things
it only adds another layer of abstraction and confusion. To avoid this I
added definitions for the regular CPU pin names like `PA1` and `PD2`. In the
end, this notation seem a lot easier to me. I am open for suggestions for a
better pin number mapping.

The chosen pin mapping for the STM8S103 (possible alternate function in
paratheses):

|Phys. STM8 pin|Name	|Functions	|Geometrical mapping|special funcion
|---:	|---	|---			| ---	|---
|1	|PD4	|UART_CLK/T2-1/beep	|13	|PWM
|2	|PD5	|TX/Ain5		|14	|Analog A3
|3	|PD6	|RX/Ain6		|15	|Analog A4
|5	|PA1	|(OscIn, kein HS)	|0	|
|6	|PA2	|(OscIn, kein HS)	|1	|
|10	|PA3	|SS/T2-3		|2	|PWM
|11	|PB5	|SDA	LED		|3	|
|12	|PB4	|SCL			|4	|
|13	|PC3	|T1-3/[T1-n1]		|5	|PWM, (n~)
|14	|PC4	|T1-4/Ain2/[T1-n2]	|6	|PWM, Analog A0, (n~)
|15	|PC5	|SCK/[T2-1]		|7	|(~)
|16	|PC6	|MOSI/[T1-1]		|8	|(~)
|17	|PC7	|MISO/[T1-2]		|9	|(~)
|18	|PD1	|(SWIM)			|10	|
|19	|PD2	|Ain3/[T2-3]		|11	|Analog A1, (~~)
|20	|PD3	|Ain4/T2-2		|12	|PWM, Analog A2





### Performance compared with the original Arduino environment

Benchmarking the original Arduino examples from Arduino 1.0.5. The simple
Blinky compiles to 57 bytes of code, the total binary including the sduino
libraries is 1868 Bytes (0x74c).

So far, wiring_analog depends on wiring_digital, even when analogWrite is not
used. This could be solved by compiling the sduino functions separately into
a library.

|Name			|Code	|Total	|Linked files other than main and wiring
|---------------------	|------	|-----	|-----------------------------
|01. Basics/		|
|BareMinimum		|2	|1238	|-
|Blink			|57	|1870	|wiring_digital
|AnalogReadSerial	|205	|3452	|digital, analog, serial, print
|DigitalReadSerial	|57	|3160	|digital, serial, print
|Fade			|226	|2189	|digital, analog
|ReadAnalogVoltage	|	|	|float not yet implemented
|02. Digital/		|
|Debounce		|192	|2016	|digital

