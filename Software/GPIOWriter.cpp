#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "GPIOWriter.hpp"

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
    std::ofstream valueFile;
    std::string directionFileName;

    // Check that the specified pin is valid.
    if (pin < 0 || pin > MAXPIN)
        throw new std::range_error("Couldn't create GPIOWriter as the pin is not valid.\n");

    // Export the required pin by writing the pin name to the gpio export file
    exportFile.open(EXPORTFILE.c_str());
    if(!exportFile.is_open()) 
        throw new std::runtime_error("Couldn't create GPIOWriter on pin as the export file could not be opened.\n" );
    exportFile << pin;
    exportFile.close();

    // Sets the direction of the pin to out so that it can be written to.
    directionFileName = FILESTRINGBASE + std::to_string(pin) + DIRECTIONFILE;
    directionFile.open(directionFileName.c_str());
    if (!directionFile.is_open())
        throw new std::runtime_error("Couldn't create GPIOWriter as the pin direction file could not be opened.\n");
    directionFile << OUT;
    directionFile.close();

    // Open the pins value file to check it can be written to.
    valueFileName = FILESTRINGBASE + std::to_string(pin) + VALUEFILE;
    valueFile.open(valueFileName.c_str());
    if (!valueFile.is_open())
        throw new std::runtime_error("Couldn't create GPIOWriter as the pin value file could not be opened.\n");
        valueFile.close();

    // Record the pin number
    this->pin = pin;

}

GPIOWriter::~GPIOWriter(){

    std::ofstream unexportFile;

    // Unexport the required pin by writing the pin name to the gpio unexport file.
    unexportFile.open(UNEXPORTFILE.c_str());
    if (!unexportFile.is_open())
        throw new std::runtime_error("Couldn't unexport the GPIOWriter pin.\n");
    unexportFile << pin;
    unexportFile.close();

}

void GPIOWriter::high(){

    std::ofstream valueFile;

    // Check the pin value system file is open.
    valueFile.open(valueFileName.c_str());
    if (!valueFile.is_open())
        throw new std::runtime_error("Pin value file could not be accessed.\n");

    // Set the pin value high.
    valueFile << HIGH;

    valueFile.close();

}

void GPIOWriter::low(){

    std::ofstream valueFile;

    // Check the pin value system file is open.
    valueFile.open(valueFileName.c_str());
    if (!valueFile.is_open())
        throw new std::runtime_error("Pin value file could not be accessed.\n");

    // Set the pin value low.
    valueFile << LOW;

    valueFile.close();

}
