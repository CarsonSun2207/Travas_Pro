cmake_minimum_required(VERSION 3.10)

# Project name
project(Client)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(Client final5.cpp linetracking.cpp motor.cpp pca9685.cpp ultrasonic.cpp client.cpp RFIDThread.cpp Readmsg.cpp BuzzCtrl.cpp)  

# Include directories
target_include_directories(Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Manually specify the pigpio include directory and library
set(PIGPIO_INCLUDE_DIR "/usr/local/include")  # Update this path if necessary
set(PIGPIO_LIBRARY "/usr/local/lib/libpigpio.so")  # Update this path if necessary

# Include the pigpio headers
target_include_directories(Client PRIVATE ${PIGPIO_INCLUDE_DIR})

#target_include_directories(Client PRIVATE /Desktop/ALL/CMFRC522.h)

# Link the pigpio library
target_link_libraries(Client PRIVATE ${PIGPIO_LIBRARY} pthread)

set(CMFRC522_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/CMFRC522.a") 
target_link_libraries(Client PRIVATE ${CMFRC522_LIBRARY})
