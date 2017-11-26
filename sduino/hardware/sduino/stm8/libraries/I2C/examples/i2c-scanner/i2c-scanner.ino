
#include "I2C.h"
#include "Mini_SSD1306.h"

#define display_RESET -1

Mini_SSD1306(display, display_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Mini_SSD1306.h!");
#endif

#define BME280 0x76
#define BUFFER_SIZE 8
static const char *helloWorld = "HelloWorld!";
static const char *foundBME280 = "Found BME280";

uint8_t count = 0;

void setup() {

  I2C_begin();

  // Initialize with the I2C addr 0x3C. Some displays use 0x3D instead.
  display_begin(SSD1306_SWITCHCAPVCC, 0x3C, 0);
  // initially clear the screen
  display_clearPages();
}

void loop() {
  uint8_t buffer[BUFFER_SIZE];
  uint8_t res;

  delay(100);

  // only redraw part of the screen
  display_setCursor(0, 0);
  display_printString(helloWorld);
  // printChar is hardcoded to avoid non printable chars
  display_printChar(count + 0x20);
  count++;
  count = count > 94 ? 0 : count;

  // write to bme280 (0x76), set register to id (0xD0)
  res = I2C_read(BME280, 0xD0, 1);
  
  if (I2C_receive() == 0x60) {
    display_setCursor(0, 1);
    display_printString(foundBME280);
  }
}