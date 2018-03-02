#include <stdlib.h>
#include <string>
#include <iostream>
#include "PWMCtrl.h"
#include "GPIOWriter.h"
#include "ReadTemp.h"

/*
    uname -r                        : Check kernel version
    /boot/config.txt                : Edit this file
    dtoverlay=pwm,pin=19,func=2     : Add this line
*/

static const int PWMCHIP = 1;
static const int HEATER_BCM_PIN = 10;
static const int DEFAULT_PERIOD = 40000; 
static const int DEFAULT_DUTY_CYCLE = 20000;   
static std::string TEMP_SENSOR_1 = "";
static std::string TEMP_SENSOR_2 = "";

int main () {

    
    try {

        // Create a new PWMCtl for PWM chip 1, set the default period and duty cycle, and enable.
        PWMCtrl *pwm = new PWMCtrl(PWMCHIP);
        pwm->configure(DEFAULT_PERIOD,DEFAULT_DUTY_CYCLE);
        pwm->enable();

        // Configure a GPIO pin to control the heater
        GPIOWriter *heater = new GPIOWriter(HEATER_BCM_PIN);
        heater->high();

        // Load the neccessary linux kernal modules and then configure two temp sensors
        loadKernelModules();
        TempReader *neonate = new TempReader(TEMP_SENSOR_1);
        TempReader *ambient = new TempReader(TEMP_SENSOR_2);
    
    } catch(std::exception& e) {

        return 1;

    }

    while (true){
        std::cout << "Baby Temperature:" << neonate->readTemp() << std::endl;
        std::cout << "Ambient Temperature:" << ambient->readTemp() << std::endl;
    }

    // PID controller

    const int sample_time = 0;
    const int body_temp = 37;
    int neonate_temp = 0;

    int previous_error = 0;
    int current_error = 0;
    int integral = 0;
    int derivative = 0;
    int Kp = 5;                  // TODO
    int Ki = 5;                  // TODO
    int Kd = 5;                  // TODO
    int output = 0;

    while(true) {
        // Read temperatures
        neonate_temp = neonate->readTemp();        // Read from temp sensor

        current_error = body_temp - neonate_temp;
        integral = integral + current_error*sample_time;
        derivative = (current_error - previous_error)/sample_time;
        output = Kp*current_error + Ki*integral + Kd*derivative;         // May require bias also?
        previous_error = current_error;
    }

    // Disable PWM controller
    pwm->disable();
    delete pwm;
    delete heater;
    delete neonate;
    delete ambient;
}
