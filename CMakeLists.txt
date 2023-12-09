cmake_minimum_required(VERSION 3.0.0)
project(
    SensorSimulatorI2C 
    VERSION 0.1.0
    DESCRIPTION "BM1390GLB sensor I2C simulation"
    LANGUAGES CXX C
)

# Add library source files
add_library(MainAppLib 
    source/i2c.c 
    source/BM1390GLV-ZTR.c
)

# Set include directories for the library
target_include_directories(MainAppLib PRIVATE include)

# Add the main executable
add_executable(MainApp 
    source/main.c
)

# Link the executable with the library
target_link_libraries(MainApp PRIVATE MainAppLib)

# Add the SensorSimulator executable
add_executable(SensorSimulator 
    source/sensor_simulator.cpp
)

# Set include directories for the executables
target_include_directories(SensorSimulator PUBLIC include)
target_include_directories(MainApp PUBLIC include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)