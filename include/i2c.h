#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

void init_i2c(void);

bool i2c_write(uint8_t addr, uint8_t *data, uint8_t len);

bool i2c_read(uint8_t addr, uint8_t *data, uint8_t len);

#endif // I2C_H