# sduino

Getting started on the STM8 CPU the easy way by using an Arduino-like
programming API.

The SPL (standard peripheral library) [offered by
ST](http://www.st.com/en/embedded-software/stsw-stm8069.html) is very
powerful and very similar to the one used for the STM32 CPU series offering
a relatively easy upgrade path in case a project outgrows the capabilities
of the 8-bit STM8 series. But using that library is not very intuitive and
still requires a fairly detailed knowledge of the CPU internals.
[compiling the SPL with SDCC](spl.html)

The Arduino project was very successful in offering a simplified API hiding
most of the complexity of embedded system programming while still allowing
for advanced programming technics.

This project makes the most important features of the Arduino API available
for the STM8S. I needed to port an existing project from an ATmega to a
better suited (read: cheaper) platform. As the project is based on some
Arduino libraries porting parts of the Arduino environment was the logical
first step. After doing that porting the firmware was finished in a
couple of days.

This whole thing is far from being a finished product. It is in alpha stage,
but still already useful. It solved its purpose for me, it might me useful
for others as well. The documentation is incomplete and written in a wild
mix of English and German, but hopefully you can still figure it out.

All you need to get stated is a simple STM8S103F breakout board and a
ST-Link V2 compatible flash programmer. Three boards and one flash
programmer together are available for well under five dollars on
http://www.aliexpress.com/ .

*Amazing!*


## Sitemap

(This really should be the navigation menu on the side. Any way to do that
with github pages and the preinstalled jekyll-themes?)

1. [Introduction and Overview](index.html)
2. [API descriptions and migration guidelines](api.html)
  * [LiquidCrystal character LCD library](api/LiquidCrystal.html)
  * [PCD8544 libray for Nokia 5110-like graphical LCDs](api/PCD8544.html)
  * [Mini_SSD1306 library for monochrome OLED-displays](api/Mini_SSD1306.html)
  * [Stepper library](api/Stepper.html)
  * [Servo library](api/Servo.html)
3. [Supported Boards](hardware.html)
  * [stm8blue: simple breakout board, STM8S103](hardware/stm8blue.html)
  * [ESP14: Wifi board, STM8S003](hardware/esp14.html)
  * [STM8S105Discovery: Evaluation board made my ST](hardware/stm8disco.html)
4. [Ways to define a pin mapping](pin_mapping.html)
5. [Installing and using the SDCC compiler](sdcc.html)
6. [Using the SPL with SDCC and sduino](spl.html)
7. [c preprocessor macro magic](macro.html).




## Usage

If you have ever used the Arduino environment before you will feel at home
right away. The compilation is done on the command line, but thanks to the
amazing [Arduino.mk makefile](https://github.com/sudar/Arduino-Makefile) by
[Sudar](http://sudarmuthu.com>) you don't have to fiddle around, it is just
one simple command.

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
[More information on installing and using SDCC](sdcc.html)

Support for the Cosmic compiler under Windows and integration into the ST
visual developer IDE might be possible, but is not done (yet?).

The build process is controlled by a makefile based on the amazing
[Arduino.mk makefile](https://github.com/sudar/Arduino-Makefile) by
[Sudar](http://sudarmuthu.com>).



## Supported hardware

### STM8S boards

So far only the the
[simple STM8S103 breakout board](hardware/stm8blue.html)
is supported. This board is build around a CPU STM8S103F3P6 and costs less
than a dollar. The CPU features a 16MHz internal oscillator, 8kB flash, 1kB
RAM, 640 byte EEPROM. It includes an UART, SPI, I2C, PWM, 10 bit ADC, 3
timer, and up to 14 I/O pins - quite similar to an Atmel ATmega8.

It is very similar to the [ESP14 Wifi-boards](hardware/esp14.html). Most
programs should run on those chinese Wifi-enabled gems as well, but that is
still untested.

Support for the more powerful
[STM8S105Discovery-boards](hardware/stm8disco.html) is planned, but not
started yet. These are very similar to an Arduino Uno with an ATmega328 CPU.



### Flash tool

You need a special flash tools in order to program the CPU. As far as I know
there is no third-party product or software that implements the needed
communication protocol. But this is not a problem, as these tools are are
easily available and unbeliveably cheap (well under $3 on aliexpress).

There are to versions of the ST-Link V2 compatible flash tool available: One
in a green plastic housing and one in a USB-Drive-like (pink) metal housing.
Both work equally well, but they use a different pinout.

Both flash tools support both, the SWIM protocol for STM8 CPUs and the SWD
protocol for the STM32 CPUs.

Pinout of Chinese ST-Link V2-clone with green plasic housing:

		+-----+
	T_JRST	| 1  2|	3V3
	5V	| 3  4|	T_JTCK/T_SWCLK
	SWIM	  5  6|	T_JTMS/T_SWDIO
	GND	| 7  8|	T_JTDO
	SWIM RST| 9 10|	T_JTDI
		+-----+

Pinout of Chinese ST-Link V2-clone with metal housing:

		+-----+
	RST	| 1  2|	SWDIO
	GND	| 3  4|	GND
	SWIM	  5  6|	SWCLK
	3V3	| 7  8|	3V3
	5V	| 9 10|	5V
		+-----+

For Linux: required lines in /etc/udev/rules.d/99-stlink.rules:

	# ST-Link/V2 programming adapter

	# ST-Link V1
	#SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device",
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3744", MODE="0666", GROUP="plugdev"

	# ST-Link/V2, the china adapter with the green plastic housing
	#SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device", ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666"
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666", GROUP="plugdev"


The pinout of the SWIM connector P3 on my board fits the pinout of the flash
tool in the metal housing perfectly:

| STM8-Board	| SWIM-Verbinder P3
| ----------	| -----------------
| 1		| 3V3
| 2		| SWIM (PD1)
| 3		| GND
| 4		| NRST


The Discovery boards made by ST all feature a ST-Link interface as well, but
only the Discovery STM8S105 supports the SWIM protocol. The Discovery
STM32F0308 implements SWD only and is not usable for the STM8:

|Pin out CN3	| SWD
|-----------	| --------------
|1		| ? detect oder so?
|2		|JTCK/SWCLK
|3		|GND
|4		|JTMS/SWDIO
|5		|NRST
|6		|SWO



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



## Included libraries
Some Arduino libraries are already ported to C-syntax. The resulting API is
still very close to the C++ version and porting an existing application is
not hard. Check out the [API migration guidelines](api.html) for details.


### General communication

#### SPI
Real hardware-SPI up to 10MHz.


#### I2C
The
[I2C master library](http://www.dsscircuits.com/articles/arduino-i2c-master-library)
by Wayne Truchsess offers some significant advantages over the Wire/TWI
library included in the standard arduino environment: It fixes some possible
deadlock situations, it allows for communication using a repeated start
condition as required by some devices, the code is much more compact and the
structure is easier to understand.

The current state of the port does not include the deadlock protection,
though.


#### HardwareSerial
The standard serial interface.



### Display

#### LiquidCrystal
Supports text LCD based on the HD44780 and compatibles, that includes almost
all character LCDs up to 4x40 Characters.


#### PCD8544
Supports monochrome graphical LCD based on the PCD8544 controller like the
popular Nokia N5110 display. Only SPI mode supported. The library is a very
much simpified version of the Adafruit library optimized for a minimal memory
footprint. Uses soft-SPI, does not need the SPI pins.
[API description](api/PCD8544.html)


#### Mini_SSD1306
Driver for SSD1306-based monochrome OLED display with 128x64
pixels. I2C support only. Based on the Adafruit-libray Adafruit_SSD1306.
[API description](api/Mini_SSD.html)


### Motor

#### Stepper
For stepper motors with 2, 4 or 5 phases. This library has a slightly
diffent user interface than the usual singleton libraries. This allow it to
handle more than one stepper per Sketch.
[API description](api/Stepper.html)


#### Servo
This library can control a great number of servos. It makes careful use
of timers: the library can control 12 servos using only 1 timer.
[API description](api/Servo.html)




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
[Stepper](api/Stepper.html) (multi-instance design for more than one stepper at a time)  
[Servo](api/Servo.html) (multi-instance design for more than one servo at a time)  

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


