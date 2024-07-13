cmake_minimum_required(VERSION 3.10)

# Set CMP0135 to NEW if applicable
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# Project name
project(Server_test)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable for the main application
add_executable(Server_test serverfinal2.cpp Readmsg.cpp Server.cpp Trafficsig.cpp)

# Include directories
target_include_directories(Server_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Manually specify the pigpio include directory and library
set(PIGPIO_INCLUDE_DIR "/usr/local/include")  # Update this path if necessary
set(PIGPIO_LIBRARY "/usr/local/lib/libpigpio.so")  # Update this path if necessary

# Include the pigpio headers
target_include_directories(Server_test PRIVATE ${PIGPIO_INCLUDE_DIR})

# Link the pigpio library
target_link_libraries(Server_test PRIVATE ${PIGPIO_LIBRARY} pthread)

# Download and include Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add the unit test executable
add_executable(Trafficsig_test Tsig_test.cpp Server.cpp Readmsg.cpp Trafficsig.cpp)

# Include the same directories for the test executable
target_include_directories(Trafficsig_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${PIGPIO_INCLUDE_DIR})

# Link Google Test, pthread, and other necessary libraries to the test executable
target_link_libraries(Trafficsig_test PRIVATE gtest gtest_main pthread ${PIGPIO_LIBRARY})

# Add test cases
add_test(NAME Trafficsig_test COMMAND Trafficsig_test)
