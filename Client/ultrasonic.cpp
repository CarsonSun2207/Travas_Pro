#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pca9685.h"
#include <atomic>

// Speed of sound in cm/s
const float SOUND_SPEED = 34300;

// Ultrasonic class to handle ultrasonic sensor operations
class Ultrasonic {
private:
    int trigPin, echoPin;          // GPIO pins for the trigger and echo
    PCA9685& pca;                  // Reference to the PCA9685 controller
    std::atomic<float> lastDistance{0.0}; // Atomic variable to store the last measured distance

    // Method to trigger the ultrasonic pulse
    void triggerPulse();
    // Static callback function to handle the echo response
    static void echoCallback(int gpio, int level, uint32_t tick, void *user1);

public:
    // Constructor to initialize the Ultrasonic object with trigger and echo pins and the PCA9685 controller
    Ultrasonic(int triggerPin, int echoPin, PCA9685& controller);
    
    // Method to get the measured distance
    float getDistance();
};

#endif // ULTRASONIC_H
