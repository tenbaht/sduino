# Sduino

_**Porting the most important features of the Arduino API to the STM8S.**_

Within a few minutes you are ready to compile and upload your first
STM8S-based project while still retaining the flexibility to use ST's SPL
functions.

All you need to get started is a simple STM8S103F breakout board for 70
cents and a ST-Link V2 compatible flash programmer for $2.50. Three boards
and one flash programmer together are available for well under five dollars
including shipping on
[aliexpress](https://www.aliexpress.com/wholesale?SearchText=stm8s103f3p6).

*Amazing!*


### Usage

The recommanded way of installation is now the Arduino Board Manager:

- Open the Settings tab of the Preferences dialog
- Enter
  `https://github.com/tenbaht/sduino/raw/master/package_sduino_stm8_index.json`
  as an Additional Boards Manager URL
- Select *sduino* in the Boards Manager list (Tools->Board:...->Boards Manager)
- Click on Install

Wait for the download to finsh and you are ready to blink:

- Open the Boards list at Tools->Board:...
- You should find a new entry "STM8S Boards".
- Choose *STM8S103F3 Breakout Board* from the list
- Open the standard Blink example from File->Examples->01. Basics->Blink
- Compile it by hitting 'Verify'

Easy, isn't it?

In order to upload the compiled sketch to a connected board you need to
[install your flash tool](hardware/flashtool/#installation-for-windows).




### Included libraries

Some Arduino libraries are already ported to C-syntax. The resulting API is
still very close to the C++ version and porting an existing application is
not hard. Check out the [API migration guidelines](api/migration.md) for
details.

#### Communication

* [SPI](api/SPI.md): Real hardware-SPI up to 10MHz.
* [I2C](api/I2C.md): Port of the I2C master library by Wayne Truchsess
* [HardwareSerial](api/HardwareSerial.md): The standard serial interface.

#### Displays

* [LiquidCrystal](api/LiquidCrystal.md): HD44780 based text LCDs
* [PCD8544](api/PCD8544.md): Monochrome graphical LCD based on the PCD8544
  controller like the Nokia 5110 display. SPI mode only.
* [Mini_SSD1306](api/Mini_SSD1306.md): SSD1306-based monochrome OLED displays
  with 128x64 pixels. I2C support only.

#### Motor control

* [Stepper](api/Stepper.md): Stepper motors with 2, 4 or 5 phases.
* [Servo](api/Servo.md): Up to 12 servos using only 1 timer.


### Supported hardware

* The [one-dollar-boards](hardware/stm8blue.md): A simple STM8S103 breakout
  board build around a CPU STM8S103F3P6. It costs less than a dollar. The
  CPU features a 16MHz internal oscillator, 8kB flash, 1kB RAM, 640 byte
  EEPROM. It includes an UART, SPI, I2C, PWM, 10 bit ADC, 3 timer, and up to
  14 I/O pins - quite similar to an Atmel ATmega8.

* The [ESP14 Wifi-boards](hardware/esp14.md) are very similar. They are
  basically a variant of these boards with an added ESP-01 Wifi-module. Almost
  all programs should run on those chinese Wifi-enabled gems as well.

* The [STM8S105Discovery-boards](hardware/stm8sdiscovery.md) are very similar
  to an Arduino Uno with an ATmega328 CPU. The support for the used STM8S105
  CPU is still quite fresh but it should work now.

* The [sduino Uno](https://github.com/roybaer/sduino_uno) is similar to the
  STM8S105Discovery board, but build to the same form factor as the Arduino
  Uno and using a very similar pin mapping. These boards are not (yet?)
  commercially available, but all the design files are free to use.

* China-gagets based on the [STM8S003](hardware/stm8s003.md) like the [ESP14
  Wifi-board](hardware/esp14.md), the [W1209 thermostat
  controller](hardware/w1209-thermostat.md), the HC12 wireless transceiver
  module and many others.


### Limitations

SDCC doesn't support C++. Some preprocessor magic is applied to close the
gap between C and C++ syntax as much as possible, but this is not a 100%
compatible drop-in replacement for full Arduino environments like for AVR or
STM32. See [Limitations](usage/limitations.md) and the
[migration guidelines](api/migration.md) for details.


### Supported Systems

* Linux 64 bit: Tested on Ubuntu 18.04, 16.04
* Linux 32 bit: Tested on Ubuntu 18.04, 16.04
* Windows: Tested on Windows 7. Sduino might work on XP (not tested), but
  the ST-Link/V2 driver is not available anymore for XP.
* MacOS: Tested on 10.12 (Sierra)
* Raspian/Raspberry Pi: Untested. Will require a manual install.
