#include "client.h"
#include <iostream>

void Client::scanCard(const std::string& cardID) {
    std::cout << "Client::scanCard called with cardID: " << cardID << std::endl;  // Debug statement

    if (server_ == nullptr) {
        std::cerr << "Error: server_ is null!" << std::endl;
        return;
    }

    std::cout << "Client::scanCard server_ is valid, updating server with cardID: " << cardID << std::endl;

    try {
        server_->updateServerWithCardID(cardID);
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in scanCard: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught in scanCard" << std::endl;
    }

    std::cout << "Client::scanCard completed" << std::endl;
}

void Client::sendmsg() {
    std::string msg = (flag == 0) ? "Entered" : "Exited";

    if (server_ == nullptr) {
        std::cerr << "Error: server_ is null!" << std::endl;
        return;
    }

    std::cout << "Client::sendmsg sending message: " << msg << std::endl;

    try {
        server_->updateServerWithCardID(msg);
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in sendmsg: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught in sendmsg" << std::endl;
    }

    std::cout << "Client::sendmsg completed" << std::endl;
}
