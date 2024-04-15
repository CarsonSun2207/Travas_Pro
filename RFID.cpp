//============================================================================
// Name        : pigpioLED.cpp
// Author      : CA ABID
// Version     : 0.2
// Copyright   : 2021
// Description : Test sample using CMFRC522 class : detect the presence of a new card
//               and print its serial number
//               To compile this program, use g++ sample1.cpp -lpigpio -lpthread
//============================================================================

#include "CMFRC522.h"
#include "pigpio.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <unistd.h>
#include <stdio.h>

using namespace std;

// Callback function to handle GPIO interrupt
void gpioInterruptHandler(int gpio, int level, uint32_t tick) {
    cout << "Interrupt detected on GPIO " << gpio << endl;
    // Add more functionality here as needed
}

class CppThread {

public:
    inline void start() {
        uthread = std::thread(&CppThread::run, this);
    }

    inline void join() {
        uthread.join();
    }

protected:
    virtual void run() = 0; 

private:
    std::thread uthread;
};

class RFIDThread : public CppThread {

private:
    void run() override {
        CMFRC522 mfrc522;
        mfrc522.PCD_Init();
        while(1) { 
            cout << "Detecting..." << endl;
            if (mfrc522.isNewCardPresent()==0) { // Detect whether a new card is present
                cout << "Detected New card" << endl;
                mfrc522.RC522_Anticoll(mfrc522._id);
                for (uint8_t i = 0; i < 8; i++) {
                    cout << hex << int(mfrc522._id[i]) << " ";
                }
                cout << endl;
            }
            this_thread::sleep_for(500ms);
        }
    }
};

int main(int argc, char *argv[]) {
    if (gpioInitialise() < 0) {
        cerr << "Pigpio initialization failed." << endl;
        return 1;
    }

    // Set GPIO 17 to trigger an interrupt on a rising edge
    gpioSetISRFunc(18, RISING_EDGE, 0, gpioInterruptHandler);

    RFIDThread rfidthd;
    rfidthd.start();
    rfidthd.join();

    gpioTerminate(); // Properly close pigpio
    return 0;
}
