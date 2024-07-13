#ifndef READMSG_H
#define READMSG_H

#include "CppThread.h"
#include "Server.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

class Readmsg : public CppThread {
private:
    Server& ser;
    void run()  {
        while (true) {
            if (ser.byteno() != 0) {
                strcpy(rdmsg, ser.readctmsg());
                std::cout << rdmsg << std::endl;
                // std::this_thread::sleep_for(200ms);
            }
        }
    }

public:
    Readmsg(Server& serversig) : ser(serversig) {};
    bool trigger;
    char rdmsg[9] = {0};
};

#endif // READMSG_H
