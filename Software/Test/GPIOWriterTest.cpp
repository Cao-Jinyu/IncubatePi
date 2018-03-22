#include "GPIOWriter.hpp"
#include <assert.h>
#include <fstream>
#include <string>
#include <iostream>

static const int INVALID_PIN = 27;
static const int VALID_PIN = 10;

int main(){
    
    GPIOWriter *testGPIOWriter;
    std::ifstream valueFile;
    std::string valueFileName;
    
    int passed = 0;
    
    //*****************************************************************************
    /*
        Test that invalid pin number results in a range error
    */
    bool test_success = false;
    try { testGPIOWriter = new GPIOWriter(INVALID_PIN); }
    catch(std::range_error& e) { test_success = true; }
    catch(std::exception& e) { assert(0); }
    assert(test_success);
    passed++; 
    delete testGPIOWriter;

   
    //*****************************************************************************
    /*
        Test that "high" method correct sets the pin high
    */
    try { 
        testGPIOWriter = new GPIOWriter(VALID_PIN); 
        testGPIOWriter->high(); 
    } catch(std::exception& e) { assert(0); }
    valueFileName = "/sys/class/gpio/gpio" + std::to_string(VALID_PIN) + "/value";
    valueFile.open(valueFileName.c_str());
    assert(valueFile.get() == '1');
    passed++;
    valueFile.close();
    delete testGPIOWriter;
    
        
    //*****************************************************************************
    /*
        Test that "low" method correct sets the pin low
    */
    try { 
        testGPIOWriter = new GPIOWriter(VALID_PIN); 
        testGPIOWriter->high(); // Assures that the pin was not just low anyway
        testGPIOWriter->low(); 
    } catch(std::exception& e) { assert(0); }
    valueFileName = "/sys/class/gpio/gpio" + std::to_string(VALID_PIN) + "/value";
    valueFile.open(valueFileName.c_str());
    assert(valueFile.get() == '0');
    passed++;
    valueFile.close();
    delete testGPIOWriter;
    
    //*****************************************************************************
    
    // Display results
    std::cout << "Success: " << passed << " out of " << passed << " tests were successful\n";
    
}