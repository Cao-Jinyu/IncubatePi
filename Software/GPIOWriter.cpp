#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "GPIOWriter.h"

const std::string EXPORTFILE = "/sys/class/gpio/export";
const std::string UNEXPORTFILE = "/sys/class/gpio/unexport";
const std::string FILESTRINGBASE = "/sys/class/gpio/gpio";
const std::string DIRECTIONFILE = "/direction";
const std::string VALUEFILE = "/value";

const int MAXPIN = 26;
const std::string OUT = "out";
const int HIGH = 1;
const int LOW = 0;

GPIOWriter::GPIOWriter(int pin){

	std::ofstream exportFile;
	std::ofstream directionFile;
	std::stringstream directionFileName;
	std::stringstream valueFileName;

	// Check that the specified pin is valid.
	if (pin < 0 || pin > MAXPIN){
		std::cerr << "Couldn't create GPIOWriter as pin" << pin <<  "is not valid." << std::endl;
		throw new std::exception();
	}

	// Export the required pin by writing the pin name to the gpio export file
	exportFile.open(EXPORTFILE.c_str());
	if(!exportFile.is_open()) {
		std::cerr << "Couldn't create GPIOWriter on pin" << pin << "as the export file could not be opened." << std::endl;
		throw new std::runtime_error(NULL);
	}
	exportFile << pin;
	exportFile.close();

	// Sets the direction of the pin to out so that it can be written to.
	directionFileName << FILESTRINGBASE << pin << DIRECTIONFILE;
	directionFile.open(directionFileName.str().c_str());
	if (!directionFile.is_open()){
		std::cerr << "Couldn't create GPIOWriter on pin" << pin << "as the direction file could not be opened." << std::endl;
		throw new std::runtime_error(NULL);
	}
	directionFile << OUT;
	directionFile.close();

	// Open the pins value file ready for writing to.
	valueFileName << FILESTRINGBASE << pin << VALUEFILE;
	valueFile.open(valueFileName.str().c_str());
	if (!valueFile.is_open()){
		std::cerr << "Couldn't create GPIOWriter on pin" << pin << "as the pin value file could not be opened." << std::endl;
		throw new std::runtime_error(NULL);
	}

	// Record the pin number
	this->pin = pin;

}

GPIOWriter::~GPIOWriter(){

	std::ofstream unexportFile;

	valueFile.close();

	// Unexport the required pin by writing the pin name to the gpio unexport file.
	unexportFile.open(UNEXPORTFILE.c_str());
	if (!unexportFile.is_open()){
		std::cerr << "Couldn't unexport the GPIOWriter pin" << std::endl;
		throw new std::runtime_error(NULL); 
	}
	unexportFile << pin;
	unexportFile.close();

}

void GPIOWriter::high(){

	// Check the pin value system file is open.
	if (!valueFile.is_open()){
		std::cerr << "Pin value file could not be accessed." << std::endl;
		throw new std::runtime_error(NULL);
	}

	// Set the pin value high.
	valueFile << HIGH;
}

void GPIOWriter::low(){

	// Check the pin value system file is open.
	if (!valueFile.is_open()){
		std::cerr << "Pin value file could not be accessed." << std::endl;
		throw new std::runtime_error(NULL);
	}

	// Set the pin value low.
	valueFile << LOW;
}