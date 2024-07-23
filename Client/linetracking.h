#ifndef LINETRACKING_H
#define LINETRACKING_H

#include "motor.h"
#include <atomic>

class LineTracking {
private:
    int IR01 = 14;  // GPIO pin numbers
    int IR02 = 15;
    int IR03 = 23;
    Motor& motor;

public:
    LineTracking(Motor& motorController);
    void detectLine();
};

#endif // LINETRACKING_H
