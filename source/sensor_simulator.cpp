#include "sensor_simulator.hpp"

int main(void) {
  const char *relativeDataPath = "../data/222.txt";

  volatile SensorRegisters sensorRegisters;

  std::ifstream iFile(relativeDataPath);
  std::string line;

  if (!iFile.is_open()) {
    std::cerr << "Unable to open data/222.txt file" << std::endl;
    return -1;
  }

  while (std::getline(iFile, line)) {
    std::cout << "Read data from file: " << line << std::endl;
    parseLine(line, &sensorRegisters);
    break;
  }

  iFile.close();

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