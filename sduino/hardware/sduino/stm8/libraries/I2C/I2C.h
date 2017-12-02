
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef I2C_h
#define I2C_h

#define SLA_W(address) (address << 1)
#define SLA_R(address) ((address << 1) + 0x01)

void I2C_begin();
void I2C_end();
uint8_t I2C_write(uint8_t, uint8_t);
uint8_t I2C_write_c(uint8_t, uint8_t, uint8_t);
uint8_t I2C_write_s(uint8_t, uint8_t, char *);
uint8_t I2C_write_sn(uint8_t, uint8_t, uint8_t *, uint8_t);
uint8_t I2C_receive(void);
uint8_t I2C_available();
uint8_t I2C_read(uint8_t, uint8_t, uint8_t);
uint8_t I2C_read_buffer(uint8_t, uint8_t*, uint8_t);
// uint8_t I2C_scan(uint8_t);
#endif
