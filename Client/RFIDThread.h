#ifndef RFIDTHREAD_H
#define RFIDTHREAD_H

#include "CppThread.h"
#include "client.h"
#include <iostream>
#include "CMFRC522.h"

// Class RFIDThread inherits from CppThread and handles RFID reading and processing
class RFIDThread : public CppThread {
private:
    Client& clie;                  // Reference to a Client object
    unsigned char get_id[16];      // Array to store the RFID card ID
    int enter_id = 180;            // ID for entering
    int exit_id = 189;             // ID for exiting

public:
    // Constructor that initializes the RFIDThread object with a reference to a Client object
    RFIDThread(Client& client) : clie(client) {}

    // Overridden run method from CppThread to perform RFID reading in a loop
    void run() override {
        CMFRC522 mfrc522;             // Initialize MFRC522 object
        mfrc522.PCD_Init();           // Initialize the RFID reader

        while (true) {
            // Check if a new card is present
            if (mfrc522.isNewCardPresent() == 0) {
                mfrc522.RC522_Anticoll(mfrc522._id);  // Perform anti-collision detection

                // Copy the detected card ID to the get_id array
                for (uint8_t i = 0; i < 8; i++) {
                    get_id[i] = mfrc522._id[i];
                }

                // Check the third byte of the ID to determine if it's an enter or exit ID
                if (get_id[2] == enter_id) {
                    clie.flag = 0;  // Set client flag to 0 for enter
                } else if (get_id[2] == exit_id) {
                    clie.flag = 1;  // Set client flag to 1 for exit
                }

                clie.sendmsg();  // Send the message to the client
            }
        }
    }
};

#endif // RFIDTHREAD_H
