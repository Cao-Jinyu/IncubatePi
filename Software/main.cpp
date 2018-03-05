#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "PWMCtrl.h"
#include "GPIOWriter.h"
#include "ReadTemp.h"
#include "PID.h"

/*
    uname -r                        : Check kernel version
    /boot/config.txt                : Edit this file
    dtoverlay=pwm,pin=19,func=2     : Add this line
*/

static const int PWMCHIP = 1;
static const int HEATER_BCM_PIN = 10;
static const int DEFAULT_PERIOD = 40000;
static const int DEFAULT_DUTY_CYCLE = 20000;

static std::string TEMP_SENSOR_1 = "28-000005f4e7d6";
static std::string TEMP_SENSOR_2 = "28-000005f50d4c";

static const int body_temp = 37;
static const int output_clip = 5;         // TODO: Tune value

static const int sample_time = 0;         // TODO


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
        TempReader::loadKernelModules();
        neonate = new TempReader(TEMP_SENSOR_1);
        ambient = new TempReader(TEMP_SENSOR_2);

    } catch(std::exception& e) {
        std::cerr << e.what();
        delete pwm;
        delete heater;
        delete neonate;
        delete ambient;
        return 1;
    }

    while(true) {
        std::cout << neonate->readTemp() << std::endl;
        // std::cout << "Neonate Temperature: " << neonate->readTemp() << std::endl;
        // std::cout << "Ambient Temperature: " << ambient->readTemp() << std::endl;
    }

/*
    while (true){
        std::cout << "Neonate Temperature :" << neonate->readTemp() << std::endl;
        std::cout << "Ambient Temperature :" << ambient->readTemp() << std::endl;
    }

    // Main loop with PID controller

    double heater_pwm;
    int output = 0;

    PID *temperature_PID = new PID(body_temp, sample_time);

    while(true) {
        // Read temperature
        output = temperature_PID->iterate(neonate->readTemp());

        // Calculate heater PWM value
        if (output > output_clip) output = output_clip;
        if (output < (0 - output_clip)) output = 0 - output_clip;
        heater_pwm = output / output_clip;
    }

    pwm->disable();
    delete pwm;
    delete heater;
    delete neonate;
    delete ambient;
    delete temperature_PID;
*/
}
