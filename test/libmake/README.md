## Defined new parameters in boards.txt

`BOARD.upload.mcu` cpu specifier for avrdude/stm8flash. Defines AVRDUDE_MCU


## Problems with SDCC and Arduino.mk

### missing/non-supported compiler flags

`-dumpversion`

`-mmcu=xxxx` wird nicht unterstützt, `-mxxx` hat kein Gleichheitszeichen

CPPFLAGS lässt sich zwar vorbelegen, dann werden die normalen Argumente aber
immer noch hinten dran gehängt.
-> irgendwie müsste sich die Standardbelegung einstellbar oder ganz abstellbar
machen.

dependencies:
Compilieren und Dependencies gleichzeitig geht mit:
`sdcc -Wp"-MMD abc" .....` (mit filename für dep.)

erwartet wird:
`sdcc -MMD .....` (ohne filename für dep.)
dieser Aufruf erzeugt aber nur die dep. in der rel-Datei und kein Object file

Der Assembler erzeugt immer .rel-Files, es werden aber .o-Files erwartet.



### general suggestions for Arduino.mk:

 - rename the programmer specific variables from AVRDUDE_xxxx to a more
   generic PROGRAMMER_xxxx
 - allow predefined AVRDUDE_xxxx definitions from the Makefile to override
   the PROGRAMMER_xxxx definitions

