#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>     
#include <stdlib.h>
#include <cstring>
#include "ReadTemp.h"

static std::string W1GPIO = "modprobe w1-gpio";
static std::string W1THERM = "modprobe w1-therm";
static std::string W1PATH = "/sys/bus/w1/devices/";
static std::string W1FILE = "/w1_slave";

const int BUFFERSIZE = 256;
   
void TempReader::loadKernelModules(){

	// Loads the w1-gpio and w1-therm kernal modules by executing the modprobe linux command.
	if (system(W1GPIO.c_str()) == -1 || system(W1THERM.c_str()) == -1){
		std::cerr << "Couldn't load the neccessary 1-wire kernal modules" << std::endl;
  		throw new std::runtime_error(NULL);
	}
}     
    
TempReader::TempReader(std::string deviceName){

	// Construct the full file name of the 1-wire device using the specified device name.
	w1FileName = W1PATH + deviceName + W1FILE;

	// Open the 1-wire device file ready for reading.
	w1File.open(w1FileName.c_str());
	if(!w1File.is_open()) {
		std::cerr << "Couldn't create TempReader as device name is not valid" << std::endl;
		throw new std::runtime_error(NULL);
	}

}

TempReader::~TempReader(){

	w1File.close();

}

float TempReader::readTemp(){

	int length;
	char * buffer;
	char * start;
	char * end;
	float temp;

	// Check the 1-wire device file is open, if not attempt to open it.
  	if (!w1File.is_open()) {
  		w1File.open(w1FileName.c_str());
		if(!w1File.is_open()) {
			std::cerr << "Couldn't read the temperature as the 1-wire device file could not be accessed" << std::endl;
			throw new std::runtime_error(NULL);
		}
  	}

  	// Get the length of the file contents.
    w1File.seekg (0, w1File.end);
    length = w1File.tellg();
    w1File.seekg (0, w1File.beg);

    // Check the file is not empty.
    if (!length){
    	std::cerr << "Couldn't read the temperature as the 1-wire device file was empty" << std::endl;
    	throw new std::runtime_error(NULL);
    }

    // Read the contents of the file.
    buffer = new char[length];
    w1File.read(buffer,length);

	// Extract the temperature value from the file contents.
	// Temperature must be divided by 1000 to get value in degrees celcius.
    start = strstr(buffer, "t=");
    if (!start) {
    	std::cerr << "Couldn't read the temperature as the 1-wire device contents was not valid" << std::endl;
    	throw new std::runtime_error(NULL);
    }
    start = start + 2;
    end = start + 7;
    temp = strtof(start, &end) / 1000;

	// Free up the buffer storage.
	delete buffer;
	 
   	return temp;

}
