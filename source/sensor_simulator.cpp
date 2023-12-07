#include "sensor_simulator.hpp"

int main(void) { 
    const char* relativeDataPath = "../data/222.txt";

    SensorRegisters volatile sensorRegisters;

    std::ifstream iFile(relativeDataPath);
    std::string line;

    if (!iFile.is_open()) {
        std::cerr << "Unable to open data/222.txt file" << std::endl;
    }

    while (std::getline(iFile, line)) {
        std::cout << "Read data from file: "<< line << std::endl;
        // parse_line(line, &sensorRegisters);


    }

    iFile.close();

    return 0;
}

int parse_line(const std::string &line, SensorRegisters volatile *sensorRegisters) { 
    // std::istringstream iss(line);
    // std::string ignore;
    // std::vector<std::string> resuts;

    // TODO parse line

    return 0;
}
