#include <gtest/gtest.h>
#include <iostream>
#include "client.h"

// TestServer class inherits from Server and provides a test implementation for updateServerWithCardID
class TestServer : public Server {
public:
    // Override the updateServerWithCardID method to provide a test implementation
    void updateServerWithCardID(const std::string& cardID) override {
        std::cout << "TestServer::updateServerWithCardID called with cardID: " << cardID << std::endl;
    }
};

// ClientServerTest class inherits from ::testing::Test and sets up the test environment
class ClientServerTest : public ::testing::Test {
protected:
    TestServer testServer; // Instance of TestServer
    Client client;         // Instance of Client

    // Constructor initializes the client with the test server
    ClientServerTest() : client(&testServer) {
        std::cout << "ClientServerTest constructor" << std::endl;
    }
};

// Test case for simulating card reading
TEST_F(ClientServerTest, SimulatesCardReading) {
    std::cout << "Running SimulatesCardReading test" << std::endl;

    // Simulate card ID
    std::string cardID = "12345";

    // Simulate scanning card
    std::cout << "Before client.scanCard" << std::endl;
    client.scanCard(cardID);
    std::cout << "After client.scanCard" << std::endl;
}

// Main function to run all the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); // Run all the tests
}
