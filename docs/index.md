# Sduino

Porting the most important features of the Arduino API to the STM8S.

Within a few minutes you are ready to compile and upload your first
STM8S-based project while still retaining the flexibility to use ST's SPL
functions.

All you need to get started is a simple STM8S103F breakout board for 70
cents and a ST-Link V2 compatible flash programmer for $2.50. Three boards
and one flash programmer together are available for well under five dollars
including shipping on [aliexpress](http://www.aliexpress.com/).

*Amazing!*


## Usage

If you have ever used the Arduino environment before you will feel at home
right away, despite this project beeing based on a makefile rather than the
full Arduino IDE. But don't be afraid, it is based on the amazing
[Arduino.mk makefile](https://github.com/sudar/Arduino-Makefile) by
[Sudar](http://sudarmuthu.com>) to control the build process, that makes
everything very easy.

Let's blink an LED using the Blink example from Arduino:

```c
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
*/

#include <Arduino.h>

// Pin 13 has an LED connected on most Arduino boards.
// Pin 3 for the STM8S103 break out board
// give it a name:
int led = LED_BUILDIN;

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
Windows. [Installation instructions](install.md)

It uses the small devices C compiler (SDCC) for compiling,
[stm8flash](https://github.com/vdudouyt/stm8flash) for uploading the binary
to the CPU, and simple Makefiles for the build process.

[More information on using SDCC](sdcc.md)

Support for the Cosmic compiler under Windows and integration into the ST
visual developer IDE might be possible, but is not done (yet?).

The build process is controlled by a makefile based on the amazing
[Arduino.mk makefile](https://github.com/sudar/Arduino-Makefile) by
[Sudar](http://sudarmuthu.com>).



## Supported hardware

* The [one-dollar-boards](hardware/stm8blue.md): A simple STM8S103 breakout
  board build around a CPU STM8S103F3P6. It costs less than a dollar. The
  CPU features a 16MHz internal oscillator, 8kB flash, 1kB RAM, 640 byte
  EEPROM. It includes an UART, SPI, I2C, PWM, 10 bit ADC, 3 timer, and up to
  14 I/O pins - quite similar to an Atmel ATmega8.

* The [ESP14 Wifi-boards](hardware/esp14.md) are very similar. They are
  basically a variant of these boards with an added ESP-01 Wifi-module. Almost
  all programs should run on those chinese Wifi-enabled gems as well.

* The [STM8S105Discovery-boards](hardware/stm8sdiscovery.md) are very similar
  to an Arduino Uno with an ATmega328 CPU. The support for the used STM8S105
  CPU is still quite fresh but it should work now.




## Compatibility with the Arduino world

I adopted the Arduino core functionality for the STM8S to set up a simple
programming environment. But unfortunatly there is no free C++ compiler
for these CPUs. This makes it impossible to do a full port of the whole
enviroment and integrate it with the Arduino IDE and build system as is
has been done for the STM32 and the ESP8266.

This is not a drop-in replacement for an AVR, but the programming API is
still very, very similar. Adopting existing libraries from C++ to C for use
with the simplified C API is often easy and can be done quite fast,
depending on the degree of dependency on specific hardware features.

The whole Arduino build system is deeply based on the assumption of
processing C++ source files. I am not sure if it would be even possible to
configure a build process based only on C files without modifing the IDE
sources. This makes a full IDE integration very unlikely.

Using a converter/compiler like
[cfront](https://en.wikipedia.org/wiki/Cfront) to translate from C++ to C
might be an option.


## Included libraries
Some Arduino libraries are already ported to C-syntax. The resulting API is
still very close to the C++ version and porting an existing application is
not hard. Check out the [API migration guidelines](api.md) for details.

#### Communication

* [SPI](api/SPI.md): Real hardware-SPI up to 10MHz.
* [I2C](api/I2C.md): Port of the I2C master library by Wayne Truchsess
* HardwareSerial: The standard serial interface.

#### Displays

* [LiquidCrystal](api/LiquidCrystal.md): HD44780 based text LCDs
* [PCD8544](api/PCD8544.md): Monochrome graphical LCD based on the PCD8544
  controller like the Nokia 5110 display. SPI mode only.
* [Mini_SSD1306](api/Mini_SSD1306.md): SSD1306-based monochrome OLED displays
  with 128x64 pixels. I2C support only.

#### Motor control

* [Stepper](api/Stepper.md): Stepper motors with 2, 4 or 5 phases.
* [Servo](api/Servo.md): Up to 12 servos using only 1 timer.



## Float arithmetics

Floating point arithmetics is supported by the SDCC standard library, but it
comes at a pretty high cost in terms of code space and CPU load. This is how
much the generated code grows by using a single float operation compared to 
using a long int:

Floating point operation	|approx. code size
--------------------		|---------
addition			| 736 Bytes
subtraction			| 754 Bytes
division			| 673 Bytes
multiplication			| 907 Bytes
sinf() or cosf()		| 3346 Bytes
log10f()			| 3437 Bytes

The Arduino standard example '01. Basics/ReadAnalogVoltage' is a very simple
simple program with only very little floating point arithmetics. But it
already uses 7336 bytes of flash. A similar sketch using integer arithmetics
results in much more compact code occuping only 3791 bytes.

Float does work, but is better to be avoided and replaced by fixed point
arithmetics whenever possible.



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
Print  
`pulseInLong()`  
[SPI](api/SPI.md):
  working, no interrupt support  
[LiquidCrystal](api/LiquidCrystal.md):
  Text LCD based on the HD44780 controller  
[PCD8544](api/PCD8544.md):
  Nokia 5110 type displays  
[Mini_SSD1306](api/Mini_SSD1306.md):
  Monochrome OLED displays based on the SSD1306 controller
[Stepper](api/Stepper.md):
  Multi-instance design for more than one stepper at a time  
[Servo](api/Servo.md):
  Multi-instance design for more than one servo at a time)  

#### implemented and partly working
Wire/I2C  

#### tested, but not working
`alternateFunctions()`  

#### not tested
`ShiftIn()`  
`random()`  
`srandom()`  

#### not implemented
`tone()`  
`noTone()`  
`pulseIn()`  
module WCharacter  
module WString  


#### Unresolved problems

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

The simple STM8S103F breakout boards are powerful and dirt cheap. They cost
well under one dollar. You can get three boards and one flash programmer
together for well under five dollars on http://www.aliexpress.com/ ,
including shipping from China.

The major downside of this CPU series is the lack of information and
community support for the STM8. The community support and the sheer number
of existing libraries for all kinds of sensors and hardware is outstanding
in the Arduino world. If you just want to get something done, go for an
Arduino board. Nothing will give you faster and easier results.

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

**Upgrade path**: The peripheral units of the STM8 are identical or at least
very, very similar to the ones used with the STM32 family of 32 bit
ARM-Cortex CPUs. This makes it is relatively easy to migrate existing
software between the 8- and the 32-bit world. This is quite unique among the
other CPUs.




## Further reading and application notes

[project documentation files](https://github.com/tenbaht/sduino/blob/master/docs/index.md)
More in detail information about supported boards, tools and the API.

[Quick introduction to the Arduino.mk makefile]
(http://hackaday.com/2015/10/01/arduino-development-theres-a-makefile-for-that/)

[PM0051](http://www.st.com/resource/en/programming_manual/cd00191343.pdf):
STM8AF Flash programming manual  
[UM0470](http://www.st.com/resource/en/user_manual/cd00173911.pdf):
STM8 SWIM protocol and debug manual  
[AN2658](http://www.st.com/resource/en/application_note/cd00176594.pdf):
Using the analog-to-digital converter of the STM8S microcontroller  

Many examples and presentations about the STM8S:  
https://github.com/VincentYChen/STM8teach  
It contains the SPL examples from ST, the most useful resource on the STM8:  
https://github.com/VincentYChen/STM8teach/tree/master/code/Project/STM8S_StdPeriph_Examples

Hardware and pinouts of several ST-Link compatible flash tools:
https://wiki.cuvoodoo.info/doku.php?id=jtag

Using the ADC:  
http://blog.mark-stevens.co.uk/2012/09/single-scan-adc-on-the-stm8s/  

Example for RS-232 handling with SPL:  
https://sourceforge.net/p/oggstreamer/oggs-stm8-firmware-001/ci/master/tree/rx_ringbuffer.c  

[AN3139](http://www.st.com/resource/en/application_note/cd00262293.pdf):
Migration guideline within the STM8L familiy  






## Modifications for the sdcc example programs

blinky.c: LED pin assignment

**uart.c**:  pin assignment (TX is at PD5, RX is at PD6).  
The UART is sending at 1200 Baud => CPU clock only 2MHz instead of 16MHz.
The clock divider needs to be configured or a different baud rate prescale value
has to be used. Pitfall: The register address for the clock divider is
different for the STM8S and the STM8L.




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


