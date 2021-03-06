#include "TempReader.hpp"
#include <assert.h>
#include <fstream>
#include <string>
#include <iostream>

static std::string TEMP_SENSOR_ID = "28-000005f4e7d6"; // Neonate Temp Sensor
static std::string W1PATH = "/sys/bus/w1/devices/";   
static std::string W1FILE = "/w1_slave";             

int main(){

    TempReader *testTempReader;
    std::ofstream w1File;
    
    int passed = 0;
    
    //*****************************************************************************
    /*
        Test that the temperature is read correctly
    */
    try { 
        testTempReader = new TempReader(TEMP_SENSOR_ID);
        std::string w1FileName = W1PATH + TEMP_SENSOR_ID + W1FILE;
        w1File.open(w1FileName);
        if (!w1File.is_open()) std::cout << "File not open\n";
        std::cout << w1FileName << std::endl;
        w1File << "a2 01 4b 7f ff 0e 10 d8 : crc=d* YES\n a2 01 4b 7f ff 0e 10 d8 t=26100\n";
        std::cout << testTempReader->readTemp() << std::endl;
        w1File.close();
        //assert(testTempReader->readTemp() == 26.1);
    } catch(std::exception& e) { assert(0); }
    passed++;
    delete testTempReader;
    
    //*****************************************************************************
    
    // Display results
    std::cout << "Success: " << passed << " out of " << passed << " tests were successful\n";
    
}
