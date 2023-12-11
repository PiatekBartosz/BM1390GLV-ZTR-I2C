#include "BM1390GLV-ZTR.h"
#include "i2c.h"

int main(void) {
  printf("Hello from main.c\n");
  BM1390GLV_ZTR_init();

  i2c_init();

  while (1) {
    float pressure, temperature;

    BM1390GLV_ZTR_cfg();
    BM1390GLV_ZTR_read(&pressure, &temperature);

    printf("Pressure: %f\n", pressure);
    printf("Temperature: %f\n", temperature);

    break;
  }

  i2c_stop();
  return 1;
}