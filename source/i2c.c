#include "i2c.h"

/*
 * All I2C functions have commends with steps that would
 * be done on registers.
 */

#define IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

static int sockfd = -1;
static uint8_t i2c_slave_addr = 0x00;

int i2c_init(void) {
  /* What would this function do on registers:
   * 1. Enable I2C and GPIO clock
   * 2. Configure I2C pins for alternate functions
   * 3. Reset I2C
   * 4. Program the peripheral input clock in I2C register
   * 5. Configure clock control
   * 6. Configure rise time
   * 7. Enable peripheral
   */

  /* Simulate it by configuring socket: */
#ifdef _WIN32 
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("Error initializing socket\n");
    return -1;
  }
#endif

  // create a TCP client
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in servaddr;

  if (sockfd < 0) {
    printf("Error creating socket\n");
    return -1;
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(IP);
  servaddr.sin_port = htons(PORT);

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
    printf("Error connecting to server\n");
    return -1;
  }
}

bool i2c_start(void) {
  /* What would this function do on registers:
   * 1. Send START condition
   * 2. Wait for START condition to be sent
   */

  if (sockfd < 0) {
    printf("I2C was not initialized\n");
    return false;
  }

  /* Simulate starting condition */
  return socket_start_condition();
}

bool i2c_stop(void) {
  /* What would this function do on registers:
   * 1. Stop I2C
   */

  /* Simulate stop condition */
  return socket_stop_condition();
}

bool i2c_address(uint8_t addr) {
  /* What would this function do on registers
   * 1. Set the slave address to the MCU DR register
   * 2. Wait for the address to be set
   * 3. Clear addr by reading SR1 and SR2
   */

  // in STM32 I2C 7-bit address must be shifted left by 1 when setting DR
  // register as the 0-bit will be used to indicate R/W
  i2c_slave_addr = (addr << 1);
  return true;
}

bool i2c_write(uint8_t register_address, uint8_t *data) {
  /* What would this function do on registers:
   * 1. Set the address of the slave device with which we want to communicate
   * 2. Set the register address we want to write to
   * 3. Write the data to the register
   * 4. Return the data
   */

  if (sockfd < 0) {
    printf("I2C was not initialized\n");
    return false;
  }

  if (i2c_slave_addr == 0x00) {
    printf("I2C address not changed\n");
    return false;
  }

  char buff[BUFFER_SIZE];
  int n;

  // strcpy(buff, "Hello from client");
  // int ret = sockbuff, sizeof(buff));

  // if (ret < 0) {
  //   printf("Error writing to server\n");
  //   return false;
  // }

  return true;
}

bool i2c_read(uint8_t register_address, uint8_t *data, size_t size) {
  /* Steps for reading I2C on registers:
   * 1. Set the address of the slave device with which we want to communicate
   * 2. Set the register address we want to read from
   * 3. Read the data from the register
   * 4. Return the data
   */

  if (sockfd < 0) {
    printf("I2C was not initialized\n");
    return false;
  }

  if (i2c_slave_addr == 0x00) {
    printf("I2C address not changed\n");
    return false;
  }

  /* I2C frame simulation: */

  /* Send slave address with and expect aknowledge*/ 
  uint8_t slave_adr_with_read_bit = i2c_slave_addr | 0x01;
  uint8_t aknowledge_bit = 0xFF; // have to simulate aknowledge bit with a byte
  uint8_t buff[2] = {slave_adr_with_read_bit, aknowledge_bit};
  bool ret = socket_write(buff, 2);
  if (!ret) {
    printf("Error writing to server\n");
    return false;
  }
  ret = socket_read(buff, 2);
  if (!ret) {
    printf("Error reading from server\n");
    return false;
  }
  if (buff[1] != 0x00) {
    printf("Error with aknowledge I2C slave address\n");
    return false;
  }

  /* Send register address and expect aknowledge*/
  buff[0] = register_address;
  buff[1] = 0xFF;
  ret = socket_write(buff, 2);
  if (!ret) {
    printf("Error writing to server\n");
    return false;
  }
  ret = socket_read(buff, 2);
  if (!ret) {
    printf("Error reading from server\n");
    return false;
  }
  if (buff[1] != 0x00) {
    printf("Error with aknowledge I2C register address\n");
    return false;
  }

  /* Read register data */
  for (size_t i = 0; i < size; ++i) {
    buff[0] = 0x00;  
    buff[1] = 0xFF;
    ret = socket_write(buff, 2);
    if (!ret) {
      printf("Error writing to server\n");
      return false;
    }
    ret = socket_read(buff, 2);
    if (!ret) {
      printf("Error reading from server\n");
      return false;
    }
    if (buff[1] != 0x00) {
      printf("Error with aknowledge I2C register address\n");
      return false;
    }
    data[i] = buff[0];
  }
  return true;
}

bool socket_start_condition() {
  /* simulating staring condition of I2C frame */

  char *buff = "START CONDITION";
  if (sockfd < 0) {
    printf("I2C was not initialized\n");
    return false;
  }

  socket_write(buff, strlen(buff));
  socket_read(buff, strlen(buff));

  // if ((ret_read < 0) || (ret_write < 0)) {
  //   printf("Error writing or reading server\n");
  //   return false;
  // }

  if (strcmp(buff, "START CONDITION") != 0) {
    printf("Error sending START condition\n");
    return false;
  }
  printf("Master: START CONDITION accepted\n");
  return true;
}

bool socket_stop_condition()
{
  char *buff = "STOP CONDITION";
  if (sockfd < 0) {
    printf("I2C was not initialized\n");
    return false;
  }

  socket_write(buff, strlen(buff));
  socket_read(buff, strlen(buff));

  // if ((ret_read < 0) || (ret_write < 0)) {
  //   printf("Error writing or reading server\n");
  //   return false;
  // }

  if (strcmp(buff, "STOP CONDITION") != 0) {
    printf("Error sending STOP condition\n");
    return false;
  }
  return true; 
}

bool socket_read(char *buff, size_t byte_count) {
  if (sockfd < 0) {
    printf("I2C was not initialized\n");
    return false;
  }

#ifdef _WIN32
  int ret = recv(sockfd, buff, byte_count, 0);
#else
  int ret = read(sockfd, buff, byte_count);
#endif

  if (ret == SOCKET_ERROR) {
    printf("Error writing to server\n");
    return false;
  }

  return true;
}

bool socket_write(char *buff, size_t byte_count) {
  if (sockfd < 0) {
    printf("I2C was not initialized\n");
    return false;
  }

#ifdef _WIN32
  int ret = send(sockfd, buff, byte_count, 0);
#else
  int ret = write(sockfd, buff, byte_count);
#endif

  if (ret < 0) {
    printf("Error writing to server\n");
    return false;
  }

  return true;
}