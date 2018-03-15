#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <thread> 
#include <chrono> 
#include "PWMCtrl.hpp"
#include "GPIOWriter.hpp"
#include "ReadTemp.hpp"
#include "PID.hpp"

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
static const int HEATER_PERIOD = 10;			// Period of heater PWM in seconds

static std::string NEONATE_TEMP_SENSOR = "28-000005f4e7d6";       // Unique identity code of DS18B20 temp sensor being used to measure neonate temperature.
static std::string AMBIENT_TEMP_SENSOR = "28-000005f50d4c";       // Unique identity code of DS18B20 temp sensor being used to measure ambient temperature.

static const int OUTPUT_CLIP = 0;	// TODO
static const int SAMPLE_TIME = 0;   // TODO
static const int TARGET = 0;		// TODO
static const int P_COEFF = 0;		// TODO
static const int D_COEFF = 0;		// TODO
static const int I_COEFF = 0;		// TODO

static PWMCtrl *pwm;            // Fan control object
static GPIOWriter *heater;      // Heater control object
static TempReader *neonate;     // Neonate temperature sensor control object
static TempReader *ambient;     // Ambient temperatrue sensor control object

/*
    Concrete implementation of the abstract class PID which implements the 
    get_current_value() function by reading the current ambient temperature.
*/
class AmbientTempPID : public PID {

    using PID::PID;
    private:
        int get_current_value(){ return ambient->readTemp(); }
};  

static AmbientTempPID *ambient_temp_pid; // Ambient temperature PID controller

static int heater_pwm_duty_cycle = 0;	// Proportion of the heater pwm signal that is high. Must be between 0 and 1.

/*
	Controls the heater PWM signal according to the specified duty cycle.
	The duty cycle is specified as a proportion by variable heater_pwm_duty_cycle.  
*/
void heater_pwm(){

	int period_ms, on_time_ms, off_time_ms;
	
	while(true){
	
		// Calculate PWM on and off times base on period and duty cycle
		period_ms = HEATER_PERIOD * 1000;
		on_time_ms = period_ms * heater_pwm_duty_cycle;
		off_time_ms = period_ms - on_time_ms;
		
		// Set the heater high and low accordingly
		heater->high();
		std::this_thread::sleep_for(std::chrono::milliseconds(on_time_ms));
		heater->low();
		std::this_thread::sleep_for(std::chrono::milliseconds(off_time_ms));

    }
}

/*
	Takes the PID output and converts this to a PWM duty cycle.
	Returns the duty cycle as a number between 0 and 1.
*/
int heater_pid_to_pwm_map(int pid_value){
	
	return 0; //TODO
}

/*
    Ensures that all created objects are properly decommisioned prior to exiting the program.
    Must be provided with the current success status of execution (either SUCCESS or FAILURE).
*/
void exit_gracefully(int success){
    
    std::cout << "Exiting...." << std::endl;
    
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
        delete ambient_temp_pid;
        
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

	// Create an ambient temperature PID controller and set it to start iterating.
	ambient_temp_pid = new AmbientTempPID(SAMPLE_TIME, TARGET, OUTPUT_CLIP, P_COEFF, I_COEFF, D_COEFF);
	ambient_temp_pid->start_pid();
	
	// Start a thread that controls the heater PWM signal.
	std::thread heater_thread(heater_pwm);
    
    while(true){
    
    	// Check if the output value of the PID controller is ready to be read.
    	if (ambient_temp_pid->isReady())	
    	
    		// Update the pwm duty cycle based on the current PID controller output.
    		heater_pwm_duty_cycle = heater_pid_to_pwm_map( ambient_temp_pid->get_pid_value() );
    		
        std::cout << ambient->readTemp() << std::endl;
        std::cout << heater_pwm_duty_cycle << std::endl;
        
    }

}
