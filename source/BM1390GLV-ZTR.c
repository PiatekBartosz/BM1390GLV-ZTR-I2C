#include "BM1390GLV-ZTR.h"
#include "i2c.h"

#define BM1390GLV_ZTR_I2C_SLAVE_ADDR 0x5D

#define MANUFACTURER_ID_ADDR 0x0F
#define PART_ID_ADDR 0x10
#define POWER_DOWN_ADDR 0x12
#define RESET_ADDR 0x13
#define MODE_CONTROL_ADDR 0x14
#define FIFO_CONTROL_ADDR 0x15
#define FIFO_DATA_ADDR 0x18
#define STATUS_ADDR 0x19
#define PRESSURE_OUT_HIGH_ADDR 0x1A
#define PRESSURE_OUT_LOW_ADDR 0x1B
#define PRESSURE_OUT_XL_ADDR 0x1C
#define TEMPERATURE_OUT_HIGH_ADDR 0x1D
#define TEMPERATURE_OUT_LOW_ADDR 0x1E

#define PRESSURE_AND_TEMPERATURE_DATA_LEN 5
#define COUNTS_PER_HPASCAL 2048 // TODO: check if correct
#define COUNTS_PER_CELSIUS 32

void BM1390GLV_ZTR_init(void) {
  i2c_init();
  i2c_address(BM1390GLV_ZTR_I2C_SLAVE_ADDR);
}

void BM1390GLV_ZTR_cfg(void) {
  i2c_start();
  /* Write to register to set sensor configuration*/

  i2c_stop();
}

void BM1390GLV_ZTR_read(uint32_t *pressure, float *temperature) {
  uint8_t raw_data[PRESSURE_AND_TEMPERATURE_DATA_LEN];

  i2c_start();
  i2c_address(BM1390GLV_ZTR_I2C_SLAVE_ADDR);
  i2c_read(TEMPERATURE_OUT_HIGH_ADDR, raw_data,
           PRESSURE_AND_TEMPERATURE_DATA_LEN);
  i2c_stop();

  *pressure = calculate_hpascal(raw_data);
  // *temperature = calculate_celsius(raw_data);
}

uint32_t calculate_hpascal(char *raw_data) {

  uint32_t pressure = 0;
  pressure |= (uint32_t)((uint8_t)raw_data[0] << 16);
  pressure |= (uint32_t)((uint8_t)raw_data[1] << 8);
  pressure |= (uint32_t)((uint8_t)raw_data[2]);

  return pressure;
}

// float calculate_celsius(char *raw_data){

//   uint32_t temperature_raw = 0;
//   temperature_raw |= (uint32_t) ((uint8_t) raw_data[3] << 8);
//   temperature_raw |= (uint32_t) ((uint8_t) raw_data[4]);
//   float temperature;
//   memcpy(&temperature, &temperature_raw, sizeof(temperature_raw));

//   // outside of sensor range
//   if ((temperature < -40.0) || (temperature > 85.0)) {
//     return -1000.0;
//   }

//   return temperature;
// }