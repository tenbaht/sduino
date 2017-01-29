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

This whole thing is far from being a finished product. It is in alpha stage,
but still already useful. It solved its purpose for me, it might me useful
for others as well. The documentation is incomplete and written in a wild
mix of English and German, but hopefully you can still figure it out.

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

The simple breakout boards are build around a CPU STM8S103F3P6 with 16MHz internal
oscillator, 8kB flash, 1kB RAM, and 640 byte EEPROM. The CPU includes a
UART, SPI, I2C, PWM, 10 bit ADC, 3 timer, and up to 14 I/O pins - quite similar to the Atmel ATmega8.

One (red) LED is connected to GPIO PB5 (CPU pin 11). The push button is for
reset. The CPU runs on 3.3V, a linear regulator is integrated on the
board. The micro USB connector is only for (5V) power supply, the data lines
are not connected.

All CPU pins are easily accessible on (optional) pin headers
(pitch 2.54mm, perfect for breadboards).


I am using the ST-Link V2 compatible flash tool in the green plastic
housing. The one in the metal housing uses a different pinout.

Connection to the green flashtool:

Signal name	|CPU board	|Flash tool	|Metal flash tool
------ 		|-----:		|-----: 	|-----:
3V3    		|1      	|2      	| 7
SWIM   		|2      	|5      	| 5
GND    		|3      	|7      	| 3
NRST   		|4      	|9      	| 1

My breakout boards came preprogrammed with a blink program and with active
write protection bits. For unlocking before first use:

	stm8flash -cstlinkv2 -pstm8s103?3 -u


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

### LiquidCrystal
Supports text LCD based on the HD44780 and compatibles, that includes almost
all character LCDs up to 4x40 Characters.


### PCD8544
Supports monochrome graphical LCD based on the PCD8544 controller like the
popular Nokia N5110 display. Only SPI mode supported. The library is a very
much simpified version of the Adafruit library optimized for a minimal memory
footprint. Uses soft-SPI, does not need the SPI pins.


### SPI
Real hardware-SPI up to 10MHz.


### I2C
The
[I2C master library](http://www.dsscircuits.com/articles/arduino-i2c-master-library)
by Wayne Truchsess offers some significant advantages over the Wire/TWI
library included in the standard arduino environment: It fixes some possible
deadlock situations, it allows for communication using a repeated start
condition as required by some devices, the code is much more compact and the
structure is easier to understand.

The current state of the port does not include the deadlock protection,
though.


### ssd1306
Driver for SSD1306-based monochrome OLED display with 128x64
pixels. I2C support only. Based on the Adafruit-libray.


### Stepper
For stepper motors with 2, 4 or 5 phases. This library has a slightly
diffent user interface than the usual singleton libraries. This allow it to
handle more than one stepper per Sketch.
[API description](api/Stepper.md)




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
[Stepper](api/Stepper.md) (multi-instance design for more than one stepper at a time)  

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



## Differences from the original Arduino environment

The original Arduino environment uses C++ syntax while sduino can only use
plain C syntax. Luckily, not many C++ features are used and in most cases a
conversion is not very hard. See the [migration guildelines](migration.md)
for details.


### Additional output pin modes

|Pin mode		|Pin properties
|---------------------	|------------------------------------
|`OUTPUT`		|output, push-pull, slow mode (default)  
|`OUTPUT_OD`		|output, open drain, fast mode  
|`OUTPUT_FAST`		|output, push-pull, fast mode  
|`OUTPUT_OD_FAST`	|output, open drain, fast mode  


### Timer

`millis()` uses timer4. The prescaler and end value is calculated at compile
time for a cycle time as close to 1ms as possible. Default values @16Mhz:
prescaler=64, counter cycle=250 (end value=249), resulting in exactly 1ms
intervals.


### Other modifications

`makeWord(unsigned char, unsigned char)` is an inline function now.


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



## ST Standard Library

Can be [downloaded from the ST website]
(http://www.st.com/en/embedded-software/stsw-stm8069.html)
(free registration required). Don't miss the Examples folder within the
downloaded zip file. This is the most useful reference on using this library
and programming the STM8 in general.

For use with SDCC the library needs to be patched:

	git clone https://github.com/g-gabber/STM8S_StdPeriph_Driver.git
	git clone https://github.com/gicking/SPL_2.2.0_SDCC_patch.git
	cp ../STM8S_SPL_2.2.0/Libraries/STM8S_StdPeriph_Driver/inc/stm8s.h .
	patch -p1 < ../SPL_2.2.0_SDCC_patch/STM8_SPL_v2.2.0_SDCC.patch
	cp -av  ../STM8S_StdPeriph_Lib/Project/STM8S_StdPeriph_Template/stm8s_conf.h .
	cp -av  ../STM8S_StdPeriph_Lib/Project/STM8S_StdPeriph_Template/stm8s_it.h .

SDCC uses .rel as the file extension for its object files.

Additional patch required for stm8s_itc.c:

```diff
--- stm8s_itc.c~	2014-10-21 17:32:20.000000000 +0200
+++ stm8s_itc.c	2016-12-11 21:56:41.786048494 +0100
@@ -55,9 +55,12 @@
   return; /* Ignore compiler warning, the returned value is in A register */
 #elif defined _RAISONANCE_ /* _RAISONANCE_ */
   return _getCC_();
-#else /* _IAR_ */
+#elif defined _IAR_ /* _IAR_ */
   asm("push cc");
   asm("pop a"); /* Ignore compiler warning, the returned value is in A register */
+#else /* _SDCC_ */
+  __asm__("push cc");
+  __asm__("pop a"); /* Ignore compiler warning, the returned value is in A register */
 #endif /* _COSMIC_*/
 }
```



Now the library can be compiled for the STM8S103 using this Makefile:

```make
CC=sdcc
AR=sdar
CFLAGS=-c -mstm8 -DSTM8S103 -I ../inc --opt-code-size -I.
LDFLAGS=-rc
SOURCES= \
	stm8s_adc1.c	stm8s_awu.c	stm8s_beep.c	stm8s_clk.c \
	stm8s_exti.c	stm8s_flash.c	stm8s_gpio.c	stm8s_i2c.c \
	stm8s_itc.c	stm8s_iwdg.c	stm8s_rst.c	stm8s_spi.c \
	stm8s_tim1.c	stm8s_tim2.c	stm8s_tim4.c	stm8s_uart1.c \
	stm8s_wwdg.c

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_LINK=$(SOURCES:.c=.rel)
EXECUTABLE=stm8s.lib

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
$(AR) $(LDFLAGS) $(EXECUTABLE) $(OBJECTS_LINK)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.lib *.rst *.rel *.lst *.ihx *.sym *.asm *.lk *.map
	rm -f $(EXECUTABLE)
```

This library can now be used for linking with blink_spl or uart_spl. The
files stm8s_conf.h and stm8s_it.h are still needed for compilation.

The linker does not remove individual unused functions from an object file,
only complete object files can be skipped.  
**=> for building a library it is better to separate all functions into
individual source files **

The SPL folder in this archive contains a script `doit` to separate the
functions before compilation.
FIXME: description needed

Erklärung wie zumindest die Interrupt-Vektoren in die eigene Datei kommen
können:
http://richs-words.blogspot.de/2010/09/stm8s-interrupt-handling.html



### Interrupts

Namen definiert in stm8s_itc.h
Interrupt-Routine definieren:

```c
/* UART1 TX */
void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX)
{
}
```

Jetzt muss noch das passende IRQ-Enable-Flag gesetzt werden und Interrupt
generell freigegeben werden, also hier:

```c
UART1_ITConfig(UART1_IT_TXE, ENABLE);
enableInterrupts();
```

Unklar ist, was die ITC-Prioritäten bewirken. Es geht jedenfalls auch ohne:

```c
ITC_DeInit();
ITC_SetSoftwarePriority(ITC_IRQ_UART1_TX, ITC_PRIORITYLEVEL_2);
```






## Programmer

STM8 uses the SWIM protocol, STM32 uses SWD protocol.

| STM8-Board	| SWIM-Verbinder P3
| ----------	| -----------------
| 1		| 3V3
| 2		| SWIM (PD1)
| 3		| GND
| 4		| NRST


Discovery STM32F0308 as ST-Link/V2 (SWD only, not usable for the STM8):

|Pin out CN3	| SWD
|-----------	| --------------
|1		| ? detect oder so?
|2		|JTCK/SWCLK
|3		|GND
|4		|JTMS/SWDIO
|5		|NRST
|6		|SWO


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




## Modifications for the sdcc example programs

blinky.c: LED pin assignment

**uart.c**:  pin assignment (TX is at PD5, RX is at PD6).  
The UART is sending at 1200 Baud => CPU clock only 2MHz instead of 16MHz.
The clock divider needs to be configured or a different baud rate prescale value
has to be used. Pitfall: The register address for the clock divider is
different for the STM8S and the STM8L.



## Pin number mappings

The Arduino environment uses its own pin numbering scheme independent from
the physical CPU pin numbers. Many Arduino sketches and libraries contain
hard-coded assumptions about the number of pins with special functions.
Ideally, all these numbers would be the same and all programs could be
compiled without changes.

[Here](docs/pin_mapping.html) I discuss some possible pin mapping and check
how close we could get the the ideal mapping. Unfortunatly, it turns out
that a perfect mapping is not possible.

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




## Notes for the Arduino port

### Additional compile-time flags

Some internal details can be influenced by setting compile-time defines
using the `CFLAGS=-Dflagname` line in the Makefile.

Compile-time flag		| Purpose
-----------------		| --------
`SUPPORT_ALTERNATE_MAPPINGS`	| Allow the use of `alternateFunctions()`
`ENABLE_SWIM`			| Do not disable the remote debugging
function on the SWIM pin. This means that this pin can not be used for
normal I/O functions.
`USE_SPL`			| Use SPL functions for I/O access instead
of direct register accesses. Useful only for debugging and porting to other
CPU variants. Do not use for regular development.
``				|
``				|
``				|




#### Use of the timers
timer1: PWM for PC6, PC7 (8,9), could be used for ADC  
timer2: PWM for PA3 (2)  
timer4: millis()  

#### ADC
the prescaler is initialised for an ADC clock in the range of 1..2 MHz. The
minimum prescaler value is 2, so for a clock speed of less than 2 MHz the
required minimum ADC clock frequency can not be reached anymore.

#### Mapping of logical pin numbers to register addresses
Die ganze Pin->Portadressen-Arithmetik könnte komlett entrümpelt werden. Statt
Tabellen fest im Code enthalten.

#### Inefficient compilation
`digitalWrite` wird spektakulär umständlich übersetzt. Hier lohnt sich
Handassembler.

#### Accessing the alternate pin functions
Added `alternateFunction()` to allow switching of some pins to their alternate
functions. This allows for three more PWM pins, but maybe it adds to much
complexity for the Arduino API. Not sure if it should stay. Has to be
enabled by defining `SUPPORT_ALTERNATE_MAPPINGS`.


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



### Useful CPU features that are not supported by the Arduino API

**Input-Capture-Mode:** Available for all four channels, at least for timer1. Would be great for precise time measurements. Maybe build a library?

**Encoder interface mode:** Kann von Haus aus mit Quadratur-Encodern umgehen
und in Hardware zählen -> perfekt für die Druckerschlitten-Motorsteuerung.

