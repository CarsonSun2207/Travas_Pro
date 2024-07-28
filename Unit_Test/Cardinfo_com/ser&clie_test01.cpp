#include <gtest/gtest.h>
#include <iostream>
#include "client.h"

class TestServer : public Server {
public:
    void updateServerWithCardID(const std::string& cardID) override {
        std::cout << "TestServer::updateServerWithCardID called with cardID: " << cardID << std::endl;
    }
};

class ClientServerTest : public ::testing::Test {
protected:
    TestServer testServer;
    Client client;

    ClientServerTest() : client(&testServer) {
        std::cout << "ClientServerTest constructor" << std::endl;
    }
};

TEST_F(ClientServerTest, SimulatesCardReading) {
    std::cout << "Running SimulatesCardReading test" << std::endl;

    // Simulate card ID
    std::string cardID = "12345";

    // Simulate scanning card
    std::cout << "Before client.scanCard" << std::endl;
    client.scanCard(cardID);
    std::cout << "After client.scanCard" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
