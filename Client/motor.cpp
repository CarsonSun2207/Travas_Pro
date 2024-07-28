#include "motor.h"

// Constructor that initializes the Motor object with a reference to a PCA9685 controller
Motor::Motor(PCA9685& pcaController) : pca(pcaController) {}

// Method to move the motor forward
void Motor::Forward() {
    // Set PWM signals for forward movement on all motor channels
    pca.setPWM(0, 0, 0);
    pca.setPWM(1, 0, 850);
    pca.setPWM(3, 0, 0);
    pca.setPWM(2, 0, 850);
    pca.setPWM(4, 0, 0);
    pca.setPWM(5, 0, 850);
    pca.setPWM(6, 0, 0);
    pca.setPWM(7, 0, 850);
}

// Method to turn the motor left
void Motor::TurnLeft() {
    // Set PWM signals for left turn on all motor channels
    pca.setPWM(0, 1024, 0);
    pca.setPWM(1, 0, 0);
    pca.setPWM(3, 1024, 0);
    pca.setPWM(2, 0, 0);
    pca.setPWM(4, 0, 0);
    pca.setPWM(5, 0, 1250);
    pca.setPWM(6, 0, 0);
    pca.setPWM(7, 0, 1250);
}

// Method to turn the motor right
void Motor::TurnRight() {
    // Set PWM signals for right turn on all motor channels
    pca.setPWM(0, 0, 0);
    pca.setPWM(1, 0, 1250);
    pca.setPWM(3, 0, 0);
    pca.setPWM(2, 0, 1250);
    pca.setPWM(4, 1024, 0);
    pca.setPWM(5, 0, 0);
    pca.setPWM(6, 1024, 0);
    pca.setPWM(7, 0, 0);
}

// Method to stop the motor
void Motor::Stop() {
    // Set PWM signals to 0 for stopping all motor channels
    pca.setPWM(0, 0, 0);
    pca.setPWM(1, 0, 0);
    pca.setPWM(3, 0, 0);
    pca.setPWM(2, 0, 0);
    pca.setPWM(4, 0, 0);
    pca.setPWM(5, 0, 0);
    pca.setPWM(6, 0, 0);
    pca.setPWM(7, 0, 0);
}

// Method to slow down the motor
void Motor::Slow() {
    // Set PWM signals for slower movement on all motor channels
    pca.setPWM(0, 0, 0);
    pca.setPWM(1, 0, 700);
    pca.setPWM(3, 0, 0);
    pca.setPWM(2, 0, 700);
    pca.setPWM(4, 0, 0);
    pca.setPWM(5, 0, 700);
    pca.setPWM(6, 0, 0);
    pca.setPWM(7, 0, 700);
}
