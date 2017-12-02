
#include "I2C.h"
#include "Mini_SSD1306.h"

#define display_RESET -1

Mini_SSD1306(display, display_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Mini_SSD1306.h!");
#endif

void setup() {}

uint16_t count = 0;
uint8_t scanResult = 0; // 0 = FOUND, 1=start failed, 2=sendAddress failed
uint8_t address;

void loop() {
  I2C_begin();

  // Initialize with the I2C addr 0x3C. Some displays use 0x3D instead.
  display_begin(SSD1306_SWITCHCAPVCC, 0x3C, 0);

  delay(50);
  count++;

  display_clearPages();
  display_setCursor(0, 0);
  display_print_s("I2C Scan: ");
  display_print_u(count);

  if (count == 3) {
    // scanResult = I2C_scan(0x15);
  }

  display_setCursor(0, 1);
  display_print_ub(scanResult, 2);

  // for (address = 8; address < 124; address++) {
  //   display_setCursor(0, 0);
  //   count++;
  //   display_print_s("I2C Scan: ");
  //   display_print_u(count);

  //   scanResult = I2C_scan(address);
  //   display_print_s("->");
  //   display_print_u(scanResult);
  //   display_setCursor(0, 1);
  //   display_print_ub(address, 16);
  //   if (scanResult == 0) {
  //     display_print_s(" <- present");
  //     delay(1000);
  //   }

  //   delay(1000);
  // }
  // display_print_s("<< stop");
  
  I2C_end();
  delay(1000);
}