#include <iostream>
#include <pigpio.h>
#include <unistd.h> 
#include <thread>
#include <cstring>
#include "Server.h"
#include "CppThread.h"
#include "Readmsg.h"
#include "Trafficsig.h"

using namespace std;

int main() {
    int server_fd;
    struct sockaddr_in address;
    int port = 5560;
    char keyin = 0;
    Server server;
    Readmsg rdmsg(server);
    Trafficsig trafsig(rdmsg,server);

    server.setupServer(server_fd, address, port);
    std::cout<<"aaa"<<std::endl;
    server.acceptConnection(server_fd, address);
    if (gpioInitialise() < 0) {
        std::cerr << "Pigpio initialisation failed." << std::endl;
        return 1;
    }
    trafsig.start();
    rdmsg.start();
    trafsig.join();
    rdmsg.join();
    std::cout<<"End connection?(y/n)"<<std::endl;
    std::cin>>keyin;
    if(keyin=='y')
       { close(server.clt_soc);
        close(server_fd);
       }
    
return 0;
}
