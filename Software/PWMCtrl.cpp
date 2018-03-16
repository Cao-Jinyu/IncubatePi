#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "PWMCtrl.hpp"

static std::string EXPORTFILE = "/sys/class/pwm/pwmchip0/export";   // Linux system file which is written to in order to export a PWM chip.
static std::string BASEFILE = "/sys/class/pwm/pwmchip0/pwm";        // This is combined with a chip number to form the directory where the PWM files associated with that chip are found.
static std::string PERIODFILE = "/period";                          // Can be combined with BASEFILE and a chip no. to give the name of the linux system file were the PWM period should be written to.
static std::string DUTYCYCLEFILE = "/duty_cycle";                   // As above for duty cycle.
static std::string ENABLEFILE = "/enable";                          // As above for enable.

static const int MIN_PERIOD = 10000;    // The PWM period must be at least this value in all instances.
static const int ENABLE = 1;            // Should be written to the enable file in order to enable the PWM signal.
static const int DISABLE = 0;           // Should be written to the enable file in order to disable the PWM signal.
static const int NUM_OF_CHIPS = 2;      // Number of PWM chips availible on the RPI being used.
        
static std::string TEMPFILE = "temp";
static std::string MODULECHECK = "lsmod | grep pwm > " + TEMPFILE;  // Linux system command which outputs any installed modules containing the string "pwm" to TEMPFILE.

PWMCtrl::PWMCtrl(int pwmchip) {

    std::ifstream tempFile;
    std::ofstream exportFile;
    std::string line;

    // Check the pwm chip number is a valid for the RPI
    if (pwmchip < 0 || pwmchip >= NUM_OF_CHIPS)
        throw new std::range_error("Couldn't create PWMCtrl as chip number is invalid.\n");
    
    // Execute Linux command that writes any modules containing the string "pwm" to a file temp.
    if (system(MODULECHECK.c_str()) == -1) 
        throw new std::runtime_error("PWM Module Check Error: Couldn't execute lsmod linux system command.\n");
    
    // Opens the file where the output of the command was piped to.
    tempFile.open(TEMPFILE.c_str());
    if (!tempFile.is_open()) 
        throw new std::runtime_error("PWM Module Check Error: Couldn't open command output file.\n");
    
    // Checks that the file is not empty. If it is empty the PWM kernal module has not been installed.
    if (!getline(tempFile,line)) {
        tempFile.close();
        throw new std::runtime_error("PWM Module Check Error: Couldn't find the neccessary kernal modules.\n");
    }           
    tempFile.close();

    // Open the linux PWM export file.
    exportFile.open(EXPORTFILE.c_str());
    if(!exportFile.is_open())
        throw new std::runtime_error("Couldn't carry out export as couldn't open PWM export linux system file.\n");   

    // Write out the pwmchip number to export that chip.
    exportFile << pwmchip << std::endl;
    exportFile.close();

    // Create pwm linux system file names.
    periodFileName = BASEFILE + std::to_string(pwmchip) + PERIODFILE;
    dutyCycleFileName = BASEFILE + std::to_string(pwmchip) + DUTYCYCLEFILE;
    enableFileName = BASEFILE + std::to_string(pwmchip) + ENABLEFILE;

}

void PWMCtrl::configure(int period, int dutyCycle) {
    
    std::ofstream periodFile;
    std::ofstream dutyCycleFile;
    std::string exceptionMessage;
    
    // Check that the period is valid.
    if (period < MIN_PERIOD) {
        exceptionMessage = "Invalid period. Must be greater than " + std::to_string(MIN_PERIOD) + ".\n";
        throw new std::range_error(exceptionMessage);
    }

    // Check that the duty cycle is valid
    if (dutyCycle < 0 || dutyCycle > period)
        throw new std::range_error("Invalid duty cycle. Must be between 0 and the period.\n");
    
    // Open the linux PWM period file.
    periodFile.open(periodFileName.c_str());
    if(!periodFile.is_open())
        throw new std::runtime_error("Couldn't update period as couldn't open PWM period linux system file.\n"); 

    // Write out period to linux PWM period file
    periodFile << period << std::endl;
    periodFile.close();

    // Open the linux PWM duty cycle file
    dutyCycleFile.open(dutyCycleFileName.c_str());
    if(!dutyCycleFile.is_open())
        throw new std::runtime_error("Couldn't update duty cycle as couldn't open PWM duty cycle linux system file.\n");  

    // Write out duty cycle to linux PWM duty cycle file
    dutyCycleFile << dutyCycle << std::endl;
    dutyCycleFile.close();

    std::cout << "Period: " << period << ", Duty Cycle: " << dutyCycle << std::endl;

}

void PWMCtrl::enable(){

    std::ofstream enableFile;
    
    // Open the linux PWM enable file
    enableFile.open(enableFileName.c_str());
    if(!enableFile.is_open()) 
        throw new std::runtime_error("Couldn't enable PWM as couldn't open PWM enable linux system file.\n");  

    // Write out enable
    enableFile << ENABLE << std::endl;
    enableFile.close();
}

void PWMCtrl::disable(){    

    std::ofstream enableFile;
    
    // Open the linux PWM enable file
    enableFile.open(enableFileName.c_str());
    if(!enableFile.is_open()) 
        throw new std::runtime_error("Couldn't disable PWM as couldn't open PWM enable linux system file.\n");     

    // Write out disable
    enableFile << DISABLE << std::endl;
    enableFile.close();
    
}
