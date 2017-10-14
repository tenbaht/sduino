## Sduino: Small Devices Arduino

Getting started on the STM8 CPU the easy way by using an Arduino-like
programming API.

This project makes the most important features of the Arduino API available
for the STM8S. It is based on free tools that are available for Linux,
MacOS, and Windows: SDCC, make, and stm8flash.

Since it's based on the SDCC Small Devices compiler, I called it "Small
Devices -uino" or "Small-duino".

This project is not supposed to be “better than Arduino”. It’s purpose
is to give you a head start into a different CPU architecture if you happen
to have a professional need or a private desire for it.

Please find more detailed information about the supported boards, the needed
tools and the library APIs on the
[project website](https://tenbaht.github.io/sduino/).

For bug reports and other issues tightly related to the repository content
please use the
[github issue tracker](https://github.com/tenbaht/sduino/issues).

For general discussions and suggestions on Sduino it might be more
convinient to open a thread on the
[STM8 board](http://stm32duino.com/viewforum.php?f=52) of the stm32duino
forum. It will help to catch my attention to prefix your topic with
"[sduino]".


## Table of Contents

   * [Project Website](https://tenbaht.github.io/sduino/)
   * [Usage example](#usage)
   * [Supported hardware](#supported-hardware)
   * [Why use a STM8 instead of an ATmega?](#why-use-a-stm8-instead-of-an-atmega)
   * [Library support](#library-support)
   * [Compatibility with the Arduino world](#compatibility-with-the-arduino-world)




## Usage example

If you have ever used the Arduino environment before you will feel at home
right away, despite this project beeing based on a makefile rather than the
full Arduino IDE. But don't be afraid, it is based on the amazing
[Arduino.mk makefile](https://github.com/sudar/Arduino-Makefile) by
[Sudar](http://sudarmuthu.com>) to control the build process, that makes
everything very easy.

First, let's make an LED blink using the Blink example from Arduino:

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
int led = LED_BUILTIN;

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


## Supported hardware

Anything with an STM8S103 or STM8S003 should work. For example, the
70-cent-[STM8S103 breakout boards](https://tenbaht.github.io/sduino/hardware/stm8blue),
[ESP14 Wifi boards](https://tenbaht.github.io/sduino/hardware/esp14),
or even the [W1209 thermostat boards](https://www.aliexpress.com/wholesale?SearchText=w1209)
that are abundant on aliexpress and Amazon. All you need to get started is a
simple ST-Link/V2 compatible flash programmer, available for less then $3
from China.

The STM8 series by ST might be the cheapest CPUs on the market while still
being as powerful as the ATmega series. That makes them every Chinese
engineer's darling and chances are pretty good that you will find an '003 if
you crack open any cheap appliance like an irrigation, temperature, or
charging controller.

The '003 or '103 CPUs feature a 16MHz internal oscillator, 8kB flash, 1kB
RAM and 128 or 640 byte EEPROM. They both include a UART, SPI, I2C, PWM,
five 10 bit ADC inputs, 3 timers, and up to 14 I/O pins - quite similar to
an Atmel ATmega8 as it was used on the first Arduino boards.

Support for the more powerful
[STM8S105 Discovery board](https://tenbaht.github.io/sduino/hardware/stm8sdiscovery)
is very fresh, but should work now. At least Blink.c works already. These
amazing $9 boards offer even more resources, come with a build-in flash
programmer and are very similar to an Arduino Uno with an ATmega328 CPU.

Find more information here: [Supported Boards](https://tenbaht.github.io/sduino/hardware)



## Why use a STM8 instead of an ATmega?

The fairly new ESP-14 module includes a STM8S003F3P6. Wifi and a
programmable I/O-CPU for just over two dollars - that might be the most
compelling reason to get started on the STM8S series. Apart from pure
curiosity and eagerness to learn something new, of course.

The simple STM8S103F breakout boards are powerful and dirt cheap. They cost
well under one dollar. You can get three boards and one flash programmer
together for well under five dollars on http://www.aliexpress.com/ ,
including shipping from China.

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



## Library support

The most important libraries and parts of the Arduino core system are
ported for the STM8 already: analog input and output, digital input and
output, delay(), millis(), Serial, SPI, and I2C.

There are ported versions of the standard libraries Servo, Stepper, and
LiquidCrystal for motor control and the popular text LCDs based on the
HD44780 controller.

For more sophisticated output there are libraries for Nokia-5110-type
graphical LCDs and SSD1306-based graphical OLED displays.



## Compatibility with the Arduino world

I adopted the Arduino core functionality for the STM8S to set up a simple
programming environment. But unfortunatly there is no free C++ compiler
for these CPUs. This makes it impossible to do a full port of the whole
enviroment and integrate it with the Arduino IDE and build system as it
has been done for the STM32 and the ESP8266.

This is not a drop-in replacement for an AVR, but the programming API is
still very, very similar. Thanks to some C preprocessor magic it is often
enough to just move over the opening bracket of the class instanciation
statement and to replace the dot in a method call for an underscore.

The whole Arduino build system is deeply based on the assumption of
processing C++ source files. I am not sure if it would be even possible to
configure a build process based only on C files without modifing the IDE
sources. This makes a full IDE integration very unlikely.

