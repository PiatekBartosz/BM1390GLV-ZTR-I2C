#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

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

typedef uint8_t RegisterAddress;

typedef union {
    // used to acces individual bits
    struct {
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
    } u;

    uint8_t data;

} Register;

typedef struct {
    RegisterAddress manufacturerIdAddr;
    Register manufacturerId;

    RegisterAddress partIdAddr;
    Register partId;

    RegisterAddress powerDownAddr;
    Register powerDown;

    RegisterAddress resetAddr;
    Register reset;

    RegisterAddress modeControlAddr;
    Register modeControl;

    RegisterAddress fifoControlAddr;
    Register fifoControl;

    RegisterAddress fifoDataAddr;
    Register fifoData;

    RegisterAddress statusAddr;
    Register status;

    RegisterAddress pressureOutHighAddr;
    Register pressureOutHigh;

    RegisterAddress pressureOutLowAddr;
    Register pressureOutLow;

    RegisterAddress pressureOutXlAddr;
    Register pressureOutXl;

    RegisterAddress temperatureOutHighAddr;
    Register temperatureOutHigh;

    RegisterAddress temperatureOutLowAddr;
    Register temperatureOutLow;

} SensorRegisters;

int parse_line(char *line, Register *reg);