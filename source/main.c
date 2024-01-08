#include "BM1390GLV-ZTR.h"
#include "i2c.h"

int main(void) {
  double timeConstant;
  uint32_t sensitivity;

  double raw_a, raw_b;

  printf("Enter sensitivity: [Pa], for example 10\n");
  if (scanf("%lf", &raw_a) != 1 || raw_a < 0) {
    printf("Invalid input sensivity\n");
    return 1;
  }

  printf("Enter time constant: [s], for example 1s: 1\n");
  if (scanf("%lf", &raw_b) != 1 || raw_b <= 0.0) {
    printf("Invalid input time constant\n");
  }

  sensitivity = (uint32_t)raw_a;
  timeConstant = raw_b;

  // assume 60 measurements per second
  uint32_t threshold = (uint32_t)(timeConstant * 60);

  BM1390GLV_ZTR_init();

  uint32_t pressure;
  float temperature;
  BM1390GLV_ZTR_read(&pressure, &temperature);
  printf("Pressure: %d\n", pressure);

  uint32_t intial_pressure = pressure;

  size_t higher_count = 0;
  size_t lower_count = 0;

  while (1) {
    int ret = BM1390GLV_ZTR_read(&pressure, &temperature);
    // END Program
    if (ret != 0) {
      return 0;
    }
    // printf("Pressure: %d\n", pressure);

    if (pressure > intial_pressure + sensitivity) {
      higher_count++;
      lower_count = 0;
    } else if (pressure < intial_pressure - sensitivity) {
      lower_count++;
      higher_count = 0;
    } else {
      higher_count = 0;
      lower_count = 0;
    }

    if ((float)higher_count > timeConstant) {
      printf("Pressure increased from initial value: %d\n", intial_pressure);
      printf("To: %d\n\n", pressure);
      intial_pressure = pressure;
    } else if ((float)lower_count > timeConstant) {
      printf("Pressure decreased from initial value: %d\n", intial_pressure);
      printf("To: %d\n\n", pressure);
      intial_pressure = pressure;
    }
  }

  i2c_stop();
  return 0;
}