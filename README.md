# sduino

Getting started on the STM8 CPU the easy way by using an Arduino-like
programming API.

This project makes the most important features of the Arduino API available
for the STM8S. It is based on free tools that are available for Linux, MacOS, and
Windows.

Please find more detailed information about the supported boards, the needed tools
and the library APIs on the
[project documentation files](https://github.com/tenbaht/sduino/blob/master/docs/index.md).




## Table of Contents

   * [Project Website](blob/master/docs/index.md)
   * [Usage example](#usage)
   * [Installation](#installation)
   * [Supported hardware](#supported-hardware)
   * [Why use a STM8 instead of an ATmega?](#why-use-a-stm8-instead-of-an-atmega)
   * [Compatibility with the Arduino world](#compatibility-with-the-arduino-world)
   * [Current status and to-do list](#current-status-and-to-do-list)
   * [Further reading and application notes](#further-reading-and-application-notes)



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




## Installation

This project uses the small devices C compiler (SDCC) for compiling,
[stm8flash](https://github.com/vdudouyt/stm8flash) for uploading the binary
to the CPU, and simple Makefiles for the build process. Support for the
Cosmic compiler under Windows and integration into the ST visual developer
IDE might be possible, but is not done (yet?).

This is a quick overview for the installation on a Linux system. Check the
[installation instructions](https://github.com/tenbaht/sduino/blob/master/docs/install.md)
for other systems.


Download and extract the
[lastest snapshot build for SDCC](http://sdcc.sourceforge.net/snap.php) to
`/opt`:

	sudo mkdir /opt
	sudo tar xvjf ~/Downloads/sdcc-snapshot* -C /opt

Make sure make is available:

	sudo apt-get install make

Download/clone this repository somewhere convinient (but separate from your
existing Arduino files, at least for now):

	git clone git@github.com:tenbaht/sduino.git

compile an example sketch:

	cd sduino/sduino/examples/01.Basics/Blink
	make


More details and instructions for MacOS and Windows in the
[installation instructions](https://github.com/tenbaht/sduino/blob/master/docs/install.md).




## Supported hardware

[STM8S103 breakout board](blob/master/docs/hardware/stm8blue.md):
works.  
[ESP14 Wifi board](blob/master/docs/hardware/esp14.md): works.  
[STM8S105 Discovery board](blob/master/docs/hardware/stm8disco.md):
not supported yet, but work in progress.  

Find more information here: [Supported Boards](blob/master/docs/hardware.md)



## Why use a STM8 instead of an ATmega?

The fairly new ESP-14 module includes a STM8S003F3P6. Wifi and a
programmable I/O-CPU for just over two dollars - that might be the most
compelling reason to get started on the STM8S series. Apart from pure
curiosity and eagerness to learn something new, of course.

The simple STM8S103F breakout boards are powerful and dirt cheap. They cost
well under one dollar. You can get three boards and one flash programmer
together for well under five dollars on http://www.aliexpress.com/ ,
including shipping from China.

*Amazing!*

The major downside is the lack of information and community support for the
STM8. The community support and the sheer number of existing libraries for
all kinds of sensors and hardware is outstanding in the Arduino world. If
you just want to get something done, go for an Arduino board. Nothing will
give you faster and easier results.

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



## Current status and to-do list

#### tested and working
`pinMode()`  
`digitalWrite()`  
`analogRead()`  
`delay()`  
`analogWrite()`  
`ShiftOut()`  
WMath: `map()`  
[HardwareSerial]  
Print (without float)  
[SPI](https://github.com/tenbaht/sduino/blob/master/docs/SPI.md):
  working, no interrupt support  
[LiquidCrystal](https://github.com/tenbaht/sduino/blob/master/docs/api/LiquidCrystal.md):
  Text LCD based on the HD44780 controller  
[PCD8544](https://github.com/tenbaht/sduino/blob/master/docs/api/PCD8544.md):
  Nokia 5110 type displays  
[Mini_SSD1306](https://github.com/tenbaht/sduino/blob/master/docs/api/Mini_SSD1306.md):
  Monochrome OLED displays based on the SSD1306 controller
[Stepper](https://github.com/tenbaht/sduino/blob/master/docs/api/Stepper.md):
  Multi-instance design for more than one stepper at a time  
[Servo](https://github.com/tenbaht/sduino/blob/master/docs/api/Servo.md):
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
`yield()`  
`tone()`  
`noTone()`  
`pulseIn()`  
`pulseInLong()`  
module WCharacter  
module WString  


#### Unresolved problems

The compile environment needs to detect which interrupts are actively used
and link only the needed ones into the binary. See test/digitalWrite:
Compiling with the straight Makefile.classic does not add UART interrupt
routines. But when using the sduino.mk Makefile the two UART interrupt
routines are pulled into the binary by the interrupt table in main.c.



## Alternative solutions

The SPL (standard peripheral library) [offered by
ST](http://www.st.com/en/embedded-software/stsw-stm8069.html) is very
powerful and very similar to the one used for the STM32 CPU series offering
a relatively easy upgrade path in case a project outgrows the capabilities
of the 8-bit STM8 series. But using that library is not very intuitive and
still requires a fairly detailed knowledge of the CPU internals.




## Further reading and application notes

[project documentation files](https://github.com/tenbaht/sduino/blob/master/docs/index.md).
More in detail information about supported boards, tools and the API.

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

Using the ADC:  
http://blog.mark-stevens.co.uk/2012/09/single-scan-adc-on-the-stm8s/  

Example for RS-232 handling with SPL:  
https://sourceforge.net/p/oggstreamer/oggs-stm8-firmware-001/ci/master/tree/rx_ringbuffer.c  

[AN3139](http://www.st.com/resource/en/application_note/cd00262293.pdf):
Migration guideline within the STM8L familiy  
