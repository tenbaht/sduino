# LiquidCrystal Library

This library is for character LCDs based on the HD44780 controller.
Derived from the Arduino LiquidCrystal library v1.8.0.



## API

This library is a singleton library, it is not possible to use more than one
instance per sketch.

The API syntax is very similar to the original C++ syntax, thanks to some
[c preprocessor macro magic](macro.html).

Apart from the usual name mangeling for polymorph functions (mostly the
different variants of the Print::print method) moving the opening bracket at
the class declarator line and replacing the dots in the method names for
underscores is all it needs.


Arduino syntax				|sduino syntax
--------------------			|---------------------
`LiquidCrystal lcd(rs,en,d0,d1,d2,d3)`			|`LiquidCrystal (lcd,rs,en,d0,d1,d2,d3)`
`LiquidCrystal lcd(rs,rw,en,d0,d1,d2,d3)`		|`LiquidCrystal (lcd,rs,rw,en,d0,d1,d2,d3)`
`LiquidCrystal lcd(rs,en,d0,d1,d2,d3,d4,d5,d6,d7)`	|`LiquidCrystal (lcd,rs,en,d0,d1,d2,d3,d4,d5,d6,d7)`
`LiquidCrystal lcd(rs,rw,en,d0,d1,d2,d3,d4,d5,d6,d7)`	|`LiquidCrystal (lcd,rs,rw,en,d0,d1,d2,d3,d4,d5,d6,d7)`
`lcd.init(rs,rw,en,d0,d1,d2,d3,d4,d5,d6,d7)`		|`lcd_init(mode,rs,rw,en,d0,d1,d2,d3,d4,d5,d6,d7)`
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
`lcd.setRowOffsets(row0,row1,row2,row3)`|`lcd_setRowOffsets(row0,row1,row2,row3)`
`lcd.createChar(number, data[])`	|`lcd_createChar(number, data[])`
`lcd.setCursor(col,row)`		|`lcd_setCursor(col,row)`
`result = lcd.write(value)`		|`result = lcd_write(value)`
`lcd.command(value)`			|`lcd_command(value)`

The LCD interface mode can be changed at run time by calling the
`lcd_init(...)` function.



## Example

Output some Text and count the time since the last reset. Notice the
slightly different position of the opening parenthesis at the "class
constructor" function LiquidCrystal compared to the C++ instatiation.

```c
#include <Arduino.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
// The instance name "lcd" is *within* the brackets
LiquidCrystal (lcd,PA1,PA2, PA3,PD2,PD3,PD4);

void setup() {
  lcd_begin(16, 2);
  lcd_print_s("hello, world!");
}


void loop() {
  lcd_setCursor(0, 1);
  lcd_print_u(millis() / 1000);
}
```


Compare it to the original Arduino C++-Sytax:
```c
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
// The instance name "lcd" is *before* the brackets
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
  lcd.print("hello, world!");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}
```



## Possible improvements

This is not a to-do-list, just brainstorming and a collection of random
thoughts.

### Direct port access
As long as the pin numberings are known at compile time it would be possible
use direct port register access and the access pattern could be optimized if
all data pins are on the same port.


### Auto-detect 4-bit mode
The parameter `fourbitmode` of the init() function/method is not needed. It
would be sufficient to check `d5` for a valid value. If it is not valid,
assume 4-bit mode.

As init() is not called very frequently the possible advantage would be small
compared to the downside of breaking the Arduino compatibility.


[Back to the main page](../index.html)
