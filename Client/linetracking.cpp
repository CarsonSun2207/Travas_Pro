#include "linetracking.h"
#include "pigpio.h"

extern std::atomic<bool> obstacleDetected;

LineTracking::LineTracking(Motor& motorController) : motor(motorController) {
    gpioSetMode(IR01, PI_INPUT);
    gpioSetMode(IR02, PI_INPUT);
    gpioSetMode(IR03, PI_INPUT);
}

void LineTracking::detectLine() {
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
