#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include "PWMCtrl.h"   

static std::string EXPORTFILE = "/sys/class/pwm/pwmchip0/export";
static std::string PERIODFILE = "/sys/class/pwm/pwmchip0/pwm1/period";
static std::string DUTYCYCLEFILE = "/sys/class/pwm/pwmchip0/pwm1/duty_cycle";
static std::string ENABLEFILE = "/sys/class/pwm/pwmchip0/pwm1/enable";

static const std::string TEMPFILE = "temp";
static std::string MODULECHECK;

static const int MIN_PERIOD = 10000;        // Minimum allowed PWM period	
static const int PWMPORT = 1;
static const int ENABLE = 1;
static const int DISABLE = 0;

int PWMCtrl::setupCheck;
int PWMCtrl::configureCheck;
int PWMCtrl::period;


PWMCtrl::PWMCtrl() {
    PWMCtrl::setupCheck = 0;
    PWMCtrl::configureCheck = 0;
    PWMCtrl::period = 0;
}

/*
    Calls the neccessary functions to set up the PWM channel.
    Returns 0 and sets setupCheck on success.
    Returns 1 on failure.
*/
int PWMCtrl::setup() {
    MODULECHECK = "lsmod | grep pwm > " + TEMPFILE;

    if (moduleCheck()) return 1;
    if (exportPWM()) return 1;
    PWMCtrl::setupCheck = 1;
    return 0;
}

/*
    Calls the neccessary functions to configure the PWM channel.
    Returns 0 and sets configureCheck on success.
    Returns 1 on failure.
*/
int PWMCtrl::configure(int period, int dutyCycle) {
    if (!PWMCtrl::setupCheck) {
        std::cerr << "Couldn't configure PWM channel as setup function has not been performed" << std::endl;
        return 1;
    }
    if (setPeriod(PWMCtrl::period)) return 1;
    if (setDutyCycle(dutyCycle)) return 1;
    PWMCtrl::configureCheck = 1;
    std::cout << "Period: " << PWMCtrl::period << ", Duty Cycle: " << dutyCycle << std::endl;
    return 0;
}

/*
	Checks that the PWM module is loaded in the linux kernel.
	Returns 0 on success.
	Returns 1 on failure.
*/
int PWMCtrl::moduleCheck(){
		
	std::ifstream infile;
	std::string line;
	
    // Execute Linux command that writes any modules containing the string pwm to a file temp.
	if (system(MODULECHECK.c_str()) == -1) {
		std::cerr << "PWM Module Check Error: Couldn't execute lsmod linux system command" << std::endl; 
		return 1;
	}
	
	// Opens the file where the output of the command was piped to.
	infile.open(TEMPFILE.c_str());
	if (!infile.is_open()) {
		std::cerr << "PWM Module Check Error: Couldn't open temp file" << std::endl;
		return 1; 
	}	
	
	// Checks that the file is not empty and prints out the module name. 
	if (!getline(infile,line)) {
		printf("Couldn't find the neccessary kernal modules\n");
		infile.close();
  		return 1;
	} else std::cout << "Module: " << line << std::endl;	 	
	infile.close();
	return 0;
}

/*
    Exports the PWM channel.
	Returns 0 on success.
	Returns 1 on failure.
*/
int PWMCtrl::exportPWM() {

    std::ofstream outfile;
	
	// Open the linux PWM export file
	outfile.open(EXPORTFILE.c_str());
	if(!outfile.is_open()) {
		std::cerr << "Couldn't carry out export as couldn't open PWM export linux system file" << std::endl;
		return 1;   
	}

	// Write out export
	outfile << PWMPORT << std::endl;
	outfile.close();
	return 0;
}

/*
    Sets the PWM period by writing to the neccesary linux system file.
    Returns 0 on success.
    Returns 1 on failure.
*/
int PWMCtrl::setPeriod(int set_period) {
	
	std::ofstream outfile;
	
	// Check that the new period is valid.
	if (set_period < MIN_PERIOD) {
		std::cerr << "Invalid period. Must be greater than" << MIN_PERIOD << "." << std::endl;
		return 1;
	}
	
	// Open the linux PWM period file.
	outfile.open(PERIODFILE.c_str());
	if(!outfile.is_open()) {
		std::cerr << "Couldn't update period as couldn't open PWM period linux system file" << std::endl;
		return 1;   
	}

	// Write out period
	outfile << set_period << std::endl;
	PWMCtrl::period = set_period;
	outfile.close();
	return 0;
}

/*
    Sets the PWM duty cycle by writing to the neccesary linux system file
    Returns 0 on success
    Returns 1 on failure
*/
int PWMCtrl::setDutyCycle(int dutyCycle) {
	
	std::ofstream outfile;
	
	// Check that the duty cycle is valid
	if (dutyCycle < 0 || dutyCycle > PWMCtrl::period) {
		std::cerr << "Invalid duty cycle. Must be between 0 and the period." << std::endl;
		return 1;
	}
	
	// Open the linux PWM duty cycle file
	outfile.open(DUTYCYCLEFILE.c_str());
	if(!outfile.is_open()) {
		std::cerr << "Couldn't update duty cycle as couldn't open PWM duty cycle linux system file" << std::endl;
		return 1;   
	}

	// Write out duty cycle 
	outfile << dutyCycle << std::endl;
	outfile.close();
	return 0;
}

/*
    Enables PWM by writing to the neccesary linux system file
    Returns 0 on success
    Returns 1 on failure
*/
int PWMCtrl::enable() {

	std::ofstream outfile;
	
	// Open the linux PWM enable file
	outfile.open(ENABLEFILE.c_str());
	if(!outfile.is_open()) {
		std::cerr << "Couldn't enable PWM as couldn't open PWM enable linux system file" << std::endl;
		return 1;   
	}

	// Write out enable
	outfile << ENABLE << std::endl;
	outfile.close();
	return 0;
}

/*
    Disables PWM by writing to the neccesary linux system file
    Returns 0 on success
    Returns 1 on failure
*/
int PWMCtrl::disable()
{	
	std::ofstream outfile;
	
	// Open the linux PWM enable file
	outfile.open(ENABLEFILE.c_str());
	if(!outfile.is_open()) {
		std::cerr << "Couldn't disable PWM as couldn't open PWM enable linux system file" << std::endl;
		return 1;   
	}

	// Write out disable
	outfile << DISABLE << std::endl;
	outfile.close();
	return 0;
}
