#include "pigpio.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "CMFRC522.h"
#include "pca9685.h"
#include "motor.h"
#include "linetracking.h"
#include "ultrasonic.h"
#include "stdio.h"
#include "client.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <thread>
#include <sstream>
#include <mutex>
#include <atomic>
#include <vector>


using namespace std;


atomic<bool> running(true);
atomic<bool> obstacleDetected(false);
mutex motorMutex;


const int PCA9685_ADDR = 0x40;
const int TRIG_PIN = 27;
const int ECHO_PIN = 22;


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
    Client& clie;
	unsigned char get_id[16];
    int enter_id = 180 ;
    int exit_id = 189 ;

public:
    void run()  {
         CMFRC522 mfrc522;
         mfrc522.PCD_Init();
	     while(1) { 
        //std::cout << "Detecting..." << std::endl;
        if (mfrc522.isNewCardPresent()==0) { // Detect whether a new card is present
            //std::cout << "Card Detected/n The card id is:" << std::endl;
            mfrc522.RC522_Anticoll(mfrc522._id);
            for (uint8_t i = 0; i < 8; i++) {
                get_id[i]= mfrc522._id[i];
                //std::cout<<std::hex<<int(mfrc522._id[i])<<" ";
            }


        if(get_id[2]==enter_id)
        {
            clie.flag = 0;
            
            //std::cout<<"Entered Crossing!!"<<std::endl;
        }else if(get_id[2]==exit_id)
        {
            clie.flag = 1;
            //std::cout<<"Exited Crossing!!"<<std::endl;

        }
        clie.sendmsg();
        //std::this_thread::sleep_for(200ms);
        }
         }
	}

RFIDThread(Client& client) : clie(client){}
    

};


class Readingmsg: public CppThread {
    private:
    Client& clie;

    public:
    void run()  {
        while(1)
        {  
            if(clie.byte_rd()!=0)
            {
            //std::cout<<"already Read!!"<<std::endl;
            clie.readsig();
            gpioDelay(500000);

            }
            
        }
    }
    
    Readingmsg(Client& Clie):clie(Clie){};
};



class BuzzCtrl: public CppThread {
    private:
    Client& clie;
    

    public:

    void run() {
        while(1){
            
            gpioSetMode(17,PI_OUTPUT);
            if(strcmp(clie.rdmsg,"R")==0)
            {
             std::cout<<"Car Stopped!!!"<<std::endl;
             //std::lock_guard<std::mutex> lock(Motioncntrl);
             gpioPWM(17, 128);
            } else if (clie.flag == 1)
            {
            
            gpioPWM(17, 0);
            std::cout << "no buzzzzzzzzzz\n";
            } else
            {
            std::cout<<"Car Running"<<std::endl;
            gpioPWM(17, 0);
            std::cout << "no buzzzzzzzzzz\n";
            }

            
            
        }

    }

    BuzzCtrl(Client& Clie):clie(Clie){};
};


int main(){
    const char host []= "192.168.1.132";
    int port = 5560;

    if (gpioInitialise() < 0) {
        std::cerr << "Pigpio initialisation failed." << std::endl;
        return 1;
    }

    int i2cHandle = i2cOpen(1, PCA9685_ADDR, 0);

    if (i2cHandle < 0) {
        std::cerr << "Failed to open I2C communication." << std::endl;
        return 1;
    }

    PCA9685 pca(i2cHandle);
    Motor motor(pca);
    LineTracking linetracking(motor);
    Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN, pca);
    Client client;
    RFIDThread rfidrd(client);
    Readingmsg rdingmsg(client);
    BuzzCtrl bzcl(client);
    

    pca.setup();

    client.setupSocket(host, port);

    if (client.sock < 0) return -1;
    
    rfidrd.start();
    rdingmsg.start();
    bzcl.start();

    thread lineThread([&] {
        while (running) {
            linetracking.detectLine();
            gpioDelay(100000);
        }
    });

    while (running) {
        float distance = ultrasonic.getDistance();

        if (distance < 20 && distance != -1) {
            obstacleDetected = true;
            motor.Stop();
        } else if (obstacleDetected){
            obstacleDetected=false;
            linetracking.detectLine();
        } else {
            std::lock_guard<std::mutex> lock(motorMutex);
            linetracking.detectLine();
        }
        
        
    }

    rfidrd.join();
    rdingmsg.join();
    lineThread.join();
    bzcl.join();
    gpioTerminate();
    return 0;

}
