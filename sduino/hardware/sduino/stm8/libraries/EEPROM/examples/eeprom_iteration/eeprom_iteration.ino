/***
    eeprom_iteration example.

    A set of example snippets highlighting the
    simplest methods for traversing the EEPROM.

    Running this sketch is not necessary, this is
    simply highlighting certain programming methods.

    Written by Christopher Andrews 2015
    Modified to C for use with SDuino by Michael Mayer 2018
    Released under MIT licence.
***/

#include <EEPROM.h>

void setup() {

  // unlock the EEPROM first to enable write access
  eeprom_unlock();

  /***
    Iterate the EEPROM using a for loop.
  ***/

  for (int index = 0 ; index < EEPROM_length() ; index++) {

    //Add one to each cell in the EEPROM
    EEPROM_cell[ index ] += 1;
  }

  /***
    Iterate the EEPROM using a while loop.
  ***/

  int index = 0;

  while (index < EEPROM_length()) {

    //Add one to each cell in the EEPROM
    EEPROM_cell[ index ] += 1;
    index++;
  }

  /***
    Iterate the EEPROM using a do-while loop.
  ***/

  int idx = 0;  //Used 'idx' to avoid name conflict with 'index' above.

  do {

    //Add one to each cell in the EEPROM
    EEPROM_cell[ idx ] += 1;
    idx++;
  } while (idx < EEPROM_length());

  // re-lock the EEPROM again. Not strictly needed, but usually a good idea.
  eeprom_lock();

} //End of setup function.

void loop() {}