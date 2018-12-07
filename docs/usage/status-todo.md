# Status and Todo

## Current status

#### tested and working

* `pinMode()`
* `digitalWrite()`
* `analogRead()`
* `delay()`
* `analogWrite()`
* `ShiftOut()`
* WMath: `map()`
* `serialEvent()`
* `pulseInLong()`
* [HardwareSerial](../api/HardwareSerial.md):
  Standard UART interface with Print functions
* [Print](../api/Print.md):
  Formatting functions to be used with the other output libraries
* [SPI](../api/SPI.md):
  working, no interrupt support  
* [LiquidCrystal](../api/LiquidCrystal.md):
  Text LCD based on the HD44780 controller  
* [PCD8544](../api/PCD8544.md):
  Nokia 5110 type displays  
* [Mini_SSD1306](../api/Mini_SSD1306.md):
  Monochrome OLED displays based on the SSD1306 controller
* [Stepper](../api/Stepper.md):
  Multi-instance design for more than one stepper at a time  
* [Servo](../api/Servo.md):
  Multi-instance design for more than one servo at a time)  

#### implemented and partly working

* [Wire/I2C](../api/I2C.md)

#### tested, but not working

* `alternateFunctions()`

#### not tested

* `ShiftIn()`
* `random()`
* `srandom()`

#### not implemented

* `tone()`
* `noTone()`
* `pulseIn()`
* handling of write error in `Print` (methods `setWriteError` etc.)
* module WCharacter
* module WString



## Todo

### Interesting libraries that might be worth porting

**[cI2C](https://github.com/SMFSW/cI2C)**: Arduino Hardware I2C for AVR
(plain c)

**[SimpleDHT](https://github.com/winlinvip/SimpleDHT)**: Simple, Stable and
Fast Arduino Temp & Humidity Sensors for DHT11 and DHT22. It started out
as plain C code (v1.0.0), but was converted to (simple) C++ with v1.0.1

**[cQueue](https://github.com/SMFSW/cQueue)**: Queue handling library
(written in plain c). But maybe too high-level to be worth using on very
small devices.

**[RTCtime](https://github.com/smz/Arduino-RTCtime)**: This library makes
using modules based on the DS1307 and DS3231 RTC really easy. It is
compatible with and is meant to leverage the standard C time library
`time.h` API (datatypes and functions) as defined in ISO/IEC 9899 (C90) and
implemented in the AVR Std C runtime libray since version 2.0.0.

**[FatFS](https://github.com/stm32duino/FatFs)**: FatFs is a generic FAT
file system module for small embedded systems. The FatFs is written in
compliance with ANSI C and completely separated from the disk I/O layer.
Therefore it is independent of hardware architecture.
stm32duino port of [FatFS by
elm-chan](http://elm-chan.org/fsw/ff/00index_e.html). A stripped down
version exists as [Petit FAT File System
Module](http://elm-chan.org/fsw/ff/00index_p.html) for very small CPU. Only
2..4kB flash usage.


### Unresolved problems

The compile environment needs to detect which interrupts are actively used
and link only the needed ones into the binary. See test/digitalWrite:
Compiling with the straight Makefile.classic does not add UART interrupt
routines. But when using the sduino.mk Makefile the two UART interrupt
routines are pulled into the binary by the interrupt table in main.c.

