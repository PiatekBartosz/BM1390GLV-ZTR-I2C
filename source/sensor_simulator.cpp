#include "sensor_simulator.hpp"

int main(void) {

  // socket programming
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Socket creation failed..." << std::endl;
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
    std::cout << "Socket bind failed..." << std::endl;
    return -1;
  }

  if ((listen(sockfd, 5)) != 0) {
    std::cout << "Listen failed..." << std::endl;
    return -1;
  }

  std::cout << "Socket successfully binded..." << std::endl;
  len = sizeof(cli);

  // file reading
  const char *relativeDataPath = "../data/222.txt";
  volatile SensorRegisters sensorRegisters;

  std::ifstream iFile(relativeDataPath);
  std::string line;

  if (!iFile.is_open()) {
    std::cerr << "Unable to open data/222.txt file" << std::endl;
    return -1;
  }

  while (1) {
    connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t *)&len);
    
    if (connfd < 0) {
      std::cout << "Server acccept failed..." << std::endl;
      return -1;
    }

    handleclient(connfd, &sensorRegisters);

  }

  // while (std::getline(iFile, line)) {
  //   std::cout << "Read data from file: " << line << std::endl;
  //   parseLine(line, &sensorRegisters);
  //   break;
  // }

  iFile.close();
  close(connfd);
  close(sockfd);

  return 0;
}

int parseLine(std::string line, volatile SensorRegisters *sensorRegisters) {
  // TODO change vector to packing the struct
  std::vector<std::string> results;
  std::vector<std::string> delimiters = {
      "SRC:   ", "CNT: ", "DSTR: ", "MODE: ", "EXC: ",  "WID:   ", "GID:   ",
      "BAT:  ",  "PRS: ", "TMP: ",  "TS: ",   "RSSI: ", "FPPL: ",  "CSQ: "};

  for (std::size_t i = 1; i < delimiters.size() - 1; ++i) {

    std::string::size_type pos = line.find(delimiters[i]);
    if (pos == std::string::npos) {
      std::cerr << "Unable to find delimiter: " << delimiters[i + 1]
                << std::endl;
      return -1;
    }

    std::string token = line.substr(0, pos);

    // put CNT
    // if (i == 1)

    // put PRS
    if (i == 9) {
      std::cout << "PRS: " << token << std::endl;
    }

    // put TMP
    if (i == 10) {
      std::cout << "TMP: " << token << std::endl;
    }

    // std::cout << "Token: " << token << std::endl;

    line = line.substr(pos, line.size());
  }

  return 0;
}

int initSensorRegisters(volatile SensorRegisters *sensorRegisters) {
  sensorRegisters->manufacturerIdAddr = MANUFACTURER_ID;
  sensorRegisters->partIdAddr = PART_ID;
  return 0;
}

void handleclient(int connfd, volatile SensorRegisters *sensorRegisters) {
  char buff[BUFFER_SIZE];
  int n;

  bzero(buff, sizeof(buff));

  read(connfd, buff, sizeof(buff));

  printf("Received data from client: %s \n", buff);
  bzero(buff, sizeof(buff));
  n = 0;

  /*
    DO FILE READING HERE 
    IF reached EOF, break loop
  */

  // copy server message in the buffer
  while ((buff[n++] = getchar()) != '\n')
    ;

  // TODO later, for now send echo
  write(connfd, buff, sizeof(buff));
}