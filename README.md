# sdunino

Getting started on the STM8 CPU the easy way by using an Arduino-like
programming API.

The SPL (standard peripheral library) offered by ST is very powerful and
very similar to the one used for the STM32 CPU series offering an relatively
easy upgrade path in case a project outgrows the capabilities of the 8 bit
STM8 series. But using that library is not very intuitive and still
requires a fairly detailed knowledge of the CPU internals.

The Arduino project was very successful in offering a simplified API hiding
most of the complexity of embedded system programming while still allowing
for advanced programming technics.

This project wants to offer the most important features of the Arduino API
for the STM8S. I needed to port an existing project from an ATmega to a
better suited (read: cheaper) platform. As the project is based on some
Arduino libraries porting parts of the Arduino environment was a logical
first step. After doing that porting the whole software was finished in a
couple of days.



## Used tools

This project is based on free tools that are available for Linux, MacOS and
Windows. It uses the small devices C compiler (SDCC) for compiling, stm8flash
for uploading the binary to the CPU and simple Makefiles for the build
process.

SDCC support for the STM8 is still quite fresh and not very mature. It
improves significantly from version to version. Make sure to use the latest
nightly build from the project site on sourceforge, not the older versions
included in your distribution. v3.5.0 as included with ubuntu 16.04 is
definitly too old and compilation will fail due to some compiler errors.

Support for the Cosmic compiler under Windows and integration into the ST
visual developer IDE might be possible, but is not done (yet?).



## Supported hardware

The simple STM8S103F breakout board as sold on aliexpress.com for well below
one dollar. Adding a ST-Link V2 compatible flash programmer and you are set
for the STM8S world for less than five dollar (3 breakout boards and one
flash tool)

*Amazing!*

The breakout boards are build around a CPU STM8S103F3P6 with 16MHz internal
oszillator, 8kB flash, 1kB RAM and 640 byte EEPROM. The CPU includes a
UART, SPI, I2C, PWM, 10 bit ADC, 3 timer and up to 14 I/O pins. So not
unalike an Atmel ATmega8.

One (red) LED is connected to GPIO PB5 (cpu pin 11). The push button is for
reset. The CPU runs on 3.3V, a linear regulator is integrated on the
board. The micro USB connector is only for (5V) power supply, the data lines
are not connected.

All CPU pins are easily accessible on (optional) pin headers
(pitch 2.54mm, perfect for breadboards). 


I am using the ST-Link V2 compatible flash tool in the green plastic
housing. The one in the metal housing uses a different pinout.

Connection to the green flashtool:

	signal	CPU	flash-
	name	board	tool
	------	------	------
	3V3	1	2
	SWIM	2	5
	GND	3	7
	NRST	4	9




## Compatibility with the Arduino world

I adopted the Arduino core functionality for the STM8S to set up a simple
programming environment. But unfortunatly there is no free C++ compiler
for these CPUs. This makes it impossible to do a full port of the whole
enviroment and integrate it with the Arduino IDE and build system as is
has been done for the STM32 and the ESP8266.

This is not a drop in replacement for an AVR, but the programming API is
still very, very similar. Adopting existing libraries from C++ to C and for
the use with the simplified C API is often easy and can be a matter of
minutes, depending on the degree of dependency on specific hardware
features.

The whole Arduino build system is deeply based on the assumption of C++
source files. I am not sure if it would be even possible to configure a
build process based only on C files. This leaves a full IDE integration
being very unlikely.

Using a converter/compiler to translate from C++ to C like cfront might be
an option.



## why use a STM8 instead of an ATmega?

The community support and the mere number of existing libraries for all kind
of sensors and hardware is outstanding in the Arduino world. If you just
want to get something done, go for an Arduino board. Nothing will show
faster and easier results.

The fairly new ESP-14 modules includes a STM8S003F3P6. Wifi and a
programmable I/O-CPU for just over two dollar - that might be the most
compelling reason to get started on the STM8S series. Apart from pure
curiosity and eagerness to learn something new, of course.

For commercial use the STM8S offers some interesting advantages:

Motor control: The STM8 has a strong focus on motor and position control
systems. Things you need to handle yourself on an ATmega are implemented in
hardware and work independently of the state of the software. There is even
hardware support for quadrature encoders as used in position sensors and
rotary encoders.

low power modes: The numbers in the datasheets don't look that different,
but in real life the STM8 can be powered two or three times longer using the
same battery capacity due to the finer control on the power modes (very,
very careful programming required)

Value for the money: 40 to 60 cents for a STM8 with 14 I/O pins compared to
1.60 to 3.00$ for an ATmega8. 



## Compiler

Tutorials:
http://www.cnx-software.com/2015/04/13/how-to-program-stm8s-1-board-in-linux/

STM8-Support erst ab Version 3.4 in Ubuntu 14.10. Für 14.4:

	add-apt-repository ppa:laczik/ppa
	apt-get update
	apt-get install sdcc

Besser einen aktuellen snapshot-build direkt von http://sdcc.sourceforge.net/
besorgen. Das braucht aber eine neue Version der libstdc++6. Deshalb:

	add-apt-repository ppa:ubuntu-toolchain-r/test
	apt-get update
	apt-get install libstdc++6

	git clone https://github.com/vdudouyt/stm8flash.git
	cd stm8flash
	make
	sudo make install

Beispiele besorgen:

	git clone https://github.com/vdudouyt/sdcc-examples-stm8.git
	cd sdcc-examples-stm8

Sind eigentlich für STM8L gedacht, da ist die Pinbelegung etwas anders und
muss entsprechend angepasst werden. Am Ende hochladen:

	stm8flash -c stlinkv2 -p stm8s103?3 -w blinky.ihx 


### mixing with assembler

c-code:
	stacktest(0x1234, 0x5678);

assember:
	push    #0x78
	push    #0x56
	push    #0x34
	push    #0x12
	call    _stacktest

resulting stack content (starting at [SP]):
	0> dch 0x17f9
	0x017f9 c0 80 ab 12 34 56 78 5b ....4Vx[

=> first paramter starts at [SP+3], HSB first.




## current status and todo list

tested and working:
pinMode()
HardwareSerial
Print (without float)
digitalWrite()
analogRead
delay

implemented and partly working:
analogWrite
SPI: usable, only interrupt support is missing

tested, but not working:
alternateFunctions()

not tested
ShiftIn()
ShiftOut()


not implemented:
yield()
Wire/I2C



## Differences to the original Arduino environment

Additinal output pin modes:
OUTPUT		outpur, push-pull, slow mode (default)
OUTPUT_OD	output, open drain, fast mode
OUTPUT_FAST	output, push-pull, fast mode
OUTPUT_OD_FAST	output, open drain, fast mode

Timer: millis() uses timer4. The prescaler and end value is calculated at
compile time for a cycle time as close to 1ms as possible. (default @16Mhz:
prescaler=64, counter cycle=250 (end value=249), resulting in exaclty 1ms
intervals.



## Further reading and application notes

STM8AF Flash programming manual (PM0051)
STM8 SWIM protocol and debug manual (UM0470)
Using the analog to digital converter of the STM8S microcontroller (AN2658)

Many examples and presentations about the STM8S:
https://github.com/VincentYChen/STM8teach
https://github.com/VincentYChen/STM8teach/tree/master/code/Project/STM8S_StdPeriph_Examples

Using the ADC:
http://blog.mark-stevens.co.uk/2012/09/single-scan-adc-on-the-stm8s/

example for RS-232 handling with SPL:
https://sourceforge.net/p/oggstreamer/oggs-stm8-firmware-001/ci/master/tree/rx_ringbuffer.c

AN3139 Migration guideline within the STM8L familiy
http://www.st.com/content/ccc/resource/technical/document/application_note/07/d7/59/69/74/8b/48/8a/CD00262293.pdf/files/CD00262293.pdf/jcr:content/translations/en.CD00262293.pdf



### Anmerkungen zu SDCC

Befehl '_ _ critical{..}' sollte eigentlich den vorherigen Interrupt-Zustand
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
  - _ _attribute_ _((weak))
  - _ _critical{} erzeugt sim/rim statt push cc,sim/pop cc
  - dead code elemination: Verbietet es, const-Tabellen anzulegen und fordert
"#define" für alles.



## ST Standard Library

Can be downloaded from ST website (Free registration required). For use with
SDCC it needs to be patched:

	git clone https://github.com/g-gabber/STM8S_StdPeriph_Driver.git
	git clone https://github.com/gicking/SPL_2.2.0_SDCC_patch.git
	cp ../STM8S_SPL_2.2.0/Libraries/STM8S_StdPeriph_Driver/inc/stm8s.h .
	patch -p1 < ../SPL_2.2.0_SDCC_patch/STM8_SPL_v2.2.0_SDCC.patch 
	 cp -av  ../STM8S_StdPeriph_Lib/Project/STM8S_StdPeriph_Template/stm8s_conf.h .
	cp -av  ../STM8S_StdPeriph_Lib/Project/STM8S_StdPeriph_Template/stm8s_it.h .

.rel-files are the object files .o

Needed additional patch for stm8s_itc.c:

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

Now the library can be compiled for the STM8S103 using this Makefile:

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

This library can now be used for linking with blink_spl or uart_spl. The
files stm8s_conf.h and stm8s_it.h are still needed for compilation.

The linker does not remove individual unused functions from an object file,
only complete object files can be skipped.
**=> for building a library it is better to separate all functions into
individual source files **

The STP library folder contains a script to do this separation before
compilation.
FIXME: description needed

Erklärung wie zumindest die Interrupt-Vektoren in die eigene Datei kommen
können:
http://richs-words.blogspot.de/2010/09/stm8s-interrupt-handling.html


### Interrupts

Namen definiert in stm8s_itc.h
Interrupt-Routine definieren:

	/* UART1 TX */
	void UART1_TX_IRQHandler(void) __interrupt(ITC_IRQ_UART1_TX)
	{
	}

Jetzt muss noch das passende IRQ-Enable-Flag gesetzt werden und Interrupt
generell freigegeben werden, also hier:

	UART1_ITConfig(UART1_IT_TXE, ENABLE);
	enableInterrupts();

Unklar ist, was die ITC-Prioritäten bewirken. Es geht jedenfalls auch ohne:

	ITC_DeInit();
	ITC_SetSoftwarePriority(ITC_IRQ_UART1_TX, ITC_PRIORITYLEVEL_2);






## Programmer

STM8 verwendet SWIM, STM32 aber SWD

STM8-Board: SWIM-Verbinder P3
1	3V3
2	SWIM (PD1)
3	GND
4	NRST

Discovery STM32F0308 als ST-Link/V2:
Belegung CN3 SWD:
1	? detect oder so?
2	JTCK/SWCLK
3	GND
4	JTMS/SWDIO
5	NRST
6	SWO (=SWIM?)

Pinbelegung der Version im grünen Plastikgehäuse:

-	+-----+
T_JRST	| 1  2|	3V3
5V	| 3  4|	T_JTCK/T_SWCLK
SWIM	  5  6|	T_JTMS/T_SWDIO
GND	| 7  8|	T_JTDO
SWIM RST| 9 10|	T_JTDI
-	+-----+

Pinbelegung der Version im Metallgehäuse

-	+-----+
RST	| 1  2|	SWDIO
GND	| 3  4|	GND
SWIM	  5  6|	SWCLK
3V3	| 7  8|	3V3
5V	| 9 10|	5V
-	+-----+

Notwendiger Eintrag in /etc/udev/rules.d/99-stlink.rules:

	# ST-Link/V2 programming adapter

	# ST-Link V1
	#SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device", 
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3744", MODE="0666", GROUP="plugdev"

	# ST-Link/V2, the china adapter with the green plastic housing
	#SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device", ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666"
	ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666", GROUP="plugdev"




## Anpassung der Beispielprogramme

blinky.c: Pinbelegung

uart.c: Pinbelegung (TX auf D5), RX ist dann D6.
Es wird mit 1200 Baud gesendet => nur 2MHz statt 16MHz

## denkbare Arduino-Zuordnung

a) feste Kommunikationspins:
STM8			Arduino
Pin	Name	Alt	Pin		Alt
D6	RX	Ain6	0	D0
D5	TX	Ain5	1	D1
A3	SS		10	B2	PWM
C6	MOSI		11	B3	PWM
C7	MISO		12	B4
C5	SCK		13	B5	LED
B5	SDA	LED	18	C4	Ain4
B4	SCL		19	C5	Ain5

b) analog:
STM8			Arduino
Pin	Name	Alt	Pin		Alt
C4	Ain2
D2	Ain3
D3	Ain4
(D5	Ain5	TX)
(D6	Ain6	RX)

c) PWM:
STM8			Arduino
Pin	Name	Alt	Pin		Alt
			3
			5
			6
			9
(			10	)
(			11	)

d) LED: (Kollision)
STM8			Arduino
Pin	Name	Alt	Pin		Alt
(B5	SDA		13	SCK)


e) direkte Durchnummerierung nach Gehäusepins: (ab 1)

 1-3  -> PD4-PD6
 4-6  -> PA1-PA3
 7-8  -> PB5-PB4 (rev.)
 9-13 -> PC3-PC7
14-16 -> PD1-PD3

SPI: 6,11,12,13 (gleiche Nummern, aber andere Reichenfolge -> fehlerträchtig)
I2C: 7,8
Seriell: 2,3
Analog: 2,3,10,15,16 (in Datenblattreihenfolge: 10,15,16,2,3)

 + gut beim Aufbau dem Steckbrett
 + logische Portzuordnung
 - analog wild verteilt
 - alle Funktionen auf total anderen Pinnummern

f) geometische Durchnummerierung, angefangen bei Pin 5/PA1 (ab 0)

 0-2  -> PA1-PA3
 3-4  -> PB5-PB4 (rev.)
 5-9 -> PC3-PC7
10-15 -> PD1-PD6

Seriell: 14,15
SPI: 2,7,8,9
I2C: 3,4
Analog: 6,11,12,14,15 (evtl. mit Lücke nummerieren: A0, A1, A2, A4, A5)
PWM: 2,5-9,11-13 (alles ausser 0,1,3,4,10,14-15)

PWM Bitmap pin 15-0: 0011 1011 1110 0100 = 0x3be4

 + auch gut beim Steckbrett
 + sehr logische Portzuordnung
 - analog noch immer verteilt
 + TX und RX sind die selten verwendeten analogen Pins A3/A4 oder A4/A5
 + analoge Pins wenigstens in Datenblattreihenfolge
 - alle Funktionen auf total anderen Pinnummern


Vergleich der Ergebnisse: logische gegenüber geometrischer Pinnummerierung

phys. STM8				nach	geometrische
Pin	Name	Funktionen		Funkt.	streng	ab PA1
1	PD4	UART_CLK/T2-1/beep	 5	1~	13~
2	PD5	TX/Ain5			1	2	14/A3
3	PD6	RX/Ain6			0	3	15/A4
5	PA1	(OscIn, kein HS)	 6	4	0
6	PA2	(OscIn, kein HS)	 7	5	1
10	PA3	SS/T2-3			10	6~	2~
11	PB5	SDA	LED		18	7	3
12	PB4	SCL			19	8	4
13	PC3	T1-3/[T1-n1]		 9	9~	5~(n~)
14	PC4	T1-4/Ain2/[T1-n2]	4	10~	6~(n~)/A0
15	PC5	SCK/[T2-1]		13	11~	7(~)
16	PC6	MOSI/[T1-1]		11	12~	8(~)
17	PC7	MISO/[T1-2]		12	13~	9(~)
18	PD1	(SWIM)			 8	14	10
19	PD2	Ain3/[T2-3]		3	15(~)	11(~~)/A1
20	PD3	Ain4/T2-2		2	16~	12~/A2

Pinmapping nach Funktion:
TX/RX,SPI,I2C wie Arduino,
Analog auf D0-D4 (statt D14-D19),
PWM 2,3,4,5,9,10,11,12,13 (Arduino PWM: 3,5,6,9,10,11, alles ausser 6 ist
abgedeckt)
nicht existent 14-17 -> evtl. besser I2C auf 14 und 15.

Pinmapping streng geometrisch:
SPI: 6,11,12,13 (gleiche Nummern, aber andere Reichenfolge -> fehlerträchtig)
I2C: 7,8
Seriell: 2,3
Analog: 2,3,10,15,16
PWM regulär: 2,12,13
PWM alternate: 7,8,9
PWM alternate negativ: 5,6
PWM alternate doppelt: 11

Pin remapping: Alternate function remapping register (AFR), EEPROM 0x4803
(OPT2) und 0x4804 (NOPT2, invertiert).  Programmierbar per SWIM (UM0470) und
im IAP-Mode (PM0051)
val	pin	0		1
AFR7	C3,C4	default		TIM1_CH1N, TIM1_CH2N
AFR4	B4,B5	default		ADC_ETR, TIM1_BKIN
AFR3	C3	default		TLI
AFR1	A3,D2	default		SPI_NSS, TIM2_CH3
AFR0	C5-C7	GPIO/SPI	TIM2_CH1, TIM1_CH1, TIM1_CH2




## Anmerkungen zur Arduino-Portierung

use of the timers:
timer1: PWM for PC6, PC7 (8,9), could be used for ADC
timer2: PWM for PA3 (2)
timer4: millis()

ADC:
the prescaler is initialised for an ADC clock in the range of 1..2 MHz. The
minimum prescaler value is 2, so for a clock speed of less than 2 MHz the
required minimum ADC clock frequency can not be reached anymore.


Die ganze Pin->Portadressen-Arithmetik könnte komlett entrümpelt werden. Statt
Tabellen fest im Code enthalten.

digitalWrite wird spektakulär umständlich übersetzt. Hier lohnt sich
Handassembler. 

added alternateFunction() to allow switching some pins to their alternate
functions. This allows for three more PWM pins, but maybe it adds to much
complexity for the Arduino API. Not sure if it should stay.


### Performance compared with the original Arduino environment

Benchmarking the original Arduino examples from Arduino 1.0.5. The simple
Blinky cmopiles to 57 bytes of code, the total binary including the sduino
libraries is 1868 Bytes (0x74c).

So far, wiring_analog depends on wiring_digital, even when analogWrite is not
used. This could be solved by compiling the sduino functions separately into
a library.

name			code	total	linked files other than main and wiring
01. Basics/
BareMinimum		2	1238	-
Blink			57	1870	wiring_digital
AnalogReadSerial	205	3452	digital, analog, serial, print
DigitalReadSerial	57	3160	digital, serial, print
Fade			226	2189	digital, analog
ReadAnalogVoltage			float not yet implemented
02. Digital/
Debounce		192	2016	digital


### Besondere Features, die von Arduino nicht unterstützt werden

Input-Capture-Mode: min. für Timer1 auf allen vier Kanälen möglich.

Encoder interface mode: Kann von Haus aus mit Quadratur-Encodern umgehen und
in Hardware zählen -> perfekt für die Druckerschlitten-Motorsteuerung.

