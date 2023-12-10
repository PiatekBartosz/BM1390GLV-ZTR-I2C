#include "BM1390GLV-ZTR.h"
#include "i2c.h"

#define BM1390GLV_ZTR_I2C_SLAVE_ADDR 0x00

void BM1390GLV_ZTR_init(void) {
  i2c_init();
  i2c_address(BM1390GLV_ZTR_I2C_SLAVE_ADDR);
}

void BM1390GLV_ZTR_cfg(void) {
  i2c_start();
  /* Write to register to set sensor configuration*/
  // i2c_write(0x00, NULL, 0);
  i2c_stop();
}

void BM1390GLV_ZTR_read(void) {
  i2c_start();
  /* Read data using I2C*/
  // i2c_read(0x00, NULL, 0);
  i2c_stop();
}