#include "client.h"
#include <iostream>

// Implementation of Client::scanCard
void Client::scanCard(const std::string& cardID) {
    std::cout << "Client::scanCard called with cardID: " << cardID << std::endl;  // Debug statement

    // Check if server_ is valid
    if (server_ == nullptr) {
        std::cerr << "Error: server_ is null!" << std::endl;
        return;
    }

    std::cout << "Client::scanCard server_ is valid, updating server with cardID: " << cardID << std::endl;

    // Try to update the server with the card ID and handle potential exceptions
    try {
        server_->updateServerWithCardID(cardID);
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in scanCard: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught in scanCard" << std::endl;
    }

    std::cout << "Client::scanCard completed" << std::endl;
}

// Implementation of Client::sendmsg
void Client::sendmsg() {
    // Determine the message to send based on the flag
    std::string msg = (flag == 0) ? "Entered" : "Exited";

    // Check if server_ is valid
    if (server_ == nullptr) {
        std::cerr << "Error: server_ is null!" << std::endl;
        return;
    }

    std::cout << "Client::sendmsg sending message: " << msg << std::endl;

    // Try to update the server with the message and handle potential exceptions
    try {
        server_->updateServerWithCardID(msg);
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in sendmsg: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught in sendmsg" << std::endl;
    }

    std::cout << "Client::sendmsg completed" << std::endl;
}
