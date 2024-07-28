#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <iostream>

class Server {
public:
    Server() {
        std::cout << "Server constructor" << std::endl;
    }
    virtual ~Server() {
        std::cout << "Server destructor" << std::endl;
    }
    virtual void updateServerWithCardID(const std::string& cardID) = 0;
};

#endif // SERVER_H
