# LiquidCrystal_I2C Library

This library is for character LCDs based on the HD44780 controller connected
via I2C bus using the cheap I2C backpack modules based on the PCF8574(T/A).

It is derived from the
[LiquidCrystal_I2C](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
library as of 09.05.2017 ([commit
e3701fb](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library/tree/e3701fb3f2398a6d18bfd94a4a7f36e065d57d77)).
by [Frank de Brabander](https://github.com/fdebrabander).


## API

The API is very similar (but, for unknown reasons, not identical) to the API
used by the Arduino LiquidCrystal library for text LCDs with a parallel
interface.

This library is a singleton library, it is not possible to use more than one
instance per sketch.

The API syntax is very similar to the original C++ syntax, thanks to some
[c preprocessor macro magic](../developer/macro).

Apart from the usual name mangeling for polymorph functions (mostly the
different variants of the Print::print method) moving the opening bracket at
the class declarator line and replacing the dots in the method names for
underscores is all it needs.

The original version of the LiquidCystal_I2C library requires the
definition of the LCD size and the desired character size (8 or 10 pixel
height) at the instantiation uses a parameterless begin() method.

This differs slightly from the semantic of the regular Arduino
LiquidCrystal library using the parallel interface. There, the
instantiation defines only the electrical connection (the used pin
numbers) and defines the logical properties (cols, rows, charsize) later
with the begin method.

As an addition to the Arduino version of this library this port
supports both initialization styles.


Arduino syntax				|sduino syntax
--------------------			|---------------------
`LiquidCrystal_I2C lcd(i2c_addr,cols,rows,charsize)` |`LiquidCrystal_I2C (lcd,i2c_addr,rows,cols,charsize)`
`LiquidCrystal_I2C lcd(i2c_addr,cols,rows)`	|`LiquidCrystal_I2C (lcd,i2c_addr,rows)`
not allowed	|`LiquidCrystal_I2C (lcd,i2c_addr)`
`lcd.init(i2c_addr,cols,rows,charsize)`	|`lcd_init(i2c_addr,cols,rows,charsize)`
`lcd.begin()`				|`lcd_begin()`
not allowed				|`lcd_begin_wh(cols,rows)`
not allowed				|`lcd_begin_full(cols,rows,charsize)`
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
`lcd.printLeft()`			|`lcd_printLeft()`
`lcd.printRight()`			|`lcd_printRight()`
`lcd.leftToRight()`			|`lcd_leftToRight()`
`lcd.rightToLeft()`			|`lcd_rightToLeft()`
`lcd.shiftIncrement()`			|`lcd_shiftIncrement()`
`lcd.shiftDecrement()`			|`lcd_shiftDecrement()`
`lcd.noBacklight()`			|`lcd_noBacklight()`
`lcd.Backlight()`			|`lcd_Backlight()`
`result = lcd.getBacklight()`		|`result = lcd_getBacklight()`
`lcd.noAutoscroll()`			|`lcd_noAutoscroll()`
`lcd.autoscroll()`			|`lcd_autoscroll()`
`lcd.createChar(number, data[])`	|`lcd_createChar(number, data[])`
`lcd.setCursor(col,row)`		|`lcd_setCursor(col,row)`
`result = lcd.write(value)`		|`result = lcd_write(value)`
`lcd.command(value)`			|`lcd_command(value)`

The library supports the following alias definitions introduced by the
original LiquidCrystal_I2C library. The use of these alias is depreciated
and should be avoided:

Arduino syntax				|sduino syntax
--------------------			|---------------------
`lcd_blink_on()`			|`lcd_blink_on()`
`lcd_blink_off()`			|`lcd_blink_off()`
`lcd_cursor_on()`			|`lcd_cursor_on()`
`lcd_cursor_off()`			|`lcd_cursor_off()`
`lcd_setBacklight(new_val)`		|`lcd_setBacklight(new_val)`
`lcd_load_custom_character(number, data[])`|`lcd_load_custom_character(number, data[])`
`lcd_printstr(string)`			|`lcd_printstr(string)`




## Example

Output some Text and count the time since the last reset. Notice the
slightly different position of the opening parenthesis at the "class
constructor" function LiquidCrystal_I2C compared to the C++ instatiation.

```c
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// initialize the library with the I2C bus address
// The instance name "lcd" is *within* the brackets
LiquidCrystal_I2C (lcd,0x27,16,2);

void setup() {
  lcd_begin();
  lcd_print_s("hello, world!");
}


void loop() {
  lcd_setCursor(0, 1);
  lcd_print_u(millis() / 1000);
}
```


Compare this to the original Arduino C++-Sytax:

```c
#include <LiquidCrystal_I2C.h>

// initialize the library with the I2C bus address
// The instance name "lcd" is *before* the brackets
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.begin();
  lcd.print("hello, world!");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}
```


As an extention to the original `LiquidCrystal_I2C` libray this Sduino port
supports an initialisation similar to the syntax of the regular
LiquidCrystal library: Defining only the electrical parameters at
instantiation (I2C bus address) and giving the logical parameters (display
size) with the `begin()` method:

```c
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// initialize the library with the I2C bus address
// The instance name "lcd" is *within* the brackets
LiquidCrystal_I2C (lcd,0x27);

void setup() {
  lcd_begin(16,2);
  lcd_print_s("hello, world!");
}


void loop() {
  lcd_setCursor(0, 1);
  lcd_print_u(millis() / 1000);
}
```




## Possible improvements

This is not a to-do-list, just brainstorming and a collection of random
thoughts.



## Further reading

[Detailed look at the PCF8574 I2C port
 expander](https://alselectro.wordpress.com/2016/05/12/serial-lcd-i2c-module-pcf8574/)

