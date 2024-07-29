# Onboard Anti-Collision and Path Navigation, and Traffic Control System - `Travas_Pro`

<p align="center">
  <img src="https://github.com/CarsonSun2207/RTEP-Project/assets/158228597/2e611b63-6fc9-4d58-8604-65ee3d71812d" width=300px />
</p>

See the full demonstration video on [YouTube](https://youtu.be/WRoeA3ryX2Q?si=PkR9EacjtkUIsWkW)

## Table of Contents
- [Motivation](https://github.com/CarsonSun2207/Travas_Pro/tree/main#motivation)
- [Hardware Requirements](https://github.com/CarsonSun2207/Travas_Pro/tree/main#hardware-requirements)
- [Software Development](https://github.com/CarsonSun2207/Travas_Pro/tree/main#software-development)
- [Unit Test](https://github.com/CarsonSun2207/Travas_Pro/tree/main#unit-test)
- [Installation](https://github.com/CarsonSun2207/Travas_Pro/tree/main#installation)

- [Social Media](https://github.com/CarsonSun2207/Travas_Pro/tree/main#social-media)
- [Contact us](https://github.com/CarsonSun2207/Travas_Pro/tree/main#contact-us)


## Motivation      
### Overview
In the near future, imagine the car as the guardian of the road and the pioneer of powerful mobility. Our latest project, Onboard Anti-Collision and Path Navigation Control, and Traffic Control System.

On a foggy morning, when the car approaches an intersection, the chip on the side of the road will send a warning signal to the car that the road ahead is dangerous, so as to remind the driver to drive carefully. When it receives a signal from the second chip, it means the car has left the road. This system helps car owners optimize their journeys, seamlessly integrating innovation with their daily lives.

But it didn't end there. If the vehicle encounters a pedestrian walking in front, the pedestrian will also keep the vehicle stationary under the detection of our ultrasonic system. This also ensures efficient and harmonious traffic flow.

### Project  Scope
1. System flexibility
    - Line tracking and obstacle avoidance in real-time
       - The integrated functionalities can be utilised to navigate through complex road conditions especially when pedestrians crossing the road ignoring the traffic light.
       - In a real-life scenario, the programme can be extended to follow a designated road path instead of line track in this project
2. Driving Safety
    - At the road entrance with a large volume of traffic, our system can greatly minimise the accident rate after optimization of traffic management, giving great security to urban traffic
3. Humanistics
    - Also, the code has performed outstanding capability to prioritise pedestrians’ lives beyond path-navigating, illustrating great care of humanistics.

## Hardware Requirements   

- Raspberry Pi 4B board ×2
- DC motor (DRV8837) ×4
- Rechargeable 18560 Bottom-top battery ×2    
- RFID Reader (RC522)
- Driver Board (PCA9685PW)
- Ultrasonic sensor (HC-SR04)
- Ultrasonic Obstacle Avoidance Module (HC-SR04)    
- Light Tracking Sensor     
- Jumper Wire F/F(4) 
- XH-2.54-5Pin Cable
- F-F Cables x n
- Prepared the necessary peripherals:    
    - HDMI display
    - USB keyboard and mouse
    - Power supply
    - Micro SD card with installed Raspbian OS System ×2


- Circuit Diagram
![circuit_2](https://github.com/CarsonSun2207/RTEP-Project/assets/158082859/ddcf9ced-58ac-46d8-aa3d-806cbf31eb5f)


- Pin Connection	
![image](https://github.com/CarsonSun2207/RTEP-Project/assets/158082859/d821acee-e734-4a32-ac53-96be38a55d33)


## Software Development   
### Onboard Raspberry Pi
Onboard Raspberry Pi acts as a node (client) connected to the server, which is mounted on the car.
- Function
    - When the car passes through the entrance of the road, the RFID reader on the car will sense the chip corresponding to the entrance, thus sending an entry signal to the server, and will receive the signal from the corresponding server to carry out the next operation. Similarly, when the car is leaving the road, the RFID reader on the car will sense the chip corresponding to the exit, thus sending the leave signal to the server.
    - Tracking sensor will make the car follow the route designed by tracking line, including forward, left turn and right turn.
    
- Libraries    
    - pigpio
    - CMFRC522
    - sys/socket
    - arpa/inet
    - `PCA9685`
        - Configuring the registers and channels to control the DC motor set.
        - Create I2C connection with the elements stated above.
    - `Linetracking`
        - Defining the line tracking logic.
        - Configure the light sensor array pins.
    - `Ultrasonic`
        - Calculating the relative distance to the obstacle.
        - Return the calculated distance and store the value.
    - `Motor`
        - Defining the movement logic. (Forward, turning, backward, etc.)
        - Setting the speed.
    - `Client`
        - Read the byte number of the message in the socket.
        - Set up socket connection with server by host address and port number.
        - Send a message depending on whether the car has entered or exited the zone.
        - Identify the signal sent by the server and recreate one for the client.
        - Read the data and identify what the signal is.
    - `CppThread`
        - Created thread objects, which provides a common interface for starting and joining threads.
    - `RFIDThread`
        - To detect whether the car is present and store the message into a character array get_id[16] 
    - `Readmsg`
        - Read the data depending on the number of byte read, which is not equal to 0
    - `BuzzCtrl`
        - Control a buzzer based on the car's status ( Activates the buzzer when the car is stopped and deactivates the buzzer when the car is running or has exited the crossing)

- Flow Chart
    - Buzzer
  ![Buzzer_FlowChart](https://github.com/CarsonSun2207/RTEP-Project/assets/158228597/a683ecc8-5619-4a22-80cf-63dcd7f0fec5)
                                                                                                                                          
  - Ultrasonic and Line Tracking
  ![UltrasonicLineTrack_FlowChart](https://github.com/CarsonSun2207/RTEP-Project/assets/158082859/9dbbd942-08f1-4576-ab61-486260da80df)


### Offboard Raspberry Pi
Offboard Raspberry Pi acts as a server that connects the traffic light signals of the road system. 
- Function
    - Traffic control: When it receives the RFID signal coming from the car to enter the road, if the traffic light signal will be changed red at this time, it will transmit the stop command  to the Onboard Raspberry Pi in the car, to notify the user to stop the car by activating the onboard buzzer; if the signal  becmomes green at this time, it will deactivate the onboard buzzer , this  indicates the user to continue his journey . When it receives the RFID signal from the car to leave, it will stop sending commands to the Raspberry Pi in the car according to the traffic light signal.

    - Line Tracking and Obstacle Avoidance: The motion of the car is controlled by the light sensor module, when the line is detected. 

- Libraries
    - `Server`
        - Set up server by the server IP address & port number.
        - Accept the connection request from the client in the socket-clt_soc.
        - Read the message sent from the client and store it in the character array pointer buffer.
        - Send traffic signal to client depending on the signal, which is altered by the thread Trafficsig.
    - `Readmsg`
        - Keep reading the message from the client.
        - Detect whether the message is entering or exiting.
    - `Trafficsig`
        - Generate the traffic light signal.
    - `CppThread`
        - Create a thread wrapper for future use
    
- Flow Chart
    - Server
  ![Server_FlowChart](https://github.com/CarsonSun2207/RTEP-Project/assets/158082859/dc733559-fd17-40ce-8423-bee159672ef6)
  


## Unit Test
- Google test is used for unit tests in this work.
    -  Install Google Test on Raspberry Pi
    ```
    sudo apt-get install libgtest-dev
    sudo apt-get install cmake
    cd /usr/src/gtest
    sudo cmake .
    sudo make
    sudo cp *.a /usr/lib
    ```
    - Add Google Test to CMakeList.txt

- Test cases are:
- 1. To test whether the server and client can connect (Server_test.cpp)
    ```
          TEST(ServerTest, SetupServer) {
            Server server;
            int server_fd;
            struct sockaddr_in address;
            int port = 5560;
        
            server.setupServer(server_fd, address, port);
            EXPECT_GT(server_fd, 0);
          }
    ```
  - Testing Result
<p align="center">
  <img src="https://github.com/user-attachments/assets/26f97ed1-7fdf-49fe-8011-b29edc1cbfc4"  />
</p>
    
- 2. To test the functionality of the generation of traffic signal  (Tsig_test.cpp)
    ```
        TEST_F(TrafficsigTest, TrafficSignalHandler) {
            // Initial state should be false
            EXPECT_FALSE(trafficSignal.getTrafficSignal());
        
            // Simulate the conditions under which trafficSignalHandler should be called
            trafficSignal.testStaticTrafficSignalHandler();
        
            // After one call, the signal should be true
            EXPECT_TRUE(trafficSignal.getTrafficSignal());
        
            // Call the handler again to toggle the signal back
            trafficSignal.testStaticTrafficSignalHandler();
        
            // After the second call, the signal should be false again
            EXPECT_FALSE(trafficSignal.getTrafficSignal());
        
            // Simulate changing the state to "Exited"
            strcpy(readmsg.rdmsg, "Exited");
            trafficSignal.testStaticTrafficSignalHandler();
        
            // Verify the signal state if needed based on "Exited"
            EXPECT_TRUE(trafficSignal.getTrafficSignal()); // Adjust based on actual logic
        }
    ```
  - Testing Result
<p align="center">
  <img src="https://github.com/user-attachments/assets/70f0368a-5608-4b88-bef5-675be468b694"  />
</p>

- 3. To test card reading (ser&clie_test01.cpp)
    ```
        TEST_F(ClientServerTest, SimulatesCardReading) {
          std::cout << "Running SimulatesCardReading test" << std::endl;
      
          // Simulate card ID
          std::string cardID = "12345";
      
          // Simulate scanning card
          std::cout << "Before client.scanCard" << std::endl;
          client.scanCard(cardID);
          std::cout << "After client.scanCard" << std::endl;
        }
    ```
  - Testing Result
<p align="center">
  <img src="https://github.com/user-attachments/assets/639aee64-95e3-4d87-87da-fbfde2bd88a8"  />
</p>


## Installation   
1. Downloading from GitHub.
2. Installing needed libraries on your Pi.
>* PIGPIO: The library can be found [here](https://abyz.me.uk/rpi/pigpio/download.html)
>* CMFRC522: The library can be found [here](https://github.com/chihebabid/CMFRC522)  

3. Using CMake To compile and build the code
- Both for the onboard Raspberry Pi (Client) and the offboard Raspberry Pi (Server)
  - Navigate to your project directory.
  - Create a build directory.
  - Run CMake
  - Build the project
  ```
    cd /path_to_your_project_directory
    mkdir build
    cd build
    cmake ..
    make
  ```
4. Run your programme
- for example, when the executable is Cilent.exe
- run the following command
 ```
    sudo ./Client
  ```


## Social Media   
- Welcome to follow our TikTok [@travas_pro](http://www.tiktok.com/@travas_pro)

<p align="center">
  <img src="https://github.com/CarsonSun2207/RTEP-Project/assets/158082859/5c1ddd1e-378b-4fe3-9dd6-2f3bb84b3fbd" width = 300px/>
</p>  



## Contact us
- Hairuo Sun    https://github.com/CarsonSun2207
- Chi Yin Leung https://github.com/cyter23
- Jiaqi Yu      https://github.com/KKJYu
- YUQING LI     https://github.com/YUQINGLI77

