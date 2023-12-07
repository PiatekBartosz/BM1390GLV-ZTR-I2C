#include "sensor_simulator.hpp"

int main(void) { 
    const char* relativeDataPath = "../data/222.txt";

    std::ifstream iFile(relativeDataPath);
    std::string line;

    if (!iFile.is_open()) {
        std::cerr << "Unable to open data/222.txt file" << std::endl;
    }

    while (std::getline(iFile, line)) {
        std::cout << line << std::endl;
    }

    iFile.close();

    return 0;
}