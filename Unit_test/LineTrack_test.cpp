#include "pigpio.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

// Constants
const int PCA9685_ADDR = 0x40;

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
        pca.setPWM(1, 0, 800);
        pca.setPWM(3, 0, 0);
        pca.setPWM(2, 0, 800);
        pca.setPWM(4, 0, 0);
        pca.setPWM(5, 0, 800);
        pca.setPWM(6, 0, 0);
        pca.setPWM(7, 0, 800);
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
        pca.setPWM(1, 0, 800);
        pca.setPWM(3, 0, 0);
        pca.setPWM(2, 0, 800);
        pca.setPWM(4, 0, 0);
        pca.setPWM(5, 0, 800);
        pca.setPWM(6, 0, 0);
        pca.setPWM(7, 0, 800);
    }
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
                motor.Slow(); 
                break;
            default:
                motor.Forward(); 
                break;
        }
    }

};

int main (){
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
    pca.setup();

    Motor motor(pca);
    LineTracking linetracking(motor);

    while (true) {
        linetracking.detectLine(); // Resume line tracking
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    gpioTerminate();
    return 0;

}

