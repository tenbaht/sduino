# STM8 breakout board

Pins 1:1 vom STM8S103F3P6 auf die Pinleisten rausgeführt.
LED Test (rot) an PB5 (Pin 11)


## Compiler

Anleitungen:
http://www.cnx-software.com/2015/04/13/how-to-program-stm8s-1-board-in-linux/

STM8-Support erst ab Version 3.4 in Ubuntu 14.10. Für 14.4:

	add-apt-repository ppa:laczik/ppa
	apt-get update
	apt-get install sdcc

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



## denkbare Arduino-Zuordnung

feste Kommunikationspins:
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

analog:
STM8			Arduino
Pin	Name	Alt	Pin		Alt
C4	Ain2
D2	Ain3
D3	Ain4
(D5	Ain5	TX)
(D6	Ain6	RX)

PWM:
STM8			Arduino
Pin	Name	Alt	Pin		Alt
			3
			5
			6
			9
(			10	)
(			11	)

LED: (Kollision)
STM8			Arduino
Pin	Name	Alt	Pin		Alt
(B5	SDA		13	SCK)

