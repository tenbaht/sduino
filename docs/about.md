# About Sduino

The idea is to smoothen the path towards your first successful STM8-based
project.

The Arduino project was very successful in offering a simplified API hiding
most of the complexity of embedded system programming while still allowing
for advanced programming technics.

The SPL (standard peripheral library) [offered by
ST](http://www.st.com/en/embedded-software/stsw-stm8069.html) is very
powerful and provides an relativly easy migration path towards the STM32 CPU
series in case a project outgrows the capabilities of the 8-bit STM8 series.
But using that library is not very intuitive and still requires a fairly
detailed knowledge of the CPU internals. [compiling the SPL with
SDCC](developer/spl.md)


### Tools used

This project is based on free tools that are available for Linux, MacOS, and
Windows.
It uses the small devices C compiler (SDCC) for compiling,
[stm8flash](https://github.com/vdudouyt/stm8flash) for uploading the binary
to the CPU, and simple Makefiles for the build process.

Support for the Cosmic compiler under Windows and integration into the ST
visual developer IDE might be possible, but is not done (yet?).

The build process is controlled by a makefile based on the amazing
[Arduino.mk makefile](https://github.com/sudar/Arduino-Makefile) by
[Sudar](http://sudarmuthu.com>).




### Project origin

For a customer I needed to port an existing project from an ATmega to a
better suited (read: cheaper) platform. As the project was based on some
Arduino libraries, porting parts of the Arduino environment was the logical
first step. After doing that porting the firmware was finished in a couple
of days. Porting some of the more commonly used Arduino libraries was a fun
project done in my spare time later.


### About me

I am a freelance programmer for embedded systems and electronics developer
based in Heidelberg, Germany. I work with embedded systems for more than
25 years now.


#### Experiences

**current CPU architectures**: AVR (ATmega, ATxmega), PIC, STM8, STM32, MCS51

**classic CPU architectures**: 6502, 68HC11, 6809, Z80, MCS48

**electronics development**: optical sensors, laser driver, sensor
electronics, measurement equipment, industrial control, SMPS, displays
(LCD, TFT)
