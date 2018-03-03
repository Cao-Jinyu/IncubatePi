#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "PWMCtrl.h"

static std::string EXPORTFILE = "/sys/class/pwm/pwmchip0/export";
static std::string BASEFILE = "/sys/class/pwm/pwmchip0/pwm";
static std::string PERIODFILE = "/period";
static std::string DUTYCYCLEFILE = "/duty_cycle";
static std::string ENABLEFILE = "/enable";

static std::string TEMPFILE = "temp";
static std::string MODULECHECK = "lsmod | grep pwm > " + TEMPFILE;

static const int MIN_PERIOD = 10000;        // Minimum allowed PWM period	
static const int ENABLE = 1;
static const int DISABLE = 0;
static const int NUM_OF_CHIPS = 2;

PWMCtrl::PWMCtrl(int pwmchip){
		
	std::ifstream tempFile;
	std::ofstream exportFile;
	std::string line;

	// Check the pwm chip number is a valid for the RPI
	if (pwmchip < 0 || pwmchip >= NUM_OF_CHIPS){
		std::cerr << "Couldn't create PWMCtrl as chip number is invalid" << std::endl;
		throw new std::exception();
	}
	
    // Execute Linux command that writes any modules containing the string pwm to a file temp.
	if (system(MODULECHECK.c_str()) == -1) {
		std::cerr << "PWM Module Check Error: Couldn't execute lsmod linux system command" << std::endl; 
		throw new std::runtime_error(NULL);
	}
	
	// Opens the file where the output of the command was piped to.
	tempFile.open(TEMPFILE.c_str());
	if (!tempFile.is_open()) {
		std::cerr << "PWM Module Check Error: Couldn't open temp file" << std::endl;
		throw new std::runtime_error(NULL);
	}	
	
	// Checks that the file is not empty. 
	if (!getline(tempFile,line)) {
		printf("PWM Module Check Error: Couldn't find the neccessary kernal modules\n");
		tempFile.close();
		throw new std::runtime_error(NULL);
	}		 	
	tempFile.close();

	// Open the linux PWM export file.
	exportFile.open(EXPORTFILE.c_str());
	if(!exportFile.is_open()) {
		std::cerr << "Couldn't carry out export as couldn't open PWM export linux system file" << std::endl;
		throw new std::runtime_error(NULL);   
	}

	// Write out the pwmchip number to export that chip.
	exportFile << pwmchip << std::endl;
	exportFile.close();

	// Create pwm linux system file names.
	periodFileName = BASEFILE + std::toString(pwmchip) + PERIODFILE;
    dutyCycleFileName = BASEFILE + std::toString(pwmchip) + DUTYCYCLEFILE;
    enableFileName = BASEFILE + std::toString(pwmchip) + ENABLEFILE;

}

void PWMCtrl::configure(int period, int dutyCycle) {
	
	std::ofstream periodFile;
	std::ofstream dutyCycleFile;
	
	// Check that the period is valid.
	if (period < MIN_PERIOD) {
		std::cerr << "Invalid period. Must be greater than " << MIN_PERIOD << "." << std::endl;
		throw new std::exception();
	}

	// Check that the duty cycle is valid
	if (dutyCycle < 0 || dutyCycle > period) {
		std::cerr << "Invalid duty cycle. Must be between 0 and the period." << std::endl;
		throw new std::exception();
	}
	
	// Open the linux PWM period file.
	periodFile.open(periodFileName.c_str());
	if(!periodFile.is_open()) {
		std::cerr << "Couldn't update period as couldn't open PWM period linux system file" << std::endl;
		throw new std::runtime_error(NULL); 
	}

	// Write out period to linux PWM period file
	periodFile << period << std::endl;
	periodFile.close();

	// Open the linux PWM duty cycle file
	dutyCycleFile.open(dutyCycleFileName.c_str());
	if(!dutyCycleFile.is_open()) {
		std::cerr << "Couldn't update duty cycle as couldn't open PWM duty cycle linux system file" << std::endl;
		throw new std::runtime_error(NULL);  
	}

	// Write out duty cycle to linux PWM duty cycle file
	dutyCycleFile << dutyCycle << std::endl;
	dutyCycleFile.close();

	std::cout << "Period: " << period << ", Duty Cycle: " << dutyCycle << std::endl;

}

void PWMCtrl::enable(){

	std::ofstream enableFile;
	
	// Open the linux PWM enable file
	enableFile.open(enableFileName.c_str());
	if(!enableFile.is_open()) {
		std::cerr << "Couldn't enable PWM as couldn't open PWM enable linux system file" << std::endl;
		throw new std::runtime_error(NULL);  
	}

	// Write out enable
	enableFile << ENABLE << std::endl;
	enableFile.close();
}

void PWMCtrl::disable(){	

	std::ofstream enableFile;
	
	// Open the linux PWM enable file
	enableFile.open(enableFileName.c_str());
	if(!enableFile.is_open()) {
		std::cerr << "Couldn't disable PWM as couldn't open PWM enable linux system file" << std::endl;
		throw new std::runtime_error(NULL);     
	}

	// Write out disable
	enableFile << DISABLE << std::endl;
	enableFile.close();
	
}
