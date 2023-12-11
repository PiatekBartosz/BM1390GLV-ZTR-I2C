#ifndef BM1390GLV_ZTR_H
#define BM1390GLV_ZTR_H

#include <stdint.h>
#include <stdio.h>

void BM1390GLV_ZTR_init(void);
void BM1390GLV_ZTR_cfg(void);
void BM1390GLV_ZTR_read(float *pressure, float *temperature);

float calculate_hpascal(uint32_t pressure_raw);
float calculate_celsius(int32_t temperature_raw);

#endif // BM1390GLV_ZTR_H