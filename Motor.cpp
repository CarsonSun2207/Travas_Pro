/*Author: Carson Sun and Terence Leung
  Date last modified: 12/04/2024
  Functionality: Motor control
  Version: mt14 
  Course: Realtime embedded programming
  Belongs to: Group 18 */

#include <pigpio.h>
#include <iostream>

//Global constant for I2C address on PCA9685 
const int PCA9685_ADDR = 0x40;



class PCA9685 {
private:
    int i2cHandle;

public:
    PCA9685(int handle) : i2cHandle(handle) {}

    void setup() {
        i2cWriteByteData(i2cHandle, 0x00, 0x20);
        int freq = 1000;
        int prescale = (25000000 / (4096 * freq)) - 1;
        i2cWriteByteData(i2cHandle, 0xFE, prescale); // Set PWM frequency to 1000 Hz
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
        pca.setPWM(1, 1024, 0);
        pca.setPWM(3, 0, 0);
        pca.setPWM(2, 1024, 0);
        pca.setPWM(4, 0, 0);
        pca.setPWM(5, 1024, 0);
        pca.setPWM(6, 0, 0);
        pca.setPWM(7, 1024, 0);
    }

    void Backward() {
        pca.setPWM(0, 1024, 0);
        pca.setPWM(1, 0, 0);
        pca.setPWM(3, 1024, 0);
        pca.setPWM(2, 0, 0);
        pca.setPWM(4, 1024, 0);
        pca.setPWM(5, 0, 0);
        pca.setPWM(6, 1024, 0);
        pca.setPWM(7, 0, 0);
    }

    void TurnLeft() {
        pca.setPWM(0, 0, 0);
        pca.setPWM(1, 0, 0);
        pca.setPWM(3, 0, 0);
        pca.setPWM(2, 0, 0);
        pca.setPWM(4, 0, 0);
        pca.setPWM(5, 512, 0);
        pca.setPWM(6, 0, 0);
        pca.setPWM(7, 512, 0);
    }

    void TurnRight() {
        pca.setPWM(0, 0, 0);
        pca.setPWM(1, 512, 0);
        pca.setPWM(3, 0, 0);
        pca.setPWM(2, 512, 0);
        pca.setPWM(4, 0, 0);
        pca.setPWM(5, 0, 0);
        pca.setPWM(6, 0, 0);
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
};


//main programme
int main() {
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
    int command;

    while (true) {
        std::cout << "Please input number to control:\n";
        std::cin >> command;

        switch (command) {
            case 1:
                motor.Forward();
                break;
            case 2:
                motor.Backward();
                break;
            case 3:
                motor.TurnLeft();
                break;
            case 4:
                motor.TurnRight();
                break;
            case 5:
                motor.Stop();
                break;
            default:
                std::cerr << "Invalid command" << std::endl;
                break;
        }
    }

    gpioTerminate();
    return 0;
}
