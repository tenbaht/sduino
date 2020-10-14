# Test the optimized versions of pinMode

After each test the content of the GPIO registers is checked against
the expected values. The table results[] contains the number of mismatched
bytes after each test.

Expected result: all bytes in results[] == 0x00


## Usage

The compiled functions are executed in the simulator:

	$ make sim
	uCsim 0.6-pre54, Copyright (C) 1997 Daniel Drotos.
	uCsim comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
	Loading from pinmode.ihx
	920 words read from pinmode.ihx
	fill ram_chip 0 1023 0
	fill io_chip 0 44 0
	s 10000
	du io_chip  0 4
	du io_chip  5 9
	du io_chip 10 14
	du io_chip 15 19
	du ram_chip 1 6
	du ram_chip 0x0100
	quit
	Stop at 0x00838a: (109)
	V-IHINZC  Flags= 0x22  34 "  A= 0x00   0 .
	0-100010  X= 0x0006   6 .    Y= 0x5013 20499 .
	SP= 0x17fd [SP+1]= 00   0 .
	 ? 0x0838a 20 fe          jra   0x838a
	F 0x00838a
	Simulated 19210 ticks in 0.003475 sec, rate=0.690970
	0x000 00 00 00 00 00 .....
	0x005 00 00 20 20 00 ..  .
	0x00a 00 00 20 10 00 .. ..
	0x00f 00 00 42 02 42 ..B.B
	0x0001 00 00 00 00 00 00 ......
	0x0100 00 00 00 00 00 00 00 00 ........
	0x0108 00 00 00 00 00 00 00 00 ........
	0x0110 00 00 00 00 00 00 00 00 ........
	0x0118 00 00 00 00 00 00 00 00 ........
	0x0120 00 00 00 00 00 00 00 00 ........
	0x0128 00 00 00 00 00 00 00 00 ........
	0x0130 00 00 00 00 00 00 00 00 ........
	0x0138 00 00 00 00 00 00 00 00 ........
	0x0140 00 00 00 00 00 00 00 00 ........
	0x0148 00 00 00 00 00 00 00 00 ........
	$


## Meaning of the output

The first block of data is the content of the GPIO registers:

	0x000 00 00 00 00 00 .....
	0x005 00 00 20 20 00 ..  .
	0x00a 00 00 20 10 00 .. ..
	0x00f 00 00 42 02 42 ..B.B

The next line contains the result bytes, one for each test step. Zero means
no differences to the expected result:

	0x0001 00 00 00 00 00 00 ......

The last block is a simple memory dump. It might contain additional
information (but not used here and always zero):

	0x0100 00 00 00 00 00 00 00 00 ........
	0x0108 00 00 00 00 00 00 00 00 ........
	0x0110 00 00 00 00 00 00 00 00 ........
	0x0118 00 00 00 00 00 00 00 00 ........
	0x0120 00 00 00 00 00 00 00 00 ........
	0x0128 00 00 00 00 00 00 00 00 ........
	0x0130 00 00 00 00 00 00 00 00 ........
	0x0138 00 00 00 00 00 00 00 00 ........
	0x0140 00 00 00 00 00 00 00 00 ........
	0x0148 00 00 00 00 00 00 00 00 ........




## Results

original version (276 bytes)
      0080A0                         47 _pinMode1:
      0081B4                        268 _setup:
      0081BE                        282 _loop:
after first commit (176 bytes):
      0080A0                         48 _pinMode1:
      0081B4                        269 _pinMode_asm:
      008264                        410 _setup:
      00826E                        424 _loop:

Reihenfolgen:
INPUT		|-CR2, -CR1, -DDR
INPUT_PULLUP	|-CR2, -DDR, +CR1
OUTPUT_FAST	|+CR1, +DDR, +CR2
OUTPUT_OD_FAST	|-CR1, +DDR, +CR2
OUTPUT_OD	|-CR1, -CR2, +DDR
OUTPUT		|+CR1, -CR2, +DDR

mode	CR2


sstm8 -tS003 -S uart=1,port=10000 mini.ihx


CR1: pullup bzw. open-drain

CR2: in: INT, out: fast
- input: als erstes reset
- output: als letztes setzen (nach DDR)


Schritt 1: CR2 (in: INT, out: fast)
- set: OUTPUT_FAST, OUTPUT_OD_FAST
- reset: sonst


## Simulator

-i m: info memory
