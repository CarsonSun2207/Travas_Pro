// Server.h
#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

class Server {
private:
    char bff[8] = {0};
    int bffsize = 8;
    char* rddmsg[8]={0};
public:
    int clt_soc;

    int byteno();
    void setupServer(int& server_fd, struct sockaddr_in& address, int port);
    void acceptConnection(int server_fd, struct sockaddr_in& address);
    char* readctmsg();
    virtual void sendtrafsig(bool tfsig);
};

#endif // SERVER_H
