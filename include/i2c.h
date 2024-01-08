#ifndef I2C_H
#define I2C_H

// cross-platform socket programming
// for Windows
#ifdef _WIN32
#include <winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
// for Linux
#else
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()


// TODO: change return types and handle errors, get rid of bool?
// I2C interface
int i2c_init(void);
int i2c_start(void);
int i2c_stop(void);
int i2c_address(uint8_t addr);
int i2c_write(uint8_t register_address, uint8_t *data);
int i2c_read(uint8_t register_address, uint8_t *data, size_t size);

// functions to simulate I2C using sockets
int socket_start_condition();
int socket_stop_condition();
int socket_read(char *buff, size_t size);
int socket_write(char *buff, size_t size);
#endif // I2C_H