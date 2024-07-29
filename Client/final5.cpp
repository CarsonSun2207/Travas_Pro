// Include necessary libraries for GPIO, socket communication, I2C, and various modules
#include "pigpio.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "CMFRC522.h"
#include "pca9685.h"
#include "motor.h"
#include "linetracking.h"
#include "ultrasonic.h"
#include "stdio.h"
#include "client.h"
#include "CppThread.h"
#include "RFIDThread.h"
#include "Readmsg.h"
#include "BuzzCtrl.h"

// Standard C++ libraries for I/O operations, threading, synchronization, and data structures
#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <thread>
#include <sstream>
#include <mutex>
#include <atomic>
#include <vector>

using namespace std;

// Atomic boolean flags for controlling the main loop and obstacle detection status
atomic<bool> running(true);
atomic<bool> obstacleDetected(false);

// Mutex for synchronizing motor operations
mutex motorMutex;

// I2C address for the PCA9685 servo driver and GPIO pins for the ultrasonic sensor
const int PCA9685_ADDR = 0x40;
const int TRIG_PIN = 27;
const int ECHO_PIN = 22;
const int timer_id = 0;
volatile int t;

int main(){
    // IP address and port number for the client socket connection
    const char host []= "192.168.1.132";
    int port = 5560;

    // Initialize the GPIO library
    if (gpioInitialise() < 0) {
        std::cerr << "Pigpio initialisation failed." << std::endl;
        return 1;
    }

    // Open I2C communication
    int i2cHandle = i2cOpen(1, PCA9685_ADDR, 0);
    if (i2cHandle < 0) {
        std::cerr << "Failed to open I2C communication." << std::endl;
        return 1;
    }

    // Initialize various components and modules
    PCA9685 pca(i2cHandle);
    Motor motor(pca);
    LineTracking linetracking(motor);
    Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN, pca);
    Client client;
    RFIDThread rfidrd(client);
    Readingmsg rdingmsg(client);
    BuzzCtrl bzcl(client);

    // Set up the PCA9685 and client socket
    pca.setup();
    client.setupSocket(host, port);

    // Check if the socket setup was successful
    if (client.sock < 0) return -1;

    // Start threads for RFID reading, message reading, and buzzer control
    rfidrd.start();
    rdingmsg.start();
    bzcl.start();

    // Thread for line tracking
    thread lineThread([&] {
        while (running) {
            linetracking.detectLine();
        }
    });

    // Main loop for obstacle detection and line tracking
    while (running) {
        float distance = ultrasonic.getDistance();

        // If an obstacle is detected, stop the motor
        if (distance < 20 && distance != -1) {
            obstacleDetected = true;
            motor.Stop();
        } else if (obstacleDetected){
            // If obstacle is no longer detected, resume line tracking
            obstacleDetected=false;
            linetracking.detectLine();
        } else {
            // Lock motor operations and perform line tracking
            std::lock_guard<std::mutex> lock(motorMutex);
            linetracking.detectLine();
        }
    }

    // Join all threads before exiting
    rfidrd.join();
    rdingmsg.join();
    lineThread.join();
    bzcl.join();

    // Terminate GPIO library
    gpioTerminate();
    return 0;
}
