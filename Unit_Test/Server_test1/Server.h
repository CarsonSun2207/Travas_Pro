// Server.h
#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

class Server {
private:
    char bff[8] = {0};
    int bffsize = 8;

public:
    int clt_soc;

    int byteno();
    void setupServer(int& server_fd, struct sockaddr_in& address, int port);
    void acceptConnection(int server_fd, struct sockaddr_in& address);
    char* readctmsg();
    void sendtrafsig(bool tfsig);
};

#endif // SERVER_H
