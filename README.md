# Onboard Anti-Collision Control and Virtual Traffic Light System - `Travas_Pro`




## Motivation   

In the not-so-distant future, picture yourself stepping into a world where vehicles become guardians of safety and pioneers of intelligent mobility. Our latest project, an Anti-Collision Control and Virtual Traffic Light System, is the protagonist of this narrative, reshaping the way we perceive road safety.

Meet Sarah, a daily commuter navigating bustling city streets. As she merges into traffic, her vehicle becomes a sanctuary of security, thanks to our revolutionary solution.

On a foggy morning, as Sarah approaches an intersection, the system detects an unexpected pedestrian shrouded in mist. In a blink, her vehicle gracefully halts, preventing a potential mishap. As the pedestrian leaves, the car intuitively resumes its journey, seamlessly blending innovation with everyday life.

But it doesn't end there. Imagine Sarah encountering a traffic light displaying a vibrant red hue. Even when the light turns green, her vehicle, guided by our intelligent system, remains stationary if pedestrians persist ahead. A fusion of obstacle awareness and traffic signal synchronization ensures not just safety but an efficient and harmonious flow of traffic.
 
## Hardware Requirements   

- Acquired Raspberry Pi 4B board ×2
- DC speed reduction motor (DRV8837) ×4
- Rechargeable 18560 Bottom-top battery ×2    
- RFID Reader (RC522)
- Driver Board (PCA9685PW)
- Ultrasonic sensor (HC-SR04)
- Ultrasonic Obstacle Avoidance Module (HC-SR04)    
- Servo (SG90) x2
- Tracking Sensor     
- Jumper Wire F/F(4) 
- XH-2.54-5Pin Cable
- F-F Cables x n
- Prepared the necessary peripherals:    
    - HDMI display
    - USB keyboard and mouse
    - Power supply
    - Micro SD card with installed Raspbian OS System ×2

- Circuit Diagram
![circuit](https://github.com/CarsonSun2207/RTEP-Project/assets/158082859/500ca218-14d0-4203-b5b5-350687bbdd5a)

- Pin Connection
![image](https://github.com/CarsonSun2207/RTEP-Project/assets/158082859/1185ea17-cd33-4689-b0ad-5ff84832d475)


## Software Development   
### The First Raspberry Pi
The first Raspberry Pi acts as a node connected to the server, which is mounted on the car.
- Function: When the car passes through the entrance of the road, the RFID reader on the car will sense the chip corresponding to the entrance, thus sending an entry signal to the server, and will receive the signal from the corresponding server to carry out the next operation. Similarly, when the car is leaving the road, the RFID reader on the car will sense the chip corresponding to the exit, thus sending the leave signal to the server.
- Libraries    
    - pigpio
    - CMFRC522
- Class
    - `Motor`
    - `Ultrasonic`
    - `PCA9685`
    - 
### The second Raspberry Pi
The second Raspberry Pi acts as a server that connects the traffic light signals of the road system. 
- Function: When it receives the RFID signal coming from the car to enter the road, if the traffic light signal is red at this time, it will transmit the command of stop to the raspberry pi in the car, so as to make the car stop; if the light is green at this time, it will not send any command. When it receives the RFID signal from the car to leave, it will stop sending commands to the raspberry pi in the car according to the traffic light signal.


## Unit Test
Perform unit test in this work. Test cases are:
- Motor
```
...
switch (command) {
            case 1:		#Forward
                motor.Forward();
                break;
            case 2:		#Backward
                motor.Backward();
                break;
            case 3:		#TurnLeft
                motor.TurnLeft();
                break;
            case 4:		#TurnRight
                motor.TurnRight();
                break;
            case 5:		#Stop
                motor.Stop();
                break;
            default:
                std::cerr << "Invalid command" << std::endl;
                break;
        }
...
```
- RFID Reader
```
...
   // Initializes PCD
   RFIDThread rfidthd;
   TogglingThread tglthd;
   rfidthd.start();
   rfidthd.join();
...
```
- Ultrasonic Sensor and Motor
```
...
if (distance != -1 && distance < 60) {
            motor.Stop();
        } else {
            motor.Forward();
        }
...
```
- 
## Prerequisites   
1. Downloading from github.
2. Installing needed libraries on your Pi.
>* PIGPIO: The library can be found [here](https://abyz.me.uk/rpi/pigpio/download.html)
>* CMFRC522: The library can be found [here](https://github.com/chihebabid/CMFRC522)  

3. To compile and build the code

## Tests
Steps to run test are:
```

```

## Social Media   
- Welcome to follow our TikTok [@travas_pro](http://www.tiktok.com/@travas_pro)        

## Reference Links   
