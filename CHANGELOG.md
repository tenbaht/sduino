# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/) 
and this project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased]
### Added
- a simple regression test script test/make-test.sh to compile all examples

### Changed
- using busybox as command shell for windows to run the wrapper scripts

### Fixed
- building the core library when using make (regression bug in 0.4.0)


## [0.4.0 - 2018-12-07]
### Added
- using stm8gal to upload using the builtin bootloader of larger STM8 devices
- Support for STM8S105K4 breakout board (stm8sblack)
- Support for Sduino-UNO board (s8uno)
- Support for Sduino MB board (mb208)
- Unlocking a MCU by using Tools->Burn Bootloader in the Arduino IDE
- new library LiquidCrystal_I2C for text LCD with I2C converter backpack
- new library LiquidCrystal_pcf2119 for I2C text LCD based on the PCF2119
- new library Wire for I2C communication
- new library EEPROM

### Changed
- completed I2C library, master transmit and master receive modes work
- updated stm8flash to support more STM8 devices and the ST-Link/V2.1
  adapter found on STM8 Nucleo boards (and espstlink for Linux
  and OSX)
- updated SDCC to version 3.8.1
- Arduino.h is automatically included now, no need for #include "Arduino.h"
  anymore

### Fixed
- Serial_serial_print_sn()
- object dependencies are calculated correctly for *.obj suffix
- Include path for sduino.mk in all example Makefiles



## [0.3.3 - 2017-11-14]
### Added
- IDE installation for OSX works now (#29, manual installation still
  problematic)

### Fixed
- Windows: better path handling to fix "'cp' and 'rm' not found" error
- removed unneed symlink in SPL that caused the IDE installation to fail (#30)



## [0.3.2 - 2017-11-08]
### Added
- make the Sduino versions of the built-in examples visible in the IDE
- support for compiling .pde and .ino files using the makefile

### Fixed
- makefile Arduino.mk uses the same tools path then the IDE
- use more scripts to generate cleaner release archive files



## [0.3.1 - 2017-11-06]
### Fixed
- don't use symlinks in archives for Windows anymore (#25)
- improved path quoting in the wrapper scripts for windows (#24)
- allow the IDE to stop a failed build process by propagating compiler error
  codes
- fix "command not found" error for cp and rm on Windows



## [0.3.0 - 2017-10-23]
### Added
- integration into the Arduino IDE
- support for easy installing via the Arduino Board Manger
- support for serialEvent()

### Changed
- adopted file layout to allow for IDE integration
- using mkdocs for building the project website

### Fixed
- changed method names for printing floats to xxx_print_f and xxx_print_fd



## [0.2.0 - 2017-03-01]
### Added
- example code for SR-HC04 ultrasonic range finder module
- adopted more examples from arduino-1.8
- support for STM8S-Discovery based on STM8S105
- Serial now reports its initialization state on call of (Serial)
- methods println_ub and println_ib for easy printing of numerical values
  in non-decimal manner
- the value of build.mcu in boards.txt is passed as a define to the compiler
- activate float support with Print
- added a changelog

### Changed
- cleaner way to map the digital pins numbers to analog channel numbers
- cleaner way to define which timers are used for PWM output in pins_arduino.h
- more flexible build script for SPL to support other CPUs
- updated the documentation

### Fixed
- corrected duplicate mapping of PWM-T21 for STM8S003/STM8S103



## [0.1.0 - 2016-02-20]
### Added
- adopted more examples from arduino-1.8
- installation instructions for Windows
- drawing to illustrate the chosen CPU pin mapping for STM8S103 breakout board
- drawing to illustrate the chosen CPU pin mapping for ESP-14 module
- added library Stepper
- added library Servo
- added library LiquidCrystal
- added library mini_SSD1306
- added library PCD8544

### Changed
- updated the documentation
- use git blob files instead of a real website
