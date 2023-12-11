#include "sensor_simulator.hpp"

#define COUNTS_PER_HPASCAL 2048 // TODO: check if correct
#define COUNTS_PER_CELSIUS 32

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
  initSensorRegisters(&sensorRegisters);

  std::ifstream iFile(relativeDataPath);
  std::string line;

  if (!iFile.is_open()) {
    std::cerr << "Unable to open data/222.txt file" << std::endl;
    return -1;
  }

  while (1) {

    if (std::getline(iFile, line)) {
      uint32_t pressure;
      float temperature;

      parseLine(line, pressure, temperature);

      // refill registers with current data
      putPressTempDataRegisters(&sensorRegisters, pressure, temperature);

      connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t *)&len);

      if (connfd < 0) {
        std::cout << "Server acccept failed..." << std::endl;
        return -1;
      }

      // handles client requests with I2C simulation
      int ret = handleClient(connfd, &sensorRegisters);
      if (ret != 0) {
        std::cerr << "Error handling client" << std::endl;
        return -1;
      }
    }

    else {
      break;
    }
  }

  iFile.close();
  close(connfd);
  close(sockfd);

  return 0;
}

int parseLine(std::string &line, uint32_t &pressure, float &temperature) {

  std::vector<std::string> delimiters = {
      "SRC:   ", "CNT: ", "DSTR: ", "MODE: ", "EXC: ",  "WID:   ", "GID:   ",
      "BAT:  ",  "PRS: ", "TMP: ",  "TS: ",   "RSSI: ", "FPPL: ",  "CSQ: "};

  size_t pressurePos = line.find(delimiters[8]) + delimiters[8].size();
  size_t temperaturePos = line.find(delimiters[9]) + delimiters[9].size();

  std::string pressure_str =
      line.substr(pressurePos, line.find(delimiters[9]) - pressurePos - 1);
  std::string temperature_str = line.substr(
      temperaturePos, line.find(delimiters[10]) - temperaturePos - 1);

  std::cout << "Pressure: " << pressure_str << std::endl;
  std::cout << "Temperature: " << temperature_str << std::endl;

  try {
    pressure = std::stoi(pressure_str);
    temperature = std::stof(temperature_str);
  } catch (const std::invalid_argument &e) {
    std::cerr << "Error converting string to float/int: " << e.what()
              << std::endl;
    return 1; // Return an error code
  } catch (const std::out_of_range &e) {
    // Handle the case where the converted value is out of range
    std::cerr << "Error: " << e.what() << std::endl;
    return 1; // Return an error code
  }

  return 0;
}

int initSensorRegisters(volatile SensorRegisters *sensorRegisters) {
  sensorRegisters->manufacturerId.data = MANUFACTURER_ID;
  sensorRegisters->partId.data = PART_ID;
  return 0;
}

// TODO: make test
int putPressTempDataRegisters(volatile SensorRegisters *sensorRegisters,
                              uint32_t pressure, float temperature) {

  uint32_t raw_pressure = static_cast<uint32_t>(pressure * COUNTS_PER_HPASCAL);
  sensorRegisters->pressureOutHigh.data = (raw_pressure >> 16) & 0xFF;
  sensorRegisters->pressureOutLow.data = (raw_pressure >> 8) & 0xFF;
  sensorRegisters->pressureOutXl.data = raw_pressure & 0xFF;

  // There might be a loss in precision because of float preccision
  int32_t raw_temperature =
      static_cast<int32_t>(temperature * COUNTS_PER_CELSIUS);
  sensorRegisters->temperatureOutHigh.data = (raw_temperature >> 8) & 0xFF;
  sensorRegisters->temperatureOutLow.data = raw_temperature & 0xFF;
  return 0;
}

int handleClient(int connfd, volatile SensorRegisters *sensorRegisters) {
  char buff[BUFFER_SIZE];
  int n;
  bzero(buff, sizeof(buff));

  // Expect START CONDITION
  read(connfd, buff, sizeof(buff));
  if (strcmp(buff, "START") != 0) {
    std::cerr << "Expected START CONDITION" << std::endl;
    return 1;
  }
  printf("Received data from client: %s \n", buff);

  write(connfd, "OK", sizeof("OK"));


  // read(connfd, buff, sizeof(buff));

  // TODO later, for now send echo
}