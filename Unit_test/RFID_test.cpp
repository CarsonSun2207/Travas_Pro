/*
To complie this program:
1. install g++
2. Create static library by: *MAKE SURE that CMFRC522 is in the same directory with RFID_test*
    g++ -c CMFRC522.cpp
    ar rcs CMFRC522.a CMFRC522.o
3.  Complie:
    g++ RFID_test.cpp -o RFID_test -lpigpio -lrt -lpthread -L. -l:CMFRC522.a
4. Run:
    sudo ./RFID_test
*/

#include "CMFRC522.h"
#include "pigpio.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <unistd.h>
#include <stdio.h>

using namespace std;



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

class RFIDThread : public CppThread { //created a thread to implemet detection and reading

private:
    void run() override {    //detect the presence of rfid card/tag, read them and then printout them
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



    RFIDThread rfidthd;
    rfidthd.start();
    rfidthd.join();

    gpioTerminate(); // Properly close pigpio
    return 0;
}
