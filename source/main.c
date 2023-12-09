#include "main.h"
#include "BM1390GLV-ZTR.h"
#include "i2c.h"

int main(void) {
    printf("Hello from main.c\n");

    BM1930GLV_ZTR_init();

    init_i2c();

    return 0;
}