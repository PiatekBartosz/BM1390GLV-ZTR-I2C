#include "main.h"
#include "BM1390GLV-ZTR.h"
#include "i2c.h"

int main(void) {
  printf("Hello from main.c\n");

  BM1930GLV_ZTR_init();

  init_i2c();

  // create a TCP client
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servaddr;

  if (sockfd < 0) {
    printf("Error creating socket\n");
    return -1;
  }

  // zero a block of memory
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(IP);
  servaddr.sin_port = htons(PORT);

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
    printf("Error connecting to server\n");
    return -1;
  }

  while (1) {
    i2c_write(sockfd, 0x00, NULL, 0);
    i2c_read(sockfd, 0x00, NULL, 0);
  }

  close(sockfd);

  return 1;
}