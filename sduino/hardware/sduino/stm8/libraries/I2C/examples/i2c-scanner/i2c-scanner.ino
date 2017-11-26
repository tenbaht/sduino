
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

// start at first printable char 0x20, see FONT_5x7.h
uint8_t count = 0x20;

void setup() {
  I2C_begin();

  // The Nokia 5110 display generates the high voltage from the 3.3v
  // line internally! (neat!)
  // Initialize with the I2C addr 0x3C. Some displays use 0x3D instead.
  display_begin(SSD1306_SWITCHCAPVCC, 0x3C, 0);
  // initially clear the screen
  display_clearPages();
}

void loop() {
  uint8_t buffer[BUFFER_SIZE];
  uint8_t res;

  delay(1000);
  // only redraw part of the screen
  display_setCursor(0, 0);
  display_printString(helloWorld);
  // printChar is hardcoded to avoid non printable chars
  display_printChar(count);
  count++;

  // write to bme280 (0x76), set register to id (0xD0)
  I2C_write(BME280, 0xD0);
  res = I2C_read_buffer(BME280, &buffer[0], 1);

  if (buffer[0] == 0x60) {
    display_setCursor(0, 1);
    display_printString(foundBME280);

    display_setCursor(0, 2);
    I2C_write(BME280, 0xF7); // start of data and burst
    res = I2C_read_buffer(BME280, &buffer[0], BUFFER_SIZE);
    display_sendByte(res); // debug
    display_sendByte(res); // debug
    display_sendByte(res); // debug

    display_sendByte(0x00); // debug
    display_sendByte(0x00); // debug
    display_sendByte(0x00); // debug

    for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
      display_sendByte(buffer[i]);
      display_sendByte(buffer[i]);
      display_sendByte(buffer[i]);
    }
  }
}