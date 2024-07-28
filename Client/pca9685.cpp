#include "pca9685.h"

// Constructor that initializes the PCA9685 object with an I2C handle
PCA9685::PCA9685(int handle) : i2cHandle(handle) {}

// Method to set up the PCA9685 PWM driver
void PCA9685::setup() {
    i2cWriteByteData(i2cHandle, 0x00, 0x20);  // Enter sleep mode
    int freq = 1000;  // Set desired PWM frequency to 1 kHz
    // Calculate the prescale value based on the desired frequency
    int prescale = static_cast<int>((25000000.0 / (4096 * freq)) - 1);
    i2cWriteByteData(i2cHandle, 0xFE, prescale);  // Set prescale for PWM frequency
    i2cWriteByteData(i2cHandle, 0x00, 0x01);  // Restart PCA9685
    i2cWriteByteData(i2cHandle, 0x00, 0x80);  // Finish reset and start PWM
}

// Method to set the PWM signal for a specific channel
void PCA9685::setPWM(int channel, int on, int off) {
    int regBase = 0x06 + 4 * channel;  // Calculate the register base for the given channel
    // Write the 'on' time to the appropriate registers
    i2cWriteByteData(i2cHandle, regBase, on & 0xFF);       // Low byte of 'on' time
    i2cWriteByteData(i2cHandle, regBase + 1, on >> 8);     // High byte of 'on' time
    // Write the 'off' time to the appropriate registers
    i2cWriteByteData(i2cHandle, regBase + 2, off & 0xFF);  // Low byte of 'off' time
    i2cWriteByteData(i2cHandle, regBase + 3, off >> 8);    // High byte of 'off' time
}
