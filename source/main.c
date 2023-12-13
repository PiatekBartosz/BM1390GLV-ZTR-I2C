#include "BM1390GLV-ZTR.h"
#include "i2c.h"

int main(void) {
  double epsilon;
  double time_constant;
  printf("Enter the sensivity constant [Pa], for example 100 Pa\n");
  scanf("%lf", &epsilon);

  printf("Enter time constant [s], for example 0.1 s\n");
  scanf("%lf", &time_constant);

  BM1390GLV_ZTR_init();

  while (1) {
    uint32_t pressure;
    float temperature;

    // BM1390GLV_ZTR_cfg();
    BM1390GLV_ZTR_read(&pressure, &temperature);

    // printf("Pressure: %d\n", pressure);
  }

  i2c_stop();
  return 1;
}