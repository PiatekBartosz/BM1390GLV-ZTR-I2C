#ifndef SENSOR_SIMULATOR_HPP
#define SENSOR_SIMULATOR_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>

// cross-platform socket programming
// for Windows
#ifdef _WIN32
#include <winsock2.h>
// #include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
// for Linux
#else
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()

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
  // used to access individual bits
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

int parseLine(std::string &line, uint32_t &pressure, float &temperature);

std::unordered_map<RegisterAddress, Register> initSensorRegisters();

int putPressTempDataRegisters(
    std::unordered_map<RegisterAddress, Register> &sensorRegistersMap,
    uint32_t pressure, float temperature);

int handleClient(
    std::unordered_map<RegisterAddress, Register> &sensorRegistersMap);

int socket_read(char *buff, int buff_size);
int socket_write(char *buff, int buff_size);

#endif // SENSOR_SIMULATOR_HPP
