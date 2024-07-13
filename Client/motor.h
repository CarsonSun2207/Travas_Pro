#ifndef MOTOR_H
#define MOTOR_H

#include "pca9685.h"

class Motor {
private:
    PCA9685& pca;

public:
    Motor(PCA9685& pcaController);
    void Forward();
    void TurnLeft();
    void TurnRight();
    void Stop();
    void Slow();
};

#endif // MOTOR_H
