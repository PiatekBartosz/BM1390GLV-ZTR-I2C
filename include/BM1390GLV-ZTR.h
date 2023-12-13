#ifndef BM1390GLV_ZTR_H
#define BM1390GLV_ZTR_H

#include <stdint.h>
#include <stdio.h>

void BM1390GLV_ZTR_init(void);
void BM1390GLV_ZTR_cfg(void);
void BM1390GLV_ZTR_read(uint32_t *pressure, float *temperature);

uint32_t calculate_hpascal(char *raw_data);
float calculate_celsius(char *raw_data);

#endif // BM1390GLV_ZTR_H