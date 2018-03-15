#include <thread> 
#include <chrono> 
#include "PID.hpp"

static const int READY = 1;
static const int NOT_READY = 0;

PID::PID(int sample_time, int require_value, int output_clip, int p_coeff, int i_coeff, int d_coeff) {
  
  	// Assignin the proportion, integral and differential constants as specified.
    Kp = p_coeff;              
    Ki = i_coeff;
    Kd = d_coeff;
    
    // Set the target value, sample time and maximum output value as specified.
    this->required_value = required_value;
    this->sample_time = sample_time;
    this->output_clip = output_clip;
    
    // Assume that no previous error existed.
    previous_error = 0;	   
    integral = 0;
    output = 0;
    output_ready = NOT_READY;          

}

void PID::start_pid(){
	
	// Create and start a thread in which the PID iteration process will run.
	std::thread iteration_thread(&PID::iterate, this);
	
}
	

void PID::iterate() {

	int current_error, derivative, current_value;
	
	while(true){
	
		// Calulate the error from the target value.
		current_value = this->get_current_value();
		current_error = current_value - required_value;
		
		// Calulate the PID output.
		integral = integral + current_error*sample_time;
		derivative = (current_error - previous_error)/sample_time;	
		output = Kp*current_error + Ki*integral + Kd*derivative; 
		
		// Indicate that the output is ready for reading.
		output_ready = READY;   
		    
		// Record the current error so that it can be used by the next iteration.
		previous_error = current_error;
		
		// Ensure that the output is within the allowed limits.
		if (output > output_clip) output = output_clip;
        if (output < (-output_clip)) output = -output_clip;

		// Wait until the next sample time.
		std::this_thread::sleep_for(std::chrono::seconds(sample_time));
		
	}
}

int PID::update_required_value(int required_value){

	this->required_value = required_value;
	
	// Reset all accumulating variables.
    previous_error = 0;	   
    integral = 0;
    output = 0;
    
    // Output value is no longer ready to be read.
    output_ready = NOT_READY;          
    
}

int PID::get_pid_value(){

	output_ready == NOT_READY; // Indicate that the output is no longer ready for reading.
	return output;
	
}

bool PID::isReady(){ return (output_ready == READY); }




