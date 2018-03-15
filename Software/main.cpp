#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <signal.h>
#include "PWMCtrl.h"
#include "GPIOWriter.h"
#include "ReadTemp.h"
#include "PID.h"

/* 
    This code is intended for execution on a RPI 3 Model B running Raspbian. 
    
    In order for this code to execute correct the linux kernel version must be at least 4.9.
    This can be checked by executing the command:   uname -r
    
    Additionally the file: /boot/config.txt
    must contain the line: dtoverlay=pwm,pin=19,func=2 
    to configure BCM pin 19 (GPIO 35) so that it can be used for PWM for the fan control.
    
    The temp sensor unique identity codes NEONATE_TEMP_SENSOR and AMBIENT_TEMP_SENSOR must be 
    set to match the DS18B20 devices being used.
*/

static const int PWMCHIP = 1;                   // PWM chip on the RPI that will be used to control the fan (PWM chip 1 is connected to GPIO pins 33 and 35).
static const int HEATER_BCM_PIN = 10;           // BCM (GPIO) Pin on the RPI that the heater will be connected to. (BCM pin 10 is connected to GPIO pin 19).
static const int DEFAULT_PERIOD = 40000;        // Default period that will be used for the fan PWM control.
static const int DEFAULT_DUTY_CYCLE = 20000;    // Default duty cycle that will be used for the fan PWM control.
static const int SUCCESS = 0;                   // Used to indicate that the program has so far executed correctly.
static const int FAILURE = 1;                   // Used to indicate that the program has not executed correctly in some way.

static std::string NEONATE_TEMP_SENSOR = "28-000005f4e7d6";       // Unique identity code of DS18B20 temp sensor being used to measure neonate temperature.
static std::string AMBIENT_TEMP_SENSOR = "28-000005f50d4c";       // Unique identity code of DS18B20 temp sensor being used to measure ambient temperature.

static const int body_temp = 36.4;
static const int output_clip = 5;         // TODO: Tune value
static const int sample_time = 0;         // TODO

static PWMCtrl *pwm;            // Fan control object
static GPIOWriter *heater;      // Heater control object
static TempReader *neonate;     // Neonate temperature sensor control object
static TempReader *ambient;     // Ambient temperatrue sensor control object

/*
    Ensures that all created objects are properly decommisioned prior to exiting the program.
    Must be provided with the current success status of execution (either SUCCESS or FAILURE).
*/
void exit_gracefully(int success){
    
    std::cout << "Exiting...." << endl;
    
    try{    
            
        if (pwm){
            pwm->disable(); // Disables the fan 
            delete pwm;
        }
        if (heater){
            heater->low(); // Turns of the heater
            delete heater;
        }
        delete neonate;
        delete ambient;
        
    } catch(std::exception& e){       
        std::cerr << e.what() << std::endl; 
        success = FAILURE; // The execution is considered a failure if any exceptions were thrown.
    }  
    
    if (success == SUCCESS)
        std::cout << "Program executed successfully" << std::endl;
    else
        std::cout << "Program execution failed" << std::endl;
    
    exit(success);
    
}

int main(){

    // Register a signal handler so that the program can exit correctly if a ctl-c signal is received.
    signal(SIGINT, exit_gracefully);

    try{
        // Create a new PWMCtl object to control the fan, set the default period and duty cycle, and enable.
        pwm = new PWMCtrl(PWMCHIP);
        pwm->configure(DEFAULT_PERIOD,DEFAULT_DUTY_CYCLE);
        pwm->enable();

        // Configure a GPIO pin to control the heater and turn it on
        heater = new GPIOWriter(HEATER_BCM_PIN);
        heater->high();

        // Load the neccessary linux kernal modules and then configure two temp sensors.
        TempReader::loadKernelModules();
        neonate = new TempReader(NEONATE_TEMP_SENSOR);
        ambient = new TempReader(AMBIENT_TEMP_SENSOR);

    } catch(std::exception& e){
        // Program exits if any exceptions are detected.
        std::cerr << e.what() << std::endl;
        exit_gracefully(FAILURE);
    }

    while(true){
        std::cout << neonate->readTemp() << std::endl;
        // std::cout << "Neonate Temperature: " << neonate->readTemp() << std::endl;
        // std::cout << "Ambient Temperature: " << ambient->readTemp() << std::endl;
    }

/*

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

    
*/
}
