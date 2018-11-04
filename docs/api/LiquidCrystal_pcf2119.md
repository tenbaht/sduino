# LiquidCrystal_pcf2119 Library

This library is for character LCDs based on the PCF2119 controller and
connected via I2C. PCF2119 based displays are pretty rare in the wild, it is
very likely that this is the wrong library for your display.

The more common case is a LCD based on the HD44780 controller and a parallel
interface connected to a small interface PCB with an I2C parallel interface
PCF8544. These two interfaces are **not compatible** with each other. That
kind of display will require the [LiquidCrystal_I2C](../LiquidCrystal_I2C)
library instead.

PCF2119 based displays seem to have been used in older fax machines and
multi function printers, but thery are hard to come by nowerdays (read:
expensive. North of 20€ at Farnell).

This Library is derived from the Arduino LiquidCrystal library v1.8.0 with
some additions from the LiquidCrystal_I2C library.



## API

This library is a singleton library, it is not possible to use more than one
instance per sketch.

This Library is an SDuino original: There is no preexisting Arduino library
for this controller. The API syntax is designed to be very similar to the
regular [LiquidCrystal library](../LiquidCrystal) for LCDs with a parallel
interface. The API syntax is very similar to the original C++ syntax, thanks
to some [c preprocessor macro magic](../../developer/macro).

Apart from the usual name mangeling for polymorph functions (mostly the
different variants of the Print::print method) moving the opening bracket at
the class declarator line and replacing the dots in the method names for
underscores is all it needs.


(hypotetical) Arduino syntax		|sduino syntax
--------------------			|---------------------
`LiquidCrystal lcd(adr)`		|`LiquidCrystal (adr)`
`LiquidCrystal lcd(adr,reset)`		|`LiquidCrystal (adr,reset)`
`LiquidCrystal lcd(adr,reset,charset)`	|`LiquidCrystal (adr,reset,charset)`
`lcd.init(adr,reset,charset)`		|`lcd_init(adr,reset,charset)`
`lcd.begin(cols,lines)`			|`lcd_begin(cols,lines)`
`lcd.begin(cols,lines,charsize)`	|`lcd_begin_charsize(cols,lines,charsize)`
`lcd.clear()`				|`lcd_clear()`
`lcd.home()`				|`lcd_home()`
`lcd.noDisplay()`			|`lcd_noDisplay()`
`lcd.display()`				|`lcd_display()`
`lcd.noBlink()`				|`lcd_noBlink()`
`lcd.blink()`				|`lcd_blink()`
`lcd.noCursor()`			|`lcd_noCursor()`
`lcd.cursor()`				|`lcd_cursor()`
`lcd.scrollDisplayLeft()`		|`lcd_scrollDisplayLeft()`
`lcd.scrollDisplayRight()`		|`lcd_scrollDisplayRight()`
`lcd.leftToRight()`			|`lcd_leftToRight()`
`lcd.rightToLeft()`			|`lcd_rightToLeft()`
`lcd.noAutoscroll()`			|`lcd_noAutoscroll()`
`lcd.autoscroll()`			|`lcd_autoscroll()`
`lcd.createChar(number, data[])`	|`lcd_createChar(number, data[])`
`lcd.setCursor(col,row)`		|`lcd_setCursor(col,row)`
`result = lcd.write(value)`		|`result = lcd_write(value)`
`result = lcd.data(value)`		|`result = lcd_data(value)`
`lcd.command(value)`			|`lcd_command(value)`



## Example

Output some Text and count the time since the last reset. Notice the
slightly different position of the opening parenthesis at the "class
constructor" function LiquidCrystal compared to the C++ instatiation.

```c
#include <Arduino.h>
#include <LiquidCrystal_pcf2119.h>

// initialize the library with the number of the reset pin
// The instance name "lcd" is *within* the brackets
LiquidCrystal_pcf2119 (lcd,PA3);

void setup() {
  lcd_begin(16, 2);
  lcd_print_s("hello, world!");
}


void loop() {
  lcd_setCursor(0, 1);
  lcd_print_u(millis() / 1000);
}
```


Compare it to the (hypotetical) Arduino C++-Sytax:
```c
#include <LiquidCrystal_pcf2119.h>

// initialize the library with the number of the reset pin
// The instance name "lcd" is *before* the brackets
LiquidCrystal_pcf2119 lcd(2);

void setup() {
  lcd.begin(16, 2);
  lcd.print("hello, world!");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}
```


## Differences between this lib and LiquidCrystal_I2C

### Physical interface

The PCF2119 supports the standard command set as the HD44780, the only
visible difference is the physical interface. Supporting I2C natively means
very easy read and write access, very similar to the ease of use of a fully
connected 8 bit parallel interface without losing so many port pins.

This library can be seen as an extended version of the regular
LiquidCrystal_I2C using 8 bit mode instead of bit-banging a four bit
interface. Or as an I2C-enabled version of LiquidCrystal in 8-bit mode.




### Character mapping

This is a head-banging one. Such an engineering nonsense. The PCF2119 exists
in six different versions, supporting six different character sets named A,
D, F, I, R and S. See page 19ff in [the
datasheet](https://www.nxp.com/docs/en/data-sheet/PCF2119X.pdf).

A is a standard ASCII mapping, R is the most random one where not even space
is at the position it is supposed to be. And guess which character set is
used by the modules still available at Farnell? Right, the R set. The left
over ones no one was willing to buy so far.

This means that the build-in clear function can't be used and the
ShiftLeft/ShiftRight modes are not useful as they fill up the free space
with 0x20 - which happens to be an arrow in this character set. Who came up
with *that* idea?

For character set A, D and I all data is always transferd unmodified.

For character sets F, R and S the `write()` method transparently swaps the
ASCII ranges 0x20..0x7f and 0xa0..0xff. This allows the use of the regular
print functions without the need to think about the character encoding. The
ranges 0x00..0x1f and 0x80..0x9f are transfered unmodified to enable the use
of user defined characters.

The additional function `data()` writes data directly into the display RAM,
without any modifications and independed from the chosen character set.

The R set is the default case if no other character is specified at
instantiation time. So if you were lucky enough to lay your hands on a
display with a more useful character set, this is the way to tell the
library about your success:

```c
// Use character set A instead of the dreaded R
LiquidCrystal_pcf2119 (lcd,PA3,'A');
```



## Possible improvements

This is not a to-do-list, just brainstorming and a collection of random
thoughts.

### Unify all the LiquidCrystal libraries
It would be very slick to unify all three libraries/four interface modes
into one, similar to
[NewLiquidCrystal](https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home)
by Francisco Malpartida.
