#include "pigpio.h"
#include "stdio.h"
#include "stdlib.h"

#include <thread>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <algorithm>
#include <chrono>

using namespace std;





void initialise() {
    if (gpioInitialise() < 0) {
        std::cerr << "Pigpio initialization failed." << std::endl;
        exit(1);
    }
}


int main()
{
    initialise();
    gpioSetMode(17, PI_OUTPUT);

    int command = 0;
    while (true) {
        std::cout << "Please input number to control:\n";
        std::cin >> command;

        switch (command) {
            case 1: {
                gpioPWM(17, 128);
                std::cout << "buzzzzzzzzzz\n";
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 2 seconds
                break;
            }
            case 2: {
                gpioPWM(17, 0);
                std::cout << "Silence\n";
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 2 seconds
                break;
            }
            case 0: {
                // Special case to stop the loop and terminate the program
                gpioTerminate(); // Cleanup pigpio
                return 0; // Exit the program
            }
            default:
                std::cerr << "Invalid command" << std::endl;
                break;
        }
    }

    return 0;
}
