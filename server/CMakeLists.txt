cmake_minimum_required(VERSION 3.10)

# Project name
project(TrafficSignalServer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(TrafficSignalServer serverfinal2.cpp Server.cpp Readmsg.cpp Trafficsig.cpp)

# Include directories
target_include_directories(TrafficSignalServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Manually specify the pigpio include directory and library
set(PIGPIO_INCLUDE_DIR "/usr/local/include")  # Update this path if necessary
set(PIGPIO_LIBRARY "/usr/local/lib/libpigpio.so")  # Update this path if necessary

# Include the pigpio headers
target_include_directories(TrafficSignalServer PRIVATE ${PIGPIO_INCLUDE_DIR})

# Link the pigpio library
target_link_libraries(TrafficSignalServer PRIVATE ${PIGPIO_LIBRARY} pthread)
