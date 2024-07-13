#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
private:
    bool sig = 1;
    const char entermsg[9] = "Entered";
    const char exitmsg[7] = "Exited";

public:
    char* rdmsg = new char[2];
    int sock;
    bool flag;
    int bytesRead;

    Client();
    ~Client();
    int byte_rd();
    void setupSocket(const std::string& host, int port);
    void sendmsg();
    bool sigiden();
    void readsig();
};

#endif // CLIENT_H
