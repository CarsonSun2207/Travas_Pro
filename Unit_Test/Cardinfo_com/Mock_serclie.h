#ifndef MOCK_SERCLIE_H
#define MOCK_SERCLIE_H

#include <gmock/gmock.h>
#include "Server.h"
#include "client.h"
#include <iostream>

class MockServer : public Server {
public:
    MockServer() {
        std::cout << "MockServer constructor" << std::endl;
    }
    MOCK_METHOD(void, updateServerWithCardID, (const std::string& cardID), (override));
};

class MockClient : public Client {
public:
    MockClient(Server* server) : Client(server) {
        std::cout << "MockClient constructor" << std::endl;
        // Ensure scanCard calls the base class method by default
        ON_CALL(*this, scanCard(testing::_)).WillByDefault(testing::Invoke(this, &MockClient::Client::scanCard));
    }
    MOCK_METHOD(void, scanCard, (const std::string& cardID), (override));
    MOCK_METHOD(void, sendmsg, (), (override));
};

#endif // MOCK_SERCLIE_H
