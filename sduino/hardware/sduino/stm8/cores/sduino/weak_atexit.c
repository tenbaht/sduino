/**
 * "weak" functions to be used if no user-defined function is defined.
 *
 * This function stub is compiled into the core library. Functions from
 * libraries are only linked if a referenced function isnt't defined any other
 * object file. This way all library function are a kind of "weak" functions.
 *
 * This function is defined in a separate source file to avoid pulling in
 * other unrelated definitions.
 */

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (*func)()) { return 0; }
