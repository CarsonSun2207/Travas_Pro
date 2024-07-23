#include "client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

Client::Client() {
    // Constructor implementation
}

Client::~Client() {
    delete[] rdmsg;  // Destructor to free allocated memory
}

int Client::byte_rd() {
    return read(sock, rdmsg, 5);
}

void Client::setupSocket(const std::string& host, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << "Socket creation error " << std::endl;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported " << std::endl;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed " << std::endl;
    } else {
        std::cout << "Connected" << std::endl;
    }
}

void Client::sendmsg() {
    if (flag == 0) {
        std::cout << entermsg << std::endl;
        send(sock, entermsg, strlen(entermsg), 0);
    } else {
        std::cout << exitmsg << std::endl;
        send(sock, exitmsg, strlen(exitmsg), 0);
    }
}

bool Client::sigiden() {
    if (strcmp(rdmsg, "RED") == 0) {
        sig = 0;
    } else if (strcmp(rdmsg, "GREEN") == 0) {
        sig = 1;
    }
    return sig;
}

void Client::readsig() {
    memset(rdmsg, 0, 2);
    bytesRead = read(sock, rdmsg, 2);
    read(sock, rdmsg, 2);
    std::cout << rdmsg << std::endl;
    sigiden();
    if (bytesRead < 0) {
        std::cerr << "Error reading from socket" << std::endl;
        return;
    } else if (bytesRead == 0) {
        std::cerr << "Connection closed by peer" << std::endl;
        return;
    }
}
