cmake_minimum_required(VERSION 3.10)
project(ClientServerTest01)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

# Add Google Test and Mock
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Add source files
add_executable(ClientServerTest01 client.cpp  Server.cpp  ser&clie_test01.cpp)

# Link Google Test and Mock
target_link_libraries(ClientServerTest01 gtest_main gmock_main)

include(GoogleTest)
gtest_discover_tests(ClientServerTest01)
