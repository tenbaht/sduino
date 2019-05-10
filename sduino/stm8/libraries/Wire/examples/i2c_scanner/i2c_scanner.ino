 // --------------------------------------
// i2c_scanner
// https://playground.arduino.cc/Main/I2cScanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
// Version 6c, November 30, 2018
//    Modified to plain C for use with SDuino by Michael Mayer
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>


void setup()
{
  Wire_begin();

  Serial_begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial_println_s("\nI2C Scanner");
}


void loop()
{
  byte error, address;
  int nDevices;

  Serial_println_s("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write_endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire_beginTransmission(address);
    error = Wire_endTransmission();

    if (error == 0)
    {
      Serial_print_s("I2C device found at address 0x");
      if (address<16) 
        Serial_print_s("0");
      Serial_print_ub(address,HEX);
      Serial_println_s("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial_print_s("Unknown error at address 0x");
      if (address<16) 
        Serial_print_s("0");
      Serial_println_ub(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial_println_s("No I2C devices found\n");
  else
    Serial_println_s("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
