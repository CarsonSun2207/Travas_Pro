#include "linetracking.h"
#include "pigpio.h"

// External atomic boolean to check for obstacle detection
extern std::atomic<bool> obstacleDetected;

// Constructor for LineTracking class
// Initializes the motor controller and sets up the GPIO modes for the IR sensors
LineTracking::LineTracking(Motor& motorController) : motor(motorController) {
    gpioSetMode(IR01, PI_INPUT); // Set IR01 as input
    gpioSetMode(IR02, PI_INPUT); // Set IR02 as input
    gpioSetMode(IR03, PI_INPUT); // Set IR03 as input
}

// Function to detect the line and control the motor based on IR sensor readings
void LineTracking::detectLine() {
    // Skip line tracking if an obstacle is detected
    if (obstacleDetected.load()) {
        return; 
    }

    int LMR = 0x00; // Variable to store the state of the IR sensors

    // Read the state of the IR sensors and update the LMR variable
    if (gpioRead(IR01) == PI_HIGH) LMR |= 0b100; // Set the left bit if IR01 is high
    if (gpioRead(IR02) == PI_HIGH) LMR |= 0b010; // Set the middle bit if IR02 is high
    if (gpioRead(IR03) == PI_HIGH) LMR |= 0b001; // Set the right bit if IR03 is high

    // Switch statement to control the motor based on the IR sensor readings
    switch (LMR) {
        case 0b010: // Only the middle sensor is high
            motor.Forward(); // Move forward
            break;
        case 0b100: // Only the left sensor is high
            motor.TurnLeft(); // Turn left
            break;
        case 0b001: // Only the right sensor is high
            motor.TurnRight(); // Turn right
            break;
        case 0b000: // No sensor is high
            motor.Slow(); // Slow down
            break;
        default: // Multiple sensors are high
            motor.Forward(); // Move forward
            break;
    }
}
