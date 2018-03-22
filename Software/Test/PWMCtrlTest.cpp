#include "PWMCtrl.hpp"
#include <assert.h>
#include <fstream>
#include <string>
#include <iostream>

static const int INVALID_CHIP_NUM = 2;
static const int VALID_CHIP_NUM = 1;
static const int INVALID_PERIOD = 9999;
static const int VALID_PERIOD = 10000;
static const int INVALID_DUTY_CYCLE_1 = -1;
static const int INVALID_DUTY_CYCLE_2 = 10001;
static const int VALID_DUTY_CYCLE = 5000;

int main(){
    
    PWMCtrl *testPWMCtrl;
    std::ifstream enableFile;
    std::string enableFileName;
    bool test_success;
    
    int passed = 0;
    
    //*****************************************************************************
    /*
        Test that an invalid chip number results in a range error
    */
    test_success = false;
    try { testPWMCtrl = new PWMCtrl(INVALID_CHIP_NUM); }
    catch(std::range_error& e) { test_success = true; }
    catch(std::exception& e) { assert(0); }
    assert(test_success);
    passed++; 
    delete testPWMCtrl;
    
    //*****************************************************************************
    /*
        Test that an invalid period results in a range error
    */
    test_success = false;
    try { 
        testPWMCtrl = new PWMCtrl(VALID_CHIP_NUM);
        testPWMCtrl->configure(INVALID_PERIOD,VALID_DUTY_CYCLE);
    }
    catch(std::range_error& e) { test_success = true; }
    catch(std::exception& e) { assert(0); }
    assert(test_success);
    passed++; 
    delete testPWMCtrl;
    
    //*****************************************************************************
    /*
        Test that an invalid negative duty cycle results in a range error
    */
    test_success = false;
    try { 
        testPWMCtrl = new PWMCtrl(VALID_CHIP_NUM);
        testPWMCtrl->configure(VALID_PERIOD,INVALID_DUTY_CYCLE_1);
    }
    catch(std::range_error& e) { test_success = true; }
    catch(std::exception& e) { assert(0); }
    assert(test_success);
    passed++; 
    delete testPWMCtrl;
    
    //*****************************************************************************
    /*
        Test that an invalid large duty cycle results in a range error
    */
    test_success = false;
    try { 
        testPWMCtrl = new PWMCtrl(VALID_CHIP_NUM);
        testPWMCtrl->configure(VALID_PERIOD,INVALID_DUTY_CYCLE_2);
    }
    catch(std::range_error& e) { test_success = true; }
    catch(std::exception& e) { assert(0); }
    assert(test_success);
    passed++; 
    delete testPWMCtrl;
    
    //*****************************************************************************
    /*
        Test that "enable" method correct sets the PWM chip high
    */
    try { 
        testPWMCtrl = new PWMCtrl(VALID_CHIP_NUM);
        testPWMCtrl->configure(VALID_PERIOD,VALID_DUTY_CYCLE);
        testPWMCtrl->enable();
    } catch(std::exception& e) { assert(0); }
    enableFileName = "/sys/class/pwm/pwmchip0/pwm" + std::to_string(VALID_CHIP_NUM) + "/enable"; 
    enableFile.open(enableFileName.c_str());
    assert(enableFile.get() == '1');
    passed++;
    enableFile.close();
    delete testPWMCtrl;
    
    //*****************************************************************************
    /*
        Test that "disable" method correct sets the PWM chip low
    */
    try { 
        testPWMCtrl = new PWMCtrl(VALID_CHIP_NUM);
        testPWMCtrl->configure(VALID_PERIOD,VALID_DUTY_CYCLE);
        testPWMCtrl->enable(); // Assures that the chip was not just low anyway
        testPWMCtrl->disable();
    } catch(std::exception& e) { assert(0); }
    enableFileName = "/sys/class/pwm/pwmchip0/pwm" + std::to_string(VALID_CHIP_NUM) + "/enable"; 
    enableFile.open(enableFileName.c_str());
    assert(enableFile.get() == '0');
    passed++;
    enableFile.close();
    delete testPWMCtrl;
    
    //*****************************************************************************
    
    // Display results
    std::cout << "Success: " << passed << " out of " << passed << " tests were successful\n";
    
}
    

