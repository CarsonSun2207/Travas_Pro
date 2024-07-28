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
    int server_fd;              // Server file descriptor
    struct sockaddr_in address; // Server address structure
    int port = 5560;            // Port number
    char keyin = 0;             // User input variable
    Server server;              // Server instance
    Readmsg rdmsg(server);      // Readmsg instance initialized with the server
    Trafficsig trafsig(rdmsg, server); // Trafficsig instance initialized with rdmsg and server

    // Setup the server
    server.setupServer(server_fd, address, port);
    std::cout << "aaa" << std::endl;
    
    // Accept connection from client
    server.acceptConnection(server_fd, address);

    // Initialize pigpio library
    if (gpioInitialise() < 0) {
        std::cerr << "Pigpio initialization failed." << std::endl;
        return 1;
    }

    // Start traffic signal and message reading threads
    trafsig.start();
    rdmsg.start();

    // Wait for the threads to finish
    trafsig.join();
    rdmsg.join();

    // Prompt user to end connection
    std::cout << "End connection?(y/n)" << std::endl;
    std::cin >> keyin;
    
    // Close connection if user inputs 'y'
    if (keyin == 'y') {
        close(server.clt_soc);
        close(server_fd);
    }

    return 0;
}
