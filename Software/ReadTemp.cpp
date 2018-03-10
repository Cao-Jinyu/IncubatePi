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
    if (system(W1GPIO.c_str()) == -1 || system(W1THERM.c_str()) == -1)
        throw new std::runtime_error("Couldn't load the neccessary 1-wire kernal modules.\n");

}

TempReader::TempReader(std::string deviceName){

    std::ifstream w1File;

    // Construct the full file name of the 1-wire device using the specified device name.
    w1FileName = W1PATH + deviceName + W1FILE;

    // Open the 1-wire device file to check it can be accessed.
    w1File.open(w1FileName.c_str());
    if(!w1File.is_open())
        throw new std::runtime_error("Couldn't create TempReader as device name is not valid.\n");

    w1File.close();

}

float TempReader::readTemp(){

    int length;
    char * buffer;
    char * start;
    char * end;
    float temp;
    std::ifstream w1File;

    //  Open the 1-wire device file ready for reading.
    w1File.open(w1FileName.c_str());
    if (!w1File.is_open())
        throw new std::runtime_error("Couldn't read the temperature as the 1-wire device file could not be accessed.\n");

    // Get the length of the file contents.
    w1File.seekg (0, w1File.end);
    length = w1File.tellg();
    w1File.seekg (0, w1File.beg);

    // Check the file is not empty.
    if (!length){
        w1File.close();
        throw new std::runtime_error("Couldn't read the temperature as the 1-wire device file was empty.\n");
    }

    // Read the contents of the file.
    buffer = new char[length+1];
    w1File.read(buffer,length+1);

    // Extract the temperature value from the file contents.
    // Temperature must be divided by 1000 to get value in degrees celcius.
    start = strstr(buffer, "t=");
    if (!start){
        w1File.close();
        throw new std::runtime_error("Couldn't read the temperature as the 1-wire device contents was not valid.\n");
    }

    start = start + 2;
    end = start + 7;
    temp = strtof(start, &end) / 1000;

    // Free up the buffer storage and close the reading file.
    delete buffer;
    w1File.close();

    return temp;

}
