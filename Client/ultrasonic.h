#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pca9685.h"
#include <atomic>

const float SOUND_SPEED = 34300;

class Ultrasonic {
private:
    int trigPin, echoPin;
    PCA9685& pca;
    std::atomic<float> lastDistance{0.0};
    
    void triggerPulse();
    static void echoCallback(int gpio, int level, uint32_t tick, void *user1);

public:
    
    Ultrasonic(int triggerPin, int echoPin, PCA9685& controller);
    float getDistance();
};

#endif // ULTRASONIC_H
