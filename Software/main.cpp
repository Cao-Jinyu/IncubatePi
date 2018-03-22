#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <thread>
#include <chrono>
#include "PWMCtrl.hpp"
#include "GPIOWriter.hpp"
#include "TempReader.hpp"
#include "PID.hpp"

/*
    This code is intended for execution on a RPI 3 Model B running Raspbian.

    In order for this code to execute correctly the linux kernel version must be at least 4.9.
    This can be checked by executing the command:   uname -r

    Additionally the file: /boot/config.txt
    must contain the line: dtoverlay=pwm,pin=19,func=2
    to configure BCM pin 19 (GPIO 35) so that it can be used for PWM for the fan control.

    The temp sensor unique identity codes NEONATE_TEMP_SENSOR and AMBIENT_TEMP_SENSOR must be
    set to match the DS18B20 devices being used.
*/

static std::string NEONATE_TEMP_SENSOR = "28-000005f4e7d6";       // Unique identity code of DS18B20 temp sensor being used to measure neonate temperature
static std::string AMBIENT_TEMP_SENSOR = "28-000005f50d4c";       // Unique identity code of DS18B20 temp sensor being used to measure ambient temperature

static const int PWMCHIP = 1;                   // PWM chip on the RPI that will be used to control the fan (PWM chip 1 is connected to GPIO pins 33 and 35)
static const int HEATER_BCM_PIN = 10;           // BCM Pin on the RPI that the heater will be connected to (BCM pin 10 is connected to GPIO pin 19)
static const int DEFAULT_PERIOD = 40000;        // Default period that will be used for the fan PWM control (nanoseconds)
static const int DEFAULT_DUTY_CYCLE = 40000;    // Default duty cycle that will be used for the fan PWM control (nanoseconds)
static const int HEATER_PERIOD = 10;            // Period of heater PWM (seconds)

static const float NEONATE_IDEAL_MIN = 36.5;    // Lower boundary of the neonate ideal body temperature range (°C)
static const float NEONATE_IDEAL_MAX = 37.2;    // Higher boundary of the neonate ideal body temperature range (°C)
static const float MIN_AMBIENT_TEMP = 25.0;     // Minimum allowed target ambient temperature(°C)
static const float MAX_AMBIENT_TEMP = 36.0;     // Maximum allowed target ambient temperature (°C)
static const float INITIAL_TARGET = 32.0;       // Initial ambient temperature target (°C)
static const int TEMP_READ_DELAY = 60;          // Delay between neonate temperature readings (seconds)

static const int SUCCESS = 0;                   // Used to indicate that the program has so far executed correctly
static const int FAILURE = 1;                   // Used to indicate that the program has not executed correctly in some way
static int success = SUCCESS;                   // Used to indicate wether the execution of the program has been successful

static std::string OUTFILE = "results.txt";     // File where program output will be written to
static std::ofstream outfile;

static const int MIN_PID = 0;           // The minimum possible value of the PID output
static const int MAX_PID = 1;           // The maximum possible value of the PID output
static const int SAMPLE_TIME = 10;      // The time between PID iterations (seconds)
static const float P_COEFF = 0.14600;   // PID proportional coefficient
static const float D_COEFF = 0.16000;   // PID differential coefficient
static const float I_COEFF = 0.00060;   // PID integral coefficient

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
        float get_current_value(){ return ambient->readTemp(); }
};

static AmbientTempPID *ambient_temp_pid; // Ambient temperature PID controller

/*
    Controls the heater PWM signal according to the PID specified duty cycle.
    The duty cycle is specified as a proportion (between 0 and 1) by variable heater_pwm_duty_cycle.
    (Intended for execution in a seperate thread as it contains an infinite loop.)
*/
void heater_pwm(){

    int period_ms, on_time_ms, off_time_ms;
    float heater_pwm_duty_cycle = 0;   
    
    while(true){
        
        // Check if the output value of the PID controller is ready to be read.
        if (ambient_temp_pid->isReady())

            // Update the pwm duty cycle based on the current PID controller output.
            heater_pwm_duty_cycle = ambient_temp_pid->get_pid_value();

        // Calculate PWM on and off times base on period and duty cycle
        period_ms = HEATER_PERIOD * 1000;
        on_time_ms = period_ms * heater_pwm_duty_cycle;
        off_time_ms = period_ms - on_time_ms;

        // Set the heater high and low accordingly
        heater->high();
        std::this_thread::sleep_for(std::chrono::milliseconds(on_time_ms));
        heater->low();
        std::this_thread::sleep_for(std::chrono::milliseconds(off_time_ms));
        
        std::cout << ambient->readTemp() << ", " << heater_pwm_duty_cycle << std::endl;
        outfile << ambient->readTemp() << ", " << heater_pwm_duty_cycle << std::endl;
    }
}

/*
    Updates the target ambient temperature based on the current neonate baby temperature.
    (Intended for execution in a seperate thread as it contains an infinite loop.)
*/
void ambient_target_select(){

    float neonate_temp;
    float target_ambient_temp = INITIAL_TARGET;     

    while (true){

        // Allow time for the temperature sensor to heat to the neonates body temperature.
        std::this_thread::sleep_for(std::chrono::seconds(TEMP_READ_DELAY));

        // Get neonate temperatrue
        neonate_temp = neonate->readTemp();

        // If the neonate temperature is outwith of the ideal range, increment or decrement the target ambient temperature accordingly
        // while ensuring it does not go outwith specified limits.
        if (neonate_temp < NEONATE_IDEAL_MIN && target_ambient_temp < MAX_AMBIENT_TEMP) target_ambient_temp++;
        else if (neonate_temp > NEONATE_IDEAL_MAX && target_ambient_temp > MIN_AMBIENT_TEMP) target_ambient_temp--;
        
        // Update the PID controller to the new target
        ambient_temp_pid->update_required_value(target_ambient_temp);

        std::cout << "Current Neonate Temperature: " << neonate_temp << std::endl;
        std::cout << "Target Ambient Temperature: " << target_ambient_temp << std::endl;
    }

}

/*
    Ensures that all created objects are properly decommisioned prior to exiting the program.
    Updates the current success status of execution (either SUCCESS or FAILURE).
*/
void exit_gracefully(int signum){

    std::cout << std::endl << "Exiting...." << std::endl;

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
        
        if (outfile.is_open()) outfile.close();
        
    } catch(std::runtime_error& e){
        std::cout << e.what() << std::endl;
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

        // Configure a GPIO pin to control the heater
        heater = new GPIOWriter(HEATER_BCM_PIN);

        // Load the neccessary linux kernal modules and then configure two temp sensors.
        TempReader::loadKernelModules();
        neonate = new TempReader(NEONATE_TEMP_SENSOR);
        ambient = new TempReader(AMBIENT_TEMP_SENSOR);

    } catch(std::exception& e){
        // Program exits if any exceptions are detected.
        std::cout << e.what();
        success = FAILURE;
        exit_gracefully(0);
    }
    
    // Open file to write program output to.
    outfile.open(OUTFILE.c_str());

    // Create an ambient temperature PID controller and set it to start iterating.
    ambient_temp_pid = new AmbientTempPID(SAMPLE_TIME, INITIAL_TARGET, MIN_PID, MAX_PID, P_COEFF, I_COEFF, D_COEFF);
    ambient_temp_pid->start_pid();

    // Start a thread that controls the heater PWM signal according to the PID output.
    std::thread heater_thread(heater_pwm);
    
    // Start a thread which monitors the neonate temperature and sets the target ambient temperature.
    std::thread temperature_thread(ambient_target_select);
    
    // Main will now sleep forever.
    heater_thread.join();
    temperature_thread.join();
}
