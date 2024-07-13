#include "ultrasonic.h"
#include "pigpio.h"

Ultrasonic::Ultrasonic(int triggerPin, int echoPin, PCA9685& controller) : trigPin(triggerPin), echoPin(echoPin), pca(controller) {
    gpioSetMode(trigPin, PI_OUTPUT);
    gpioSetMode(echoPin, PI_INPUT);
    gpioWrite(trigPin, PI_LOW);
    gpioSetAlertFuncEx(echoPin, echoCallback, this);
}

void Ultrasonic::triggerPulse() {
    gpioWrite(trigPin, PI_HIGH);
    gpioDelay(10);  // Use gpioDelay for accurate short delay
    gpioWrite(trigPin, PI_LOW);
}

void Ultrasonic::echoCallback(int gpio, int level, uint32_t tick, void *user1) {
    static uint32_t startTick, endTick;

    if (level == 1) startTick = tick;
    else if (level == 0) {
        endTick = tick;
        auto distance = ((endTick - startTick) / 1000000.0) * SOUND_SPEED / 2.0;
        ((Ultrasonic*)user1)->lastDistance.store(distance);
    }
}

float Ultrasonic::getDistance() {
    triggerPulse();
    return lastDistance.load();
}
