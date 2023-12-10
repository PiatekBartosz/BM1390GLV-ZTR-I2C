#include "i2c.h"

void init_i2c(void) { /* Set registers to initialize I2C*/
}

bool i2c_write(int connfd, uint8_t addr, uint8_t *data, uint8_t len) {
  char buff[BUFFER_SIZE];
  int n;

  bzero(buff, sizeof(buff));
  strcpy(buff, "Hello from client");
  ssize_t ret = write(connfd, buff, sizeof(buff));

  if (ret < 0) {
    printf("Error writing to server\n");
    return false;
  }

  return true;
}

bool i2c_read(int connfd, uint8_t addr, uint8_t *data, uint8_t len) {
  char buff[BUFFER_SIZE];
  int n;

  bzero(buff, sizeof(buff));
  n = 0;

  read(connfd, buff, sizeof(buff));

  printf("From Server in CLient: %s\n", buff);

  // TODO change return value
  return true;
}
