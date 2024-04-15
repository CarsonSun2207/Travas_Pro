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

unsigned char get_id[16];
int enter_id = 68 ;
int exit_id = 189 ;
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
	// pointer to the thread
	std::thread uthread;
};

class RFIDThread : public CppThread {

private:
	// implement run which is doing all the work
	void run() {
         CMFRC522 mfrc522;
         mfrc522.PCD_Init();
	     while(1) { 
        std::cout << "Detecting..." << std::endl;
        if (mfrc522.isNewCardPresent()==0) { // Detect whether a new card is present
            std::cout << "New card is here..." << std::endl;
            mfrc522.RC522_Anticoll(mfrc522._id);
            for (uint8_t i = 0; i < 8; i++) {
                get_id[i]= mfrc522._id[i];
                std::cout<<std::hex<<int(mfrc522._id[i])<<" ";
            }
          
        }
        std::this_thread::sleep_for(500ms);
    }
	}
};


};

int main(int argc, char *argv[]) {
    gpioInitialise();
	
    // Initializes PCD
   RFIDThread rfidthd;
   rfidthd.start();
   
   rfidthd.join();
   
}

