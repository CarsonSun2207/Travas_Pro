#ifndef PCA9685_H
#define PCA9685_H

#include "pigpio.h"

// Class to interface with the PCA9685 PWM driver
class PCA9685 {
private:
    int i2cHandle;  // Handle for I2C communication

public:
    // Constructor that takes an I2C handle
    PCA9685(int handle);

    // Method to set up the PCA9685
    void setup();

    // Method to set the PWM signal for a specific channel
    void setPWM(int channel, int on, int off);
};

#endif // PCA9685_H
