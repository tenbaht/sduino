# Pin mapping scheme

Many Arduino sketches and libraries contain hard-coded assumptions about the
number of pins with special functions. Ideally, all these numbers would be
the same and all programs could be compiled without changes. This is not
possible, but let's check how close we could get.



## Possible logical pin number mappings

**Functional mapping** would try to match the pins with special functions
(like serial, SPI, I2C, analog input and PWM output) as closely as possible.

**Geometrical mapping** would choose a logical order of the CPU pins as they
are accessible on the CPU or with a breakout board.

Functional mapping would allow for designing a PCB using the Arduino Uno
form factor and take advantage of all the existing shields (as long as they
are 3.3V compatible). The number of needed changes to existing sketches
would be cut down to a minimum. The downside would be a pretty random order
of pin numbers when using a simple breakout board.

Geometrical mapping is easier for breadboard use, but will always require
changes on existing sketches. Since we need to modify them from C++ to C
syntax anyway that is maybe a less severe problem as it sounds.



### a) Matching the communication pins

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


### b) Matching the analog inputs

|STM8 pin|Name	|Alt	|Arduino pin	|ATmega pin	|Alt
| ----	| ----	| :---:	| ----		| ----		| ----
|C4	|Ain2	|	|		|		|
|D2	|Ain3	|	|		|		|
|D3	|Ain4	|	|		|		|
|D5	|Ain5	|TX	|		|		|
|D6	|Ain6	|RX	|		|		|


### c) Matching the PWM-capable pins

|STM8 pin|Name	|Alt	|Arduino pin	|ATmega pin	|Alt
| ----	| ----	| :---:	| ----		| ----		| ----
|	|	|	|3		|		|
|	|	|	|5		|		|
|	|	|	|6		|		|
|	|	|	|9		|		|
|(	|	|	|10		|)		|
|(	|	|	|11		|)		|


### d) Matching the LED: (collision)

|STM8 pin|Name	|Alt	|Arduino pin	|ATmega pin	|Alt
| ----	| ----	| :---:	| ----		| ----		| ----
|PB5	|SDA	|	|13		|SCK		|


### e) Simple geometric numbering for SO20 package (count up from 1, starting at pin 1)

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


### f) Simple geometric numbering for square UFQFPN20 package (count up from 0, starting at pin 2/PA1)


	 0-2  -> PA1-PA3
	 3-4  -> PB5-PB4 (reverse order)
	 5-9 -> PC3-PC7
	10-15 -> PD1-PD6

serial: 14,15  
SPI: 2,7,8,9  
I2C: 3,4  
Analog: 6,11,12,14,15 (for an easier structure maybe use non-continous
numbers for the Arduino-like Ax-numbers: A0, A1, A2, A4, A5)  
PWM: 2,5,6,12 plus either only 13 or 7-9 but not 13 (via alternate mapping)  
PWM Bitmap pin 15-0: 0011 0000 0110 0100 = 0x3064 (regular mapping)  
PWM Bitmap pin 15-0: 0001 0011 1110 0100 = 0x13e4 (alternate mapping)  

 + Easy and logical for use on a breadboard
 + Very clear and logical port pin ordering
 - Analog pins are still scattered around
 + TX and RX would be the rarely used analog pin numbers A3/A4 or A4/A5 at
   the end of the analog pin number list
 + At least the analog pins are in data sheet order
 - All functions use totally different pin numbers than Arduino



## Comparing the results

Compare logical/functional mapping vs. simple geometrical numbering

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



## Pin remapping

Some functions share the same CPU port pin. The Alternate function remapping
register (AFR) is used to choose the actual function. Most important choice
is for pin PC5-PC7: SPI (default) or PWM (alternate). Unfortunatly, this
influences PD4 as well (default PWM, alternate non-PWM).

The AFR is two EEPROM cells at 0x4803 (OPT2) and 0x4804 (NOPT2, inverted).
Programmable via SWIM (see UM0470) and in IAP-Mode (see PM0051).


|Bit	|Pin influenced	|Function for 0	|Function for 1
|----	|----		|----		|----
|AFR7	|PC3,PC4	|default	|TIM1_CH1N, TIM1_CH2N
|AFR4	|PB4,PB5	|default	|ADC_ETR, TIM1_BKIN
|AFR3	|PC3		|default	|TLI
|AFR1	|PA3,PD2	|default	|SPI_NSS, TIM2_CH3
|AFR0	|PC5-PC7	|GPIO/SPI	|TIM2_CH1, TIM1_CH1, TIM1_CH2

