#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "Server.h"

class Client {
public:
    Client(Server* server) : server_(server), flag(0) {
        std::cout << "Client constructor" << std::endl;
    }
    virtual ~Client() {
        std::cout << "Client destructor" << std::endl;
    }
    virtual void scanCard(const std::string& cardID);
    virtual void sendmsg();
    int flag;
    Server* getServer() const { return server_; }

private:
    Server* server_;
};

#endif // CLIENT_H
