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
- Connection board      
- Jumper Wire F/F(4) 
- XH-2.54-5Pin Cable
- F-F Cables
- Prepared the necessary peripherals:    
    - HDMI display
    - USB keyboard and mouse
    - Power supply
    - Micro SD card with installed Raspbian OS System ×2

- Circuit Diagram 
![hardware_01](https://github.com/CarsonSun2207/RTEP-Project/assets/158228597/44bc54b7-b3c2-4fc6-838f-fb386b0eae8f)

- Pin Connection
![image](https://github.com/CarsonSun2207/RTEP-Project/assets/158082859/fb38fcc2-b90c-4e84-b29a-5e3afccb84ab)

## Software Development   
- Libraries    
    - pigpio
    - CMFRC522
- Class
    - `Motor`
    - `Ultrasonic`
    - `PCA9685`
    - 


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
