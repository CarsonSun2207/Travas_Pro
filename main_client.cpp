#include "pigpio.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "CMFRC522.h"
#include "stdio.h"

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
const float SOUND_SPEED = 34300;  // Speed of sound in cm/s at room temperature


class PCA9685 {
private:
    int i2cHandle;

public:
    PCA9685(int handle) : i2cHandle(handle) {}

    void setup() {
        i2cWriteByteData(i2cHandle, 0x00, 0x20);  // Enter sleep mode
        int freq = 1000;  // 1 kHz
        int prescale = static_cast<int>((25000000.0 / (4096 * freq)) - 1);
        i2cWriteByteData(i2cHandle, 0xFE, prescale);  // Set prescale for PWM frequency
        i2cWriteByteData(i2cHandle, 0x00, 0x01);  // Restart
        i2cWriteByteData(i2cHandle, 0x00, 0x80);  // Finish reset
    }

    void setPWM(int channel, int on, int off) {
        int regBase = 0x06 + 4 * channel;
        i2cWriteByteData(i2cHandle, regBase, on & 0xFF);
        i2cWriteByteData(i2cHandle, regBase + 1, on >> 8);
        i2cWriteByteData(i2cHandle, regBase + 2, off & 0xFF);
        i2cWriteByteData(i2cHandle, regBase + 3, off >> 8);
    }
};


class Motor {
private:
    PCA9685& pca;

public:
    Motor(PCA9685& pcaController) : pca(pcaController) {}

    void Forward() {
        pca.setPWM(0, 0, 0);
        pca.setPWM(1, 0, 850);
        pca.setPWM(3, 0, 0);
        pca.setPWM(2, 0, 850);
        pca.setPWM(4, 0, 0);
        pca.setPWM(5, 0, 850);
        pca.setPWM(6, 0, 0);
        pca.setPWM(7, 0, 850);
    }

    void TurnLeft() {
        pca.setPWM(0, 1024, 0);
        pca.setPWM(1, 0, 0);
        pca.setPWM(3, 1024, 0);
        pca.setPWM(2, 0, 0);
        pca.setPWM(4, 0, 0);
        pca.setPWM(5, 0, 1250);
        pca.setPWM(6, 0, 0);
        pca.setPWM(7, 0, 1250);
    }

    void TurnRight() {
        pca.setPWM(0, 0, 0);
        pca.setPWM(1, 0, 1250);
        pca.setPWM(3, 0, 0);
        pca.setPWM(2, 0, 1250);
        pca.setPWM(4, 1024, 0);
        pca.setPWM(5, 0, 0);
        pca.setPWM(6, 1024, 0);
        pca.setPWM(7, 0, 0);
    }

    void Stop() {
        pca.setPWM(0, 0, 0);
        pca.setPWM(1, 0, 0);
        pca.setPWM(3, 0, 0);
        pca.setPWM(2, 0, 0);
        pca.setPWM(4, 0, 0);
        pca.setPWM(5, 0, 0);
        pca.setPWM(6, 0, 0);
        pca.setPWM(7, 0, 0);
    }

    void Slow() {
        pca.setPWM(0, 0, 0);
        pca.setPWM(1, 0, 700);
        pca.setPWM(3, 0, 0);
        pca.setPWM(2, 0, 700);
        pca.setPWM(4, 0, 0);
        pca.setPWM(5, 0, 700);
        pca.setPWM(6, 0, 0);
        pca.setPWM(7, 0, 700);
    }
};


class Ultrasonic {
private:
    int trigPin, echoPin;
    PCA9685& pca;
    atomic<float> lastDistance{0.0};

    void triggerPulse() {
        gpioWrite(trigPin, PI_HIGH);
        this_thread::sleep_for(chrono::microseconds(10));
        gpioWrite(trigPin, PI_LOW);
    }

    static void echoCallback(int gpio, int level, uint32_t tick, void *user1) {
        static uint32_t startTick, endTick;

        if (level == 1) startTick = tick;
        else if (level == 0) {
            endTick = tick;
            auto distance = ((endTick - startTick) / 1000000.0) * SOUND_SPEED / 2.0;
            ((Ultrasonic*)user1)->lastDistance.store(distance);
        }
    }

public:
    Ultrasonic(int triggerPin, int echoPin, PCA9685& controller) : trigPin(triggerPin), echoPin(echoPin), pca(controller) {
        gpioSetMode(trigPin, PI_OUTPUT);
        gpioSetMode(echoPin, PI_INPUT);
        gpioWrite(trigPin, PI_LOW);
        gpioSetAlertFuncEx(echoPin, echoCallback, this);
    }

    float getDistance() {
        triggerPulse();
        return lastDistance.load();
    }
};


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


class Client{
    private:
        bool sig=1;
        const char entermsg[9]="Entered";
        const char exitmsg[7]="Exited";
    
    public:

    char* rdmsg =new char {2};
    int sock;
    bool flag;
    int bytesRead;
    int byte_rd(){ return read(sock,rdmsg,5);};   
 

    void setupSocket(const std::string& host, int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    //std::cout<<"I am here in setupsocket"<<std::endl;
    if (sock < 0) {
        std::cout << "Socket creation error "<<std::endl;
         
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported "<<std::endl;
        
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed "<<std::endl;
    
    }else if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) > 0)
    {
        std::cout<<"Connected"<<std::endl;
    }
    
    }

    void sendmsg() {
        if (flag==0)
        {
            std::cout<<entermsg<<std::endl;
            send(sock, entermsg,strlen(entermsg), 0);
        }else{
            std::cout<<exitmsg<<std::endl;
            send(sock, exitmsg,strlen(exitmsg), 0);
        }
        //std::cout<<"Notified Server!!"<<std::endl;

    }
     bool sigiden() {
        if(strcmp(rdmsg,"RED")==0)
        {
            sig=0;
            //std::cout<<"The current boolean signal is:"<<sig<<std::endl;
        }
        else if (strcmp(rdmsg,"GREEN")==0)
        {
           sig=1;
            //std::cout<<"The current boolean signal is:"<<sig<<std::endl;
        }
        
            
    return sig;           
    }

    void readsig(){
        
        //std::cout<<"Reading data..."<<std::endl;
        memset(rdmsg, 0, 2);
        bytesRead = read(sock,rdmsg,2);
        read(sock,rdmsg,2);
        std::cout<<rdmsg<<std::endl;
        sigiden();
        if (bytesRead < 0) {
        // Error handling
            std::cerr << "Error reading from socket" << std::endl;
            delete[] rdmsg; // Free allocated memory
            return ;
        } else if (bytesRead == 0) {
        // Connection closed
            std::cerr << "Connection closed by peer" << std::endl;
            delete[] rdmsg; // Free allocated memory
            return ;
        } else {
        // Read successful
            //std::cout<<rdmsg<<std::endl;
            //sigiden();
        }
    
    }
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
            std::this_thread::sleep_for(500ms);
            }
            
        }
    }
    
    Readingmsg(Client& Clie):clie(Clie){};
};


class LineTracking {
private:
    int IR01 = 14;  // GPIO pin numbers
    int IR02 = 15;
    int IR03 = 23;
    Motor& motor;

public:
    LineTracking(Motor& motorController) : motor(motorController) {
        gpioSetMode(14, PI_INPUT);
        gpioSetMode(15, PI_INPUT);
        gpioSetMode(23, PI_INPUT);
    }

    void detectLine() {
        if (obstacleDetected.load()) {
            return; // Skip line tracking if an obstacle is detected
        } 
        int LMR = 0x00;
        if (gpioRead(IR01) == PI_HIGH) LMR |= 0b100;
        if (gpioRead(IR02) == PI_HIGH) LMR |= 0b010;
        if (gpioRead(IR03) == PI_HIGH) LMR |= 0b001;

        switch (LMR) {
            case 0b010:
                motor.Forward();
                break;
            case 0b100:
                motor.TurnLeft();
                break;
            case 0b001:
                motor.TurnRight();
                break;
            case 0b000:
                motor.Slow(); // Consider changing this to a search pattern instead of just slowing
                break;
            default:
                motor.Forward(); // Default action can be modified based on desired behavior
                break;
        }
    }

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
            this_thread::sleep_for(chrono::milliseconds(100));
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
        
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    rfidrd.join();
    rdingmsg.join();
    lineThread.join();
    bzcl.join();
    gpioTerminate();
    return 0;

}

