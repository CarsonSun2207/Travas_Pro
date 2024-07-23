#include "pca9685.h"

PCA9685::PCA9685(int handle) : i2cHandle(handle) {}

void PCA9685::setup() {
    i2cWriteByteData(i2cHandle, 0x00, 0x20);  // Enter sleep mode
    int freq = 1000;  // 1 kHz
    int prescale = static_cast<int>((25000000.0 / (4096 * freq)) - 1);
    i2cWriteByteData(i2cHandle, 0xFE, prescale);  // Set prescale for PWM frequency
    i2cWriteByteData(i2cHandle, 0x00, 0x01);  // Restart
    i2cWriteByteData(i2cHandle, 0x00, 0x80);  // Finish reset
}

void PCA9685::setPWM(int channel, int on, int off) {
    int regBase = 0x06 + 4 * channel;
    i2cWriteByteData(i2cHandle, regBase, on & 0xFF);
    i2cWriteByteData(i2cHandle, regBase + 1, on >> 8);
    i2cWriteByteData(i2cHandle, regBase + 2, off & 0xFF);
    i2cWriteByteData(i2cHandle, regBase + 3, off >> 8);
}
