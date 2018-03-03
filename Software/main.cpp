#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
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

    PWMCtrl *pwm;
    GPIOWriter *heater;
    TempReader *neonate;
    TempReader *ambient;

    try {
        // Create a new PWMCtl for PWM chip 1, set the default period and duty cycle, and enable.
        pwm = new PWMCtrl(PWMCHIP);
        pwm->configure(DEFAULT_PERIOD,DEFAULT_DUTY_CYCLE);
        pwm->enable();

        // Configure a GPIO pin to control the heater
        heater = new GPIOWriter(HEATER_BCM_PIN);
        heater->high();

        // Load the neccessary linux kernal modules and then configure two temp sensors
        loadKernelModules();
        neonate = new TempReader(TEMP_SENSOR_1);
        ambient = new TempReader(TEMP_SENSOR_2);
    
    } catch(std::exception& e) {

        // TODO: Fix memory leaks here

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

    int Kp = 5;                  // TODO: Tune value
    int Ki = 5;                  // TODO: Tune value
    int Kd = 5;                  // TODO: Tune value
    int output_clip = 5;         // TODO: Tune value

    int output = 0;
    double heater_pwm;

    while(true) {
        // Read temperature
        neonate_temp = neonate->readTemp();

        current_error = body_temp - neonate_temp;
        integral = integral + current_error*sample_time;
        derivative = (current_error - previous_error)/sample_time;
        output = Kp*current_error + Ki*integral + Kd*derivative;         // May require bias also?
        previous_error = current_error;

        // Calculate heater PWM value
        if (output > output_clip) output = output_clip;
        if (output < (0 - output_clip)) output = 0 - output_clip;
        heater_pwm = output / output_clip;
    }

    // Disable PWM controller
    pwm->disable();
    delete pwm;
    delete heater;
    delete neonate;
    delete ambient;
}
