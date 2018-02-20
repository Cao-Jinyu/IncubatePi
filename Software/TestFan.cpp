#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>  

using namespace std;     
 
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define DELAY 10000000

#define DEFAULT_PERIOD 40000		// PWM period in nanoseconds
#define MIN_PERIOD 10000	        // Minimum allowed PWM period	
 	
#define PWMPORT 1 	
#define ENABLE 1
#define DISABLE 0 

#define EXPORTFILE      /sys/class/pwm/pwmchip0/export
#define PERIODFILE      /sys/class/pwm/pwmchip0/pwm1/period
#define DUTYCYCLEFILE   /sys/class/pwm/pwmchip0/pwm1/duty_cycle
#define ENABLEFILE      /sys/class/pwm/pwmchip0/pwm1/enable

#define TEMPFILE        temp
#define MODULECHECK     lsmod | grep fjes > TEMPFILE


class PWMCtl{

    private:
        int setupCheck;
        int configureCheck;
        int period;
        
        int moduleCheck();
        int exportPWM();    
        int setPeriod(int period);
        int setDutyCycle(int dutyCycle);
        

    
    public:     
        PWMCtl();        
        int setup();
        int configure(int period, int dutyCycle); 
        int enable();
        int disable();
};

PWMCtl::PWMCtl(){
    setupCheck = 0;
    configureCheck = 0;  
    period = 0;
}
 
/*
    Calls the neccessary functions to set up the PWM channel.
    Returns 0 and sets setupCheck on success.
	Returns 1 on failure.
*/
int PWMCtl::setup(){
    if (moduleCheck()) return 1;
    if (exportPWM()) return 1;
    setupCheck = 1;
    return 0;
}

/*
    Calls the neccessary functions to configure the PWM channel.
    Returns 0 and sets configureCheck on success.
    Returns 1 on failure.
*/
int PWMCtl::configure(int period, int dutyCycle){
    if (!setupCheck){
        cerr << "Couldn't configure PWM channel as setup function has not been performed" << endl;
        return 1;
    }
    if (setPeriod(period)) return 1;
    if (setDutyCycle(dutyCycle)) return 1;
    configureCheck = 1;
    cout << "Period: " << period << ", Duty Cycle: " << dutyCycle << endl;
    return 0;
}

/* 
	Checks that the PWM module is loaded in the linux kernel.
	This is done by searching the linux module list and confirming that 
	there is at least one module containing the string 'pwm'.
	Returns 0 on success.
	Returns 1 on failure.
*/
int PWMCtl::moduleCheck(){
		
	ifstream infile;
	string line;
	
    // Execute Linux command that writes any modules containing the string pwm to a file temp.
	if (system(TOSTRING(MODULECHECK)) == -1){
		cerr << "PWM Module Check Error: Couldn't execute lsmod linux system command" << endl; 
		return 1;
	}
	
	// Opens the file where the output of the command was piped to.
	infile.open(TOSTRING(TEMPFILE));
	if (!infile.is_open()) {
		cerr << "PWM Module Check Error: Couldn't open temp file" << endl;
		return 1; 
	}	
	
	// Checks that the file is not empty and prints out the module name. 
	if (!getline(infile,line) ){
		printf("Couldn't find the neccessary kernal modules\n");
		infile.close();
  		return 1;
	} else cout << "Module: " << line << endl;	 	
	infile.close();
	return 0;
}

/*
    Exports the PWM channel.
	Returns 0 on success.
	Returns 1 on failure.
    
*/
int PWMCtl::exportPWM(){

    ofstream outfile;
	
	// Open the linux PWM export file
	outfile.open(TOSTRING(EXPORTFILE));
	if(!outfile.is_open()) {
		cerr << "Couldn't carry out export as couldn't open PWM export linux system file" << endl;
		return 1;   
	}

	// Write out export
	outfile << PWMPORT << endl;
	outfile.close();
	return 0;
}

/*
    Sets the PWM period by writing to the neccesary linux system file.
    Returns 0 on success.
    Returns 1 on failure.
*/
int PWMCtl::setPeriod(int period){
	
	ofstream outfile;
	
	// Check that the period is valid.
	if (period < MIN_PERIOD){
		cerr << "Invalid period. Must be greater than" << MIN_PERIOD << "." << endl;
		return 1;
	}
	
	// Open the linux PWM period file.
	outfile.open(TOSTRING(PERIODFILE));
	if(!outfile.is_open()) {
		cerr << "Couldn't update period as couldn't open PWM period linux system file" << endl;
		return 1;   
	}

	// Write out period
	outfile << period << endl;
	this->period = period;
	outfile.close();
	return 0;
}

/*
    Sets the PWM duty cycle by writing to the neccesary linux system file
    Returns 0 on success
    Returns 1 on failure
*/
int PWMCtl::setDutyCycle(int dutyCycle){
	
	ofstream outfile;
	
	// Check that the duty cycle is valid
	if (dutyCycle < 0 || dutyCycle > period){
		cerr << "Invalid duty cycle. Must be between 0 and the period." << endl;
		return 1;
	}
	
	// Open the linux PWM duty cycle file
	outfile.open(TOSTRING(DUTYCYCLEFILE));
	if(!outfile.is_open()) {
		cerr << "Couldn't update duty cycle as couldn't open PWM duty cycle linux system file" << endl;
		return 1;   
	}

	// Write out duty cycle 
	outfile << dutyCycle << endl;
	outfile.close();
	return 0;
}


/*
    Enables PWM by writing to the neccesary linux system file
    Returns 0 on success
    Returns 1 on failure
*/
int PWMCtl::enable(){
	
	ofstream outfile;
	
	// Open the linux PWM enable file
	outfile.open(TOSTRING(ENABLEFILE));
	if(!outfile.is_open()) {
		cerr << "Couldn't enable PWM as couldn't open PWM enable linux system file" << endl;
		return 1;   
	}

	// Write out enable
	outfile << ENABLE << endl;
	outfile.close();
	return 0;
}

/*
    Disables PWM by writing to the neccesary linux system file
    Returns 0 on success
    Returns 1 on failure
*/
int PWMCtl::disable(){
	
	ofstream outfile;
	
	// Open the linux PWM enable file
	outfile.open(TOSTRING(ENABLEFILE));
	if(!outfile.is_open()) {
		cerr << "Couldn't disable PWM as couldn't open PWM enable linux system file" << endl;
		return 1;   
	}

	// Write out disable
	outfile << DISABLE << endl;
	outfile.close();
	return 0;
}

int main (void) {

	/* 
		uname -r       					: Check kernel version
		/boot/config.txt				: Edit this file
		dtoverlay=pwm,pin=13,func=4		: Add this line		
	*/

 	PWMCtl *pwm = new PWMCtl();
 	if (pwm->setup()) return 1;
 	if (pwm->configure(DEFAULT_PERIOD,0)) return 1;
 	pwm->enable();
 	
 	//for (;;){
 	    for (int i=0; i<=4; i++){
 	        pwm->configure(DEFAULT_PERIOD, DEFAULT_PERIOD * i/4);
 	        usleep(DELAY);
 	    } 
    //}
    
    
    pwm->disable();
    delete pwm;
    	    		
}

