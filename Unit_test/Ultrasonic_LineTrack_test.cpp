#include "pigpio.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

using namespace std;

// Global control flags and mutex
atomic<bool> running(true);
atomic<bool> obstacleDetected(false);
mutex motorMutex;

// Constants
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

    static void echoCallback(int gpio, int level, uint32_t tick, void *user) {
        static uint32_t startTick, endTick;

        if (level == 1) startTick = tick;
        else if (level == 0) {
            endTick = tick;
            auto distance = ((endTick - startTick) / 1000000.0) * SOUND_SPEED / 2.0;
            ((Ultrasonic*)user)->lastDistance.store(distance);
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
    Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN, pca);

    thread lineThread([&] {
        while (running) {
            linetracking.detectLine();
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    });

    while (running) {
        float distance = ultrasonic.getDistance();
        cout << "Distance: " << distance << " cm" << endl;

        if (distance < 20 && distance != -1) {
            obstacleDetected = true;
            motor.Stop();
        } else if (obstacleDetected) {
            obstacleDetected = false;
            linetracking.detectLine(); // Resume line tracking
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    lineThread.join();
    gpioTerminate();
    return 0;

}
