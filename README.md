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

```
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

Support for the Cosmic compiler under Windows and integration into the ST
visual developer IDE might be possible, but is not done (yet?).



## Supported hardware

The simple STM8S103F breakout board as sold on aliexpress.com for well below
one dollar. Add a ST-Link V2 compatible flash programmer and you are set
for the STM8S world for less than five dollars (3 breakout boards and one
flash tool).

*Amazing!*

The breakout boards are build around a CPU STM8S103F3P6 with 16MHz internal
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

|Signal name	|CPU board	|Flash tool	|Metal flash tool
|------ 	|-----:		|-----: 	|-----:
|3V3    	|1      	|2      	| 7
|SWIM   	|2      	|5      	| 5
|GND    	|3      	|7      	| 3
|NRST   	|4      	|9      	| 1

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

**LiquidCrystal**:
Supports text LCD based on the HD44780 and compatibles, that includes almost
all character LCDs up to 4x40 Characters.


**SPI**


**I2C**: The
[I2C master library] (http://www.dsscircuits.com/articles/arduino-i2c-master-library)
by Wayne Truchsess offers some significant advantages over the Wire/TWI
library included in the standard arduino environment: It fixes some possible
deadlock situations, it allows for communication using a repeated start
condition as required by some devices, the code is much more compact and the
structure is easier to understand.

The current state of the port does not include the deadlock protection,
though.


**ssd1306**: Driver for SSD1306-based monochrome OLED display with 128x64
pixels. I2C support only. Based on the Adafruit-libray.




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



## Compiler

Tutorials:
http://www.cnx-software.com/2015/04/13/how-to-program-stm8s-1-board-in-linux/

STM8-Support only started with Version 3.4 in Ubuntu 14.10. For Ubuntu 14.4:

	add-apt-repository ppa:laczik/ppa
	apt-get update
	apt-get install sdcc

But even this version is fairly old and contains some known bugs. Better
download a current snapshot build from http://sdcc.sourceforge.net/ and
unpack it to `/opt/sdcc`. This requires a current version of libstdc++6:

	add-apt-repository ppa:ubuntu-toolchain-r/test
	apt-get update
	apt-get install libstdc++6

If you prefer to compile stm8flash yourself instead of using the Linux
binaries in the `tools` directory:

	git clone https://github.com/vdudouyt/stm8flash.git
	cd stm8flash
	make
	sudo make install

Download some example code:

	git clone https://github.com/vdudouyt/sdcc-examples-stm8.git
	cd sdcc-examples-stm8

The examples are meant for the STM8L, not the STM8S. This requires some
changes to account for the different pinout and register addresses (see below).
Finally upload the binary to the CPU:

	stm8flash -c stlinkv2 -p stm8s103?3 -w blinky.ihx



### Mixing assembler code with C code

c-code:

	stacktest(0x1234, 0x5678);

assember:

	push    #0x78
	push    #0x56
	push    #0x34
	push    #0x12
	call    _stacktest

resulting stack content (starting at [SP], using simulator sstm8):

	0> dch 0x17f9
	0x017f9 c0 80 ab 12 34 56 78 5b ....4Vx[

=> first paramter starts at [SP+3], MSB first.

#### Register assignment

**return values**:
8 bit values in A, 16 bit values in X, 32 bit values in Y/X (Y=MSB, X=LSB)

**register preservation**:
Not implemented for the STM8 (yet?). For some architectures SDCC implements
the possibility to mark a function that it does not effect the contents of
some registers:

	void f(void) __preserves_regs(b, c, iyl, iyh);




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
conversion is not very hard.



### Migrating existing code from C++ to C syntax

In most cases a conversion from C++ to C it is just a matter of exchanging a
dot for an underscore. A C++ method name `class.method()` becomes a C
function name `class_method()`.

This is possible since most libraries are written to be used as a singleton
anyway, so the fixed name prefix is not a problem.

There are two bigger problems left:



#### Polymorph functions

The concept of polymorphism does not exist for plain C. As a workaround
'mangled' function names are used for the different parameter type
combinations supported by the original polymorph methods.

Typical name extensions are: `_u` for unsigned values, `_i` for signed
integer values, `_c`for characters, `_s` for strings, `_n` for data
buffer/length combinations.

For more non-regular polymorphism the name extension is often related to the
different use cases or to the names of the given parameters. Refer to the
respective library header file for details.

Some examples of typical name changes:

C++ name				| C name
-------					| -------
`Print.print(int)`			| `Print_print_i`
`Print.print(unsigned)`			| `Print_print_u`
`Print.print(char)`			| `Print_print_c`
`Print.print(char *)`			| `Print_print_s`
`Print.print(char *buf, int len)`	| `Print_print_n`
`Print.print(unsigned n, int base)`	| `Print_print_ub`
`random(long)`				| `random`
`random(long howsmall, long howbig)`	| `random_minmax`




#### Inheritance from Print class

Most character output modules inherit methods from the Print class by
providing a virtual write method. A similar result can be achived by
providing a function pointer to the write function to be used to the print
functions.

This additional parameter is hidden from the user by providing more
convinient defines in every library that need to 'inherit' functions from
Print. This way 
`lcd.print("Hello World!")` becomes
`lcd_print_s("Hello World!")` and
`Serial.print("Hello World!")` becomes
`Serial_print_s("Hello World!")`. Both call the same code from Print, but
with different function pointers to their own putchar/write function.




#### Additional output pin modes

|Pin mode		|Pin properties
|---------------------	|------------------------------------
|`OUTPUT`		|output, push-pull, slow mode (default)  
|`OUTPUT_OD`		|output, open drain, fast mode  
|`OUTPUT_FAST`		|output, push-pull, fast mode  
|`OUTPUT_OD_FAST`	|output, open drain, fast mode  


#### Timer

`millis()` uses timer4. The prescaler and end value is calculated at compile
time for a cycle time as close to 1ms as possible. Default values @16Mhz:
prescaler=64, counter cycle=250 (end value=249), resulting in exactly 1ms
intervals.


#### Other modifications

`makeWord(unsigned char, unsigned char)` is an inline function now.



## Further reading and application notes

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



### Notes on SDCC

The linker `sdld` does not automatically link the object file for main.c if it
is part of a library. It must be part of the list of object files. (Important
for the build process with Arduino.mk)

Befehl `__critical{..}` sollte eigentlich den vorherigen Interrupt-Zustand
wiederherstellen, es wird aber einfach ein festes Paar sim/rim produziert.
Mit "push cc; sim" und "pop cc" klappt es im Simulator, aber nicht in der
Realität.

Für jeden benutzten Interrupt __muss__ ein Prototyp in der Datei stehen, in
der auch main() definiert ist. Aber für jeden Prototypen, für den es keine
Funktion gibt, ergibt einen Linkerfehler. Das erklärt den Sinn von stm8s_it.h
im Projektverzeichniss. Eine Arduino-ähnliche Umgebung muss diese Datei also
nach Analyse aller Sourcen selber erzeugen.

sstm8: does not account for different cpu models.
base address for UART1 is 0x5240, not 0x5230
TX and RX interrupt vectors 0x804C and 0x8050.


Compilieren: braucht libboost-graph:
libboost-graph1.54-dev - generic graph components and algorithms in C++  
libboost-graph1.54.0 - generic graph components and algorithms in C++  
libboost-graph1.55-dev - generic graph components and algorithms in C++  
libboost-graph1.55.0 - generic graph components and algorithms in C++  

Es fehlen selbst elementare peephole-Optimierungen:
aufeinander folgende addw x,# und subw x,# werden nicht zusammengefasst
Multiplikation mit zwei wird nicht durch bitshift ersetzt (besonders beim
Arrayzugriff absurd)

Fehlende Features:
  - _ _preserves_regs() function attribute not supported
  - _ _attribute_ _((weak))
  - _ _critical{} generates sim/rim instead of push cc,sim/pop cc
  - dead code elimination: Does not recognize tables of const values. Using a
  const table would still pull in the whole object file, even when all
  accesses to the table have been eleminated by the optimizer. Only way out
  is to use `#define` statements instead.


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



## Possible logical pin number mappings

Many Arduino sketches and libraries contain hard-coded assumptions about the
number of pins with special functions. Ideally, all these numbers would be
the same and all programs could be compiled without changes. This is not
possible, but let's check how close we could get.

a) Matching the communication pins:

|STM8 pin|Name	|Alt	|Arduino pin	|ATmega pin	|Alt
| ----- | ----- | ----- | ----: 	| ----- 	| ----
|PD6	|RX	|Ain6	|0		|PD0		|
|PD5	|TX	|Ain5	|1		|PD1		|
|PA3	|SS	|	|10		|PB2		|PWM
|PC6	|MOSI	|	|11		|PB3		|PWM
|PC7	|MISO	|	|12		|PB4		|
|PC5	|SCK	|	|13		|PB5		|LED
|PB5	|SDA	|LED	|18		|PC4		|Ain4
|PB4	|SCL	|	|19		|PC5		|Ain5


b) Matching the analog inputs:

|STM8 pin|Name	|Alt	|Arduino pin	|ATmega pin	|Alt
| ----	| ----	| :---:	| ----		| ----		| ----
|C4	|Ain2	|	|		|		|
|D2	|Ain3	|	|		|		|
|D3	|Ain4	|	|		|		|
|D5	|Ain5	|TX	|		|		|
|D6	|Ain6	|RX	|		|		|


c) Matching the PWM-capable pins:

|STM8 pin|Name	|Alt	|Arduino pin	|ATmega pin	|Alt
| ----	| ----	| :---:	| ----		| ----		| ----
|	|	|	|3		|		|
|	|	|	|5		|		|
|	|	|	|6		|		|
|	|	|	|9		|		|
|(	|	|	|10		|)		|
|(	|	|	|11		|)		|


d) Matching the LED: (collision)

|STM8 pin|Name	|Alt	|Arduino pin	|ATmega pin	|Alt
| ----	| ----	| :---:	| ----		| ----		| ----
|PB5	|SDA	|	|13		|SCK		|


e) Simple geometric numbering for SO20 package (count up from 1, starting at pin 1):

	 1-3  -> PD4-PD6
	 4-6  -> PA1-PA3
	 7-8  -> PB5-PB4 (reverse order)
	 9-13 -> PC3-PC7
	14-16 -> PD1-PD3

SPI: 6,11,12,13 (same numbers as Arduino, but with different meanings ->
error prone)  
I2C: 7,8  
serial: 2,3  
Analog: 2,3,10,15,16 (data sheet order would be: 10,15,16,2,3)  

 + Easy and logical for use on a breadboard
 + Logical port pin ordering
 - Analog pins are scattered
 - All functions use totally different pin numbers than Arduino

f) Simple geometric numbering for square UFQFPN20 package (count up from 0, starting at pin 5/PA1):


	 0-2  -> PA1-PA3
	 3-4  -> PB5-PB4 (reverse order)
	 5-9 -> PC3-PC7
	10-15 -> PD1-PD6

serial: 14,15  
SPI: 2,7,8,9  
I2C: 3,4  
Analog: 6,11,12,14,15 (for an easier structure maybe use non-continous
numbers for the Arduino-like Ax-numbers: A0, A1, A2, A4, A5)  
PWM: 2,5-9,11-13 (all except 0,1,3,4,10,14-15)  
PWM Bitmap pin 15-0: 0011 1011 1110 0100 = 0x3be4  

 + Easy and logical for use on a breadboard
 + Very clear and logical port pin ordering
 - Analog pins are still scattered around
 + TX and RX would be the rarely used analog pin numbers A3/A4 or A4/A5 at
   the end of the analog pin number list
 + At least the analog pins are in data sheet order
 - All functions use totally different pin numbers than Arduino


Comparing the results: logical/functional mapping vs. simple geometrical numbering

|Phys. STM8 pin|Name	|Functions	|Functional mapping|Geometrical| mapping
|---:	|---	|---			|---	|---	|---
|	|	|			|	|strict	|from PA1
|1	|PD4	|UART_CLK/T2-1/beep	| 5	|1~	|13~
|2	|PD5	|TX/Ain5		|	|1	|2	14/A3
|3	|PD6	|RX/Ain6		|	|0	|3	15/A4
|5	|PA1	|(OscIn, kein HS)	| 6	|4	|0
|6	|PA2	|(OscIn, kein HS)	| 7	|5	|1
|10	|PA3	|SS/T2-3		|	|10	|6~	2~
|11	|PB5	|SDA	LED		|18	|7	|3
|12	|PB4	|SCL			|19	|8	|4
|13	|PC3	|T1-3/[T1-n1]		| 9	|9~	|5~(n~)
|14	|PC4	|T1-4/Ain2/[T1-n2]	|4	|10~	|6~(n~)/A0
|15	|PC5	|SCK/[T2-1]		|13	|11~	|7(~)
|16	|PC6	|MOSI/[T1-1]		|11	|12~	|8(~)
|17	|PC7	|MISO/[T1-2]		|12	|13~	|9(~)
|18	|PD1	|(SWIM)			| 8	|14	|10
|19	|PD2	|Ain3/[T2-3]		|3	|15(~)	|11(~~)/A1
|20	|PD3	|Ain4/T2-2		|2	|16~	|12~/A2

Functional pin mapping:

TX/RX,SPI,I2C match the Arduino numbers

Analog mapped to D0-D4 (instead of D14-D19),

PWM 2,3,4,5,9,10,11,12,13 (Arduino PWM: 3,5,6,9,10,11, all matched except
for pin 6)

non-existant: 14-17 -> it might be better to map I2C to 14 and 15.


Strict geometrical pin mapping:

SPI: 6,11,12,13 (same numbers as Arduino, but with different meanings ->
error prone)  
I2C: 7,8  
serial: 2,3  
analog: 2,3,10,15,16  
PWM regular: 2,12,13  
PWM alternate: 7,8,9  
PWM alternate negative: 5,6  
PWM alternate (duplicates): 11  


### Pin remapping

Alternate function remapping register (AFR), EEPROM 0x4803
(OPT2) und 0x4804 (NOPT2, invertiert).  Programmierbar per SWIM (UM0470) und
im IAP-Mode (PM0051)


|Bit	|Pin influenced	|Function for 0	|Function for 1
|----	|----		|----		|----
|AFR7	|PC3,PC4	|default	|TIM1_CH1N, TIM1_CH2N
|AFR4	|PB4,PB5	|default	|ADC_ETR, TIM1_BKIN
|AFR3	|PC3		|default	|TLI
|AFR1	|PA3,PD2	|default	|SPI_NSS, TIM2_CH3
|AFR0	|PC5-PC7	|GPIO/SPI	|TIM2_CH1, TIM1_CH1, TIM1_CH2




## Notes for the Arduino port

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
complexity for the Arduino API. Not sure if it should stay.


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

