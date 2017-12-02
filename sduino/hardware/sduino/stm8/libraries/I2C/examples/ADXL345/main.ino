
#include "I2C.h"
#include "Mini_SSD1306.h"

#define display_RESET -1

Mini_SSD1306(display, display_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Mini_SSD1306.h!");
#endif

#define ADXL345 0x53

void setup() {
  I2C_begin();

  // Initialize with the I2C addr 0x3C. Some displays use 0x3D instead.
  display_begin(SSD1306_SWITCHCAPVCC, 0x3C, 0);

  delay(150);

  display_setCursor(0, 0);
  display_print_s("init");
  delay(150);

  I2C_write_c(ADXL345, 0x2D, 0x08);
  I2C_write_c(ADXL345, 0x31, 0x0B);
  I2C_write_c(ADXL345, 0x1E, 0x00);
  I2C_write_c(ADXL345, 0x1F, 0x00);
  I2C_write_c(ADXL345, 0x20, 0x00);
  delay(150);
  display_print_s(" done.");
  delay(150);

  display_clearPages();
}

void loop() {
  int16_t x, y, z;

  I2C_read(ADXL345, 0x00, 1);
  if (/* I2C_available() == 1 && */ I2C_receive() == 0b11100101) {
    I2C_read(ADXL345, 0x32, 6);
    // if (I2C_available() == 6) {
    x = I2C_receive();
    x |= ((int16_t)I2C_receive() << 8);
    y = I2C_receive();
    y |= ((int16_t)I2C_receive() << 8);
    z = I2C_receive();
    z |= ((int16_t)I2C_receive() << 8);

    display_setCursor(0, 0);
    display_print_s("X: ");
    display_print_i(x);
    display_setCursor(0, 1);
    display_print_s("Y: ");
    display_print_i(y);
    display_setCursor(0, 2);
    display_print_s("Z: ");
    display_print_i(z);

    // } else {
    //   display_print_s("ADXL345 read error");
    // }
  } else {
    display_print_s("ADXL345 misisng");
  }

  delay(1000);
}