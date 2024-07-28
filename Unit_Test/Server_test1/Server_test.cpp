#include <gtest/gtest.h>
#include <iostream>
#include <pigpio.h>
#include <unistd.h> 
#include <thread>
#include <cstring>
#include "Server.h"
#include "CppThread.h"
#include "Readmsg.h"
#include "Trafficsig.h"

// Test case for setting up the server
TEST(ServerTest, SetupServer) {
    Server server;           // Create a Server instance
    int server_fd;           // Server file descriptor
    struct sockaddr_in address; // Server address structure
    int port = 5560;         // Port number

    // Setup the server and expect the file descriptor to be greater than 0
    server.setupServer(server_fd, address, port);
    EXPECT_GT(server_fd, 0); // Check if server_fd is greater than 0
}

// Main function to run all the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); // Run all the tests
}
