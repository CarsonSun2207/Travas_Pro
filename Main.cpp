#include <iostream>
#include <cmath>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

class PCA9685 {
private:
    int fd;

public:
    PCA9685(int address, bool debug) {
        fd = wiringPiI2CSetup(address);
        reset();
    }

    void reset() {
        writeReg(0x00, 0x00);
    }

    void setPWMFreq(int freq) {
        float prescaleval = 25000000.0;  // 25MHz
        prescaleval /= 4096.0;  // 12-bit
        prescaleval /= float(freq);
        prescaleval -= 1.0;
        int prescale = int(prescaleval + 0.5);

        int oldmode = readReg(0x00);
        int newmode = (oldmode & 0x7F) | 0x10;  // sleep
        writeReg(0xFE, prescale);
        writeReg(0x00, oldmode);
        delay(5);
        writeReg(0x00, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
    }

    void setPWM(int channel, int on, int off) {
        writeReg(0x06 + 4 * channel, on & 0xFF);
        writeReg(0x06 + 4 * channel + 1, on >> 8);
        writeReg(0x06 + 4 * channel + 2, off & 0xFF);
        writeReg(0x06 + 4 * channel + 3, off >> 8);
    }
private:
    void writeReg(int reg, int data) {
        wiringPiI2CWriteReg8(fd, reg, data);
    }

    int readReg(int reg) {
        return wiringPiI2CReadReg8(fd, reg);
    }
};
class ADC {
private:
    int fd;

public:
    ADC(int address) {
        fd = wiringPiI2CSetup(address);
    }

    int recvADC(int channel) {
        wiringPiI2CWrite(fd, channel);
        delay(100);
        int val = wiringPiI2CRead(fd);
        return val;
    }
};
class Motor {
private:
    PCA9685 pwm;
    ADC adc;
    double time_proportion = 2.5;

public:
    Motor() : pwm(0x40, true), adc(0x48) {
        pwm.setPWMFreq(50);
    }

    void duty_range(int& duty) {
        if (duty > 4095) {
            duty = 4095;
        }
        else if (duty < -4095) {
            duty = -4095;
        }
    }

    void setMotorModel(int duty1, int duty2, int duty3, int duty4) {
        duty1, duty2, duty3, duty4 = duty_range(duty1, duty2, duty3, duty4);
        Motor PWM;

        void loop() {
            PWM.setMotorModel(2000, 2000, 2000, 2000);  // Forward
            usleep(3000000);
            PWM.setMotorModel(-2000, -2000, -2000, -2000);  // Back
            usleep(3000000);
            PWM.setMotorModel(-500, -500, 2000, 2000);  // Left
            usleep(3000000);
            PWM.setMotorModel(2000, 2000, -500, -500);  // Right
            usleep(3000000);
            PWM.setMotorModel(0, 0, 0, 0);  // Stop
        }

        void destroy() {
            PWM.setMotorModel(0, 0, 0, 0);
        }

        int main() {
            wiringPiSetup();
            while (1) {
                PWM.setMotorModel(2000, 2000, 2000, 2000);
            }

            return 0;
        }
//motor rotate
//move forwrad
//while(1)
/*
  {deploy CNN;
  grabbing input from camera feed
  }
  if(detected ==1)
  {
  }
  else
  {
  }//deploy as interrupt
*/

