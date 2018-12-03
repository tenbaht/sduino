/*
 * EEPROM Clear
 *
 * Sets all of the bytes of the EEPROM to 0.
 * Please see eeprom_iteration for a more in depth
 * look at how to traverse the EEPROM.
 *
 * Modified to C for use with SDuino by Michael Mayer 2018
 *
 * This example code is in the public domain.
 */

#include <EEPROM.h>

void setup() {
  // initialize the LED pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  /***
    Iterate through each byte of the EEPROM storage.

    Larger STM8 processors have larger EEPROM sizes, E.g:
    - STM8S003:		128B EEPROM storage.
    - STM8S103:		640B EEPROM storage.
    - STM8S105:		1kB EEPROM storage.
    - STM8S208S:	1.5kB EEPROM storage.
    - STM8S208R:	2kB EEPROM storage.

    Rather than hard-coding the length, you should use the pre-provided length function.
    This will make your code portable to all STM8 processors.
  ***/

  for (int i = 0 ; i < EEPROM_length() ; i++) {
    EEPROM_write(i, 0);
  }

  // turn the LED on when we're done
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  /** Empty loop. **/
}
