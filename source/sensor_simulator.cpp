#include "sensor_simulator.hpp"

#define MANUFACTURER_ID 0xE0
#define PART_ID 0x34
#define COUNTS_PER_HPASCAL 2048 // TODO: check if correct
#define COUNTS_PER_CELSIUS 32

#define IP "127.0.0.1"
#define PORT 8081
#define BUFFER_SIZE 100
#define I2C_SLAVE_ADDRESS 0x5D

static int sockfd = -1;
static int connfd = -1;

int main(void) {

#ifdef _WIN32
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "WSAStartup failed." << std::endl;
    return -1;
  }
#endif

  /*
    Socket creation
  */

  int len;
  struct sockaddr_in serv_addr, client_addr;
#ifdef _WIN32
  int client_addr_len = sizeof(client_addr);
#else
  socklen_t client_addr_len = sizeof(client_addr);
#endif

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Socket creation failed..." << std::endl;
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(IP);
  serv_addr.sin_port = htons(PORT);

  if ((bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) != 0) {
    std::cout << "Socket bind failed..." << std::endl;
#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
    return -1;
  }

  if ((listen(sockfd, 5)) != 0) {
    std::cout << "Listen failed..." << std::endl;
#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
    return -1;
  }

  std::cout << "Socket successfully binded..." << std::endl;

  /*
    File reading
  */

  const char *relativeDataPath = "../data/222.txt";

  std::unordered_map<RegisterAddress, Register> sensorRegistersMap =
      initSensorRegisters();

  std::ifstream iFile(relativeDataPath);
  std::string line;

  if (!iFile.is_open()) {
    std::cerr << "Unable to open data/222.txt file" << std::endl;
    std::cerr << "Make sure you are in the build directory: ./SensorSimulator"
              << std::endl;
    return -1;
  }

  connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

  /*
    Main app loop
  */

  while (1) {

    if (connfd < 0) {
      std::cout << "Server acccept failed..." << std::endl;
#ifdef _WIN32
      closesocket(sockfd);
      WSACleanup();
#else
      close(sockfd);
#endif
      return -1;
    }

    if (std::getline(iFile, line)) {
      uint32_t pressure;
      float temperature;

      parseLine(line, pressure, temperature);

      // refill registers with current data
      putPressTempDataRegisters(sensorRegistersMap, pressure, temperature);

      if (connfd < 0) {
        std::cout << "Server acccept failed..." << std::endl;
        return -1;
      }

      // handles client requests with I2C simulation
      int ret = handleClient(sensorRegistersMap);
      if (ret != 0) {
        std::cerr << "Error handling client" << std::endl;
        return -1;
      }
    }

    else {
      char buff[BUFFER_SIZE] = {0};
      socket_read(buff, 5);
      strcpy(buff, "END");
      socket_write(buff, 3);
      break;
    }
  }

  iFile.close();

#ifdef _WIN32
  closesocket(sockfd);
  closesocket(connfd);
  WSACleanup();
#else
  close(connfd);
  close(sockfd);
#endif

  std::cout << "End of data" << std::endl;

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

// Function to initialize the registers
std::unordered_map<RegisterAddress, Register> initSensorRegisters() {

  std::unordered_map<RegisterAddress, Register> sensorRegistersMap = {
      {MANUFACTURER_ID_ADDR, {0}},    {PART_ID_ADDR, {0}},
      {POWER_DOWN_ADDR, {0}},         {RESET_ADDR, {0}},
      {MODE_CONTROL_ADDR, {0}},       {FIFO_CONTROL_ADDR, {0}},
      {FIFO_DATA_ADDR, {0}},          {STATUS_ADDR, {0}},
      {PRESSURE_OUT_HIGH_ADDR, {0}},  {PRESSURE_OUT_LOW_ADDR, {0}},
      {PRESSURE_OUT_XL_ADDR, {0}},    {TEMPERATURE_OUT_HIGH_ADDR, {0}},
      {TEMPERATURE_OUT_LOW_ADDR, {0}}};

  sensorRegistersMap[MANUFACTURER_ID_ADDR].data = MANUFACTURER_ID;
  sensorRegistersMap[PART_ID_ADDR].data = PART_ID;
  return sensorRegistersMap;
}

// TODO: make test
int putPressTempDataRegisters(
    std::unordered_map<RegisterAddress, Register> &sensorRegistersMap,
    uint32_t pressure, float temperature) {

  // do not account for COUNTS_PER_HPASCAL to avoid loss of precision
  // uint32_t raw_pressure = pressure * COUNTS_PER_HPASCAL;
  sensorRegistersMap[PRESSURE_OUT_HIGH_ADDR].data = (pressure >> 16) & 0xFF;
  sensorRegistersMap[PRESSURE_OUT_LOW_ADDR].data = (pressure >> 8) & 0xFF;
  sensorRegistersMap[PRESSURE_OUT_XL_ADDR].data = pressure & 0xFF;

  // There might be a loss in precision because of float preccision
  // TODO: change serialization
  // not used in the example
  uint32_t raw_temperature;
  static_assert(sizeof(float) == sizeof(uint32_t),
                "Size mismatch between float and uint32_t");
  std::memcpy(&raw_temperature, &temperature, sizeof(raw_temperature));
  sensorRegistersMap[TEMPERATURE_OUT_HIGH_ADDR].data =
      static_cast<char>((raw_temperature >> 8) & 0xFF);
  sensorRegistersMap[TEMPERATURE_OUT_LOW_ADDR].data =
      static_cast<char>(raw_temperature & 0xFF);

  return 0;
}

int handleClient(
    std::unordered_map<RegisterAddress, Register> &sensorRegistersMap) {
  char buff[BUFFER_SIZE] = {0};
  int n;

  // Expect I2C START CONDITION
  int start_condition_len = strlen("START");
  socket_read(buff, start_condition_len);
  if (strcmp(buff, "START") != 0) {
    std::cerr << "Expected START CONDITION" << std::endl;
    return 1;
  }
  std::cout << "Slave: Got start condition" << std::endl;
  memset(buff, 0, BUFFER_SIZE);
  strcpy(buff, "START");
  socket_write(buff, start_condition_len);
  std::cout << "Slave: Start condition sent" << std::endl;

  // Expect I2C SLAVE ADDRESS & ACK
  socket_read(buff, 2);
  uint8_t slave_address = (uint8_t)buff[0];
  uint8_t ack = (uint8_t)buff[1];
  if (slave_address >> 1 != I2C_SLAVE_ADDRESS) {
    std::cerr << "Expected I2C SLAVE ADDRESS" << std::endl;
    return 1;
  }
  buff[0] = slave_address;
  buff[1] = 0x00;
  socket_write(buff, 2);

  // Check if write mode
  if ((slave_address & 0x01) == 0x00) {
    // Expect I2C REGISTER ADDRESS & ACK
    socket_read(buff, 2);
    uint8_t register_address = (uint8_t)buff[0];
    uint8_t ack = (uint8_t)buff[1];
    buff[0] = register_address;
    buff[1] = 0x00;
    socket_write(buff, 2);

    // Expect 2nd START CONDITION
    start_condition_len = strlen("START");
    socket_read(buff, start_condition_len);
    if (strcmp(buff, "START") != 0) {
      std::cerr << "Expected 2nd START CONDITION" << std::endl;
      return 1;
    }
    memset(buff, 0, BUFFER_SIZE);
    strcpy(buff, "START");
    socket_write(buff, start_condition_len);

    // Expect slave addres 2nd time and ACK
    socket_read(buff, 2);
    slave_address = (uint8_t)buff[0];
    ack = (uint8_t)buff[1];
    if (slave_address >> 1 != I2C_SLAVE_ADDRESS) {
      std::cerr << "Expected I2C SLAVE ADDRESS" << std::endl;
      return 1;
    }
    buff[0] = slave_address;
    buff[1] = 0x00;
    socket_write(buff, 2);

    // Expect blank message
    socket_read(buff, 2);

    // Send data to master until NACK
    for (int i = 0; i < 5; ++i) {
      switch (i) {
      case 0:
        buff[0] = sensorRegistersMap[PRESSURE_OUT_HIGH_ADDR].data;
        buff[1] = 0xFF;
        break;

      case 1:
        buff[0] = sensorRegistersMap[PRESSURE_OUT_LOW_ADDR].data;
        buff[1] = 0xFF;
        break;

      case 2:
        buff[0] = sensorRegistersMap[PRESSURE_OUT_XL_ADDR].data;
        buff[1] = 0xFF;
        break;

      case 3:
        buff[0] = sensorRegistersMap[TEMPERATURE_OUT_HIGH_ADDR].data;
        buff[1] = 0xFF;
        break;

      case 4:
        buff[0] = sensorRegistersMap[TEMPERATURE_OUT_LOW_ADDR].data;
        buff[1] = 0xFF;
        break;
      }

      socket_write(buff, 2);
      socket_read(buff, 2);

      ack = (uint8_t)buff[1];
      if (ack != 0x00) {
        std::cout << "Slave: No ACK received" << std::endl;
      }
    }
    socket_write(buff, 2);

    // Expect STOP CONDITION
    socket_read(buff, 4);
    if (std::strcmp(buff, "STOP") == 0) {
      std::cout << "Slave: Got STOP condition" << std::endl;
      strcpy(buff, "STOP");
      socket_write(buff, 4);
    }
  }
  return 0;
}

int socket_read(char *buff, int byte_count) {
  memset(buff, '\0', BUFFER_SIZE);
#ifdef _WIN32
  recv(connfd, buff, byte_count, 0);
#else
  read(connfd, buff, byte_count);
#endif
  return 0;
}

int socket_write(char *buff, int byte_count) {
#ifdef _WIN32
  send(connfd, buff, byte_count, 0);
#else
  write(connfd, buff, byte_count);
#endif

  // TODO: only for tests
  // std::cout << "Slave: Socket write: " << buff << std::endl;
  return 0;
}
