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

void init_i2c(void);

bool i2c_write(int connfd, uint8_t addr, uint8_t *data, uint8_t len);

bool i2c_read(int connfd, uint8_t addr, uint8_t *data, uint8_t len);

#endif // I2C_H