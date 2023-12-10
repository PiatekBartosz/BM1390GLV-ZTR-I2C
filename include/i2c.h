#ifndef I2C_H
#define I2C_H

#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

// I2C interface
int i2c_init(void);
bool i2c_start(void);
bool i2c_stop(void);
bool i2c_address(uint8_t addr);
bool i2c_write(uint8_t *data, size_t size);
bool i2c_read(uint8_t register_address, uint8_t *data, size_t size);

// functions to simulate I2C using sockets
bool socket_write(char *buff, size_t size);
bool socket_read(char *buff);

#endif // I2C_H