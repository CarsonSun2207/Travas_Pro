#ifndef MOCK_SERCLIE_H
#define MOCK_SERCLIE_H

#include <gmock/gmock.h>
#include "Server.h"
#include "client.h"
#include <iostream>

// Mock implementation of the Server class for testing purposes
class MockServer : public Server {
public:
    // Constructor prints a message for debugging
    MockServer() {
        std::cout << "MockServer constructor" << std::endl;
    }
    
    // Mock method to override the updateServerWithCardID function
    MOCK_METHOD(void, updateServerWithCardID, (const std::string& cardID), (override));
};

// Mock implementation of the Client class for testing purposes
class MockClient : public Client {
public:
    // Constructor initializes the base class with the server reference and prints a message for debugging
    MockClient(Server* server) : Client(server) {
        std::cout << "MockClient constructor" << std::endl;
        // Ensure scanCard calls the base class method by default
        ON_CALL(*this, scanCard(testing::_)).WillByDefault(testing::Invoke(this, &MockClient::Client::scanCard));
    }
    
    // Mock method to override the scanCard function
    MOCK_METHOD(void, scanCard, (const std::string& cardID), (override));
    
    // Mock method to override the sendmsg function
    MOCK_METHOD(void, sendmsg, (), (override));
};

#endif // MOCK_SERCLIE_H
