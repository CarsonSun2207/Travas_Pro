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

TEST(ServerTest, SetupServer) {
    Server server;
    int server_fd;
    struct sockaddr_in address;
    int port = 5560;

    server.setupServer(server_fd, address, port);
    EXPECT_GT(server_fd, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}