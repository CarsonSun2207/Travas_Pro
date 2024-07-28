#ifndef MOTOR_H
#define MOTOR_H

#include "pca9685.h"

// Motor class definition to control motor movements using PCA9685 servo driver
class Motor {
private:
    PCA9685& pca;  // Reference to a PCA9685 object for controlling PWM signals

public:
    // Constructor that takes a reference to a PCA9685 controller object
    Motor(PCA9685& pcaController);

    // Public member functions to control the motor
    void Forward();   // Move the motor forward
    void TurnLeft();  // Turn the motor left
    void TurnRight(); // Turn the motor right
    void Stop();      // Stop the motor
    void Slow();      // Slow down the motor
};

#endif // MOTOR_H
