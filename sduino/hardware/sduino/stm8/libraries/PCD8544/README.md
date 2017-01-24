What is it?
===========

**PCD8544** is a library for the [Arduino](http://arduino.cc/) to interface with LCDs based on the
Philips PCD8544 controller ([datasheet](https://github.com/carlosefr/pcd8544/blob/docs/docs/pcd8544.pdf?raw=true))
or compatibles. These displays are commonly found on older monochrome mobile phones, such as the
[Nokia 3310](http://en.wikipedia.org/wiki/Nokia_3310) or [5110](http://en.wikipedia.org/wiki/Nokia_5110),
so if you have one of these stuck in a drawer, take it out and start hacking away! :)

This is a modified version of the
[PCD8544 library](https://github.com/carlosefr/pcd8544) written by
[Carlos Rodrigues](https://github.com/carlosefr). It is ported from C++ to C
syntax and is meant to be used with the sduino environment for the STM8.

This library is meant to have a minimal memory footprint. If you need graphics and other features and can spare the resources, check out the [library](https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library) from Adafruit.

![PCD8544.png](https://raw.githubusercontent.com/carlosefr/pcd8544/gh-pages/PCD8544.jpg)

Installation
============

Download the latest zip file from the [releases](https://github.com/carlosefr/pcd8544/releases) section. Then open it from the `Sketch > Include Library > Add .ZIP Library...` menu inside the Arduino IDE and a new "PCD8544" entry should appear in the `Sketch > Include Library` and `File > Examples` menus.

![Arduino IDE](https://raw.githubusercontent.com/carlosefr/pcd8544/gh-pages/screenshot-01.png)

How it Works
============

To use this library, you must first connect your LCD to the proper pins on the Arduino.
For a Nokia 3310 display the connections would be the following:

Display Pin       | Arduino Pin
------------------|------------
Pin 1             | +3.3V Pin
Pin 2 (SCLK)      | Digital Pin 3
Pin 3 (SDIN/MOSI) | Digital Pin 4
Pin 4 (D/C)       | Digital Pin 5
Pin 5 (SCE)       | Digital Pin 7
Pin 6             | Ground Pin
Pin 7             | 10uF capacitor to Ground Pin
Pin 8 (RST)       | Digital Pin 6

For this display model, "Pin 1" is the leftmost pin when facing the back of the display with the connector on top.

Nokia 5110 displays are slightly different. They have an external oscillator pin between pins 5 and 6 which should
be connected to +3.3V. I haven't used one of these myself, so please see the diagrams on
[this page](http://serdisplib.sourceforge.net/ser/pcd8544.html) for more details.

Since these LCDs are **3.3V** devices, you should add
[extra components](http://www.sparkfun.com/commerce/tutorial_info.php?tutorials_id=65) to
connect it to the digital pins of the Arduino (not necessary if you are using a 3.3V variant
of the Arduino, such as the [Arduino Pro](http://www.arduino.cc/en/Main/ArduinoBoardPro)). However, the I/O pins
are supposed to be 5V tolerant, so you can get by with 1K resistors in series with each pin if you like to live
dangerously.

Now, take a moment and read through the included [`HelloWorld.ino`](examples/HelloWorld/HelloWorld.ino) example.
It shows how to use the basic features of the library. There is also another
[`Thermometer.ino`](examples/Thermometer/Thermometer.ino) example that demonstrates bitmapped graphics and charts.

Custom Symbols
==============

The library allows the use of custom bitmap symbols (5x8), defined by an array of five bytes.
To make it easy to create custom symbols, there's a graphical glyph editor
[available online](http://carlosefr.github.io/pcd8544/).
