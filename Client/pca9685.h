#ifndef PCA9685_H
#define PCA9685_H

#include "pigpio.h"

class PCA9685 {
private:
    int i2cHandle;

public:
    PCA9685(int handle);
    void setup();
    void setPWM(int channel, int on, int off);
};

#endif // PCA9685_H
