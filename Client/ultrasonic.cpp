#include "ultrasonic.h"
#include "pigpio.h"

// Constructor that initializes the Ultrasonic object with the trigger and echo pins and the PCA9685 controller
Ultrasonic::Ultrasonic(int triggerPin, int echoPin, PCA9685& controller)
    : trigPin(triggerPin), echoPin(echoPin), pca(controller) {
    // Set the trigger pin as output
    gpioSetMode(trigPin, PI_OUTPUT);
    // Set the echo pin as input
    gpioSetMode(echoPin, PI_INPUT);
    // Initialize the trigger pin to low
    gpioWrite(trigPin, PI_LOW);
    // Set the echo callback function
    gpioSetAlertFuncEx(echoPin, echoCallback, this);
}

// Method to trigger the ultrasonic pulse
void Ultrasonic::triggerPulse() {
    gpioWrite(trigPin, PI_HIGH); // Set the trigger pin high
    gpioWrite(trigPin, PI_LOW);  // Immediately set the trigger pin low
}

// Static callback function to handle the echo response
void Ultrasonic::echoCallback(int gpio, int level, uint32_t tick, void *user1) {
    static uint32_t startTick, endTick;

    // If echo pin goes high, record the start tick
    if (level == 1) {
        startTick = tick;
    }
    // If echo pin goes low, record the end tick and calculate distance
    else if (level == 0) {
        endTick = tick;
        auto distance = ((endTick - startTick) / 1000000.0) * SOUND_SPEED / 2.0;
        // Store the calculated distance in the lastDistance atomic variable
        ((Ultrasonic*)user1)->lastDistance.store(distance);
    }
}

// Method to get the measured distance
float Ultrasonic::getDistance() {
    triggerPulse();  // Trigger a pulse to start measurement
    return lastDistance.load();  // Return the last measured distance
}
