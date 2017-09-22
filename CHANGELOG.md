# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/) 
and this project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased]
### Added
- support for serialEvent()

### Changed
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
