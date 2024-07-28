#ifndef MOCK_SERCLIE_H
#define MOCK_SERCLIE_H

#include <gmock/gmock.h>
#include "Server.h"
#include "client.h"

class MockServer : public Server {
public:
    MOCK_METHOD(void, updateServerWithCardID, (const std::string& cardID), (override));
};

class MockClient : public Client {
public:
    MockClient(Server* server) : Client(server) {
        // Ensure scanCard calls the base class method by default
        ON_CALL(*this, scanCard(testing::_)).WillByDefault([this](const std::string& cardID) {
            this->Client::scanCard(cardID);
        });
    }
    MOCK_METHOD(void, scanCard, (const std::string& cardID), (override));
    MOCK_METHOD(void, sendmsg, (), (override));
};

#endif // MOCK_SERCLIE_H
