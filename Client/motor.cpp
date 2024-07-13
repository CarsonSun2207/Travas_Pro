#include "motor.h"

Motor::Motor(PCA9685& pcaController) : pca(pcaController) {}

void Motor::Forward() {
    pca.setPWM(0, 0, 0);
    pca.setPWM(1, 0, 850);
    pca.setPWM(3, 0, 0);
    pca.setPWM(2, 0, 850);
    pca.setPWM(4, 0, 0);
    pca.setPWM(5, 0, 850);
    pca.setPWM(6, 0, 0);
    pca.setPWM(7, 0, 850);
}

void Motor::TurnLeft() {
    pca.setPWM(0, 1024, 0);
    pca.setPWM(1, 0, 0);
    pca.setPWM(3, 1024, 0);
    pca.setPWM(2, 0, 0);
    pca.setPWM(4, 0, 0);
    pca.setPWM(5, 0, 1250);
    pca.setPWM(6, 0, 0);
    pca.setPWM(7, 0, 1250);
}

void Motor::TurnRight() {
    pca.setPWM(0, 0, 0);
    pca.setPWM(1, 0, 1250);
    pca.setPWM(3, 0, 0);
    pca.setPWM(2, 0, 1250);
    pca.setPWM(4, 1024, 0);
    pca.setPWM(5, 0, 0);
    pca.setPWM(6, 1024, 0);
    pca.setPWM(7, 0, 0);
}

void Motor::Stop() {
    pca.setPWM(0, 0, 0);
    pca.setPWM(1, 0, 0);
    pca.setPWM(3, 0, 0);
    pca.setPWM(2, 0, 0);
    pca.setPWM(4, 0, 0);
    pca.setPWM(5, 0, 0);
    pca.setPWM(6, 0, 0);
    pca.setPWM(7, 0, 0);
}

void Motor::Slow() {
    pca.setPWM(0, 0, 0);
    pca.setPWM(1, 0, 700);
    pca.setPWM(3, 0, 0);
    pca.setPWM(2, 0, 700);
    pca.setPWM(4, 0, 0);
    pca.setPWM(5, 0, 700);
    pca.setPWM(6, 0, 0);
    pca.setPWM(7, 0, 700);
}
