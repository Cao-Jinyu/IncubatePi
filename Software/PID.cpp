#include <thread> 
#include <chrono> 
#include "PID.hpp"

PID::PID(int sample_time, float require_value, float min, float max, float p_coeff, float i_coeff, float d_coeff) {
  
  	// Assign the proportion, integral and differential constants as specified.
    Kp = p_coeff;              
    Ki = i_coeff;
    Kd = d_coeff;
    
    // Set the target value, sample time and maximum output value as specified.
    this->required_value = required_value;
    this->sample_time = sample_time;
    this->min = min;
    this->max = max;
    
    // Assume that no previous error existed.
    previous_error = 0;	   
    integral = 0;
    output = 0;
    output_ready = false;          

}

void PID::start_pid(){
	
	// Create and start a thread in which the PID iteration process will run.
	std::thread iteration_thread(&PID::iterate, this);
	
}
	
void PID::iterate() {

	float current_error, derivative, current_value;
	
	while(true){
        
        // Ensure synchronization with other methods.
        protect.lock();
	
		// Calulate the error from the target value.
		current_value = this->get_current_value();
		current_error = current_value - required_value;
		
		// Calulate the PID output.
		integral = integral + current_error*sample_time;
		derivative = (current_error - previous_error)/sample_time;	
		output = output - (Kp*current_error + Ki*integral + Kd*derivative); 
        
        // Ensure that the output is within the allowed limits.
		if (output > max) output = max;
        if (output < min) output = min;
		
		// Indicate that the output is ready for reading.
		output_ready = true;   
		    
		// Record the current error so that it can be used by the next iteration.
		previous_error = current_error;
        
        // End of protected segment.
        protect.unlock();

		// Wait until the next sample time.
		std::this_thread::sleep_for(std::chrono::seconds(sample_time));
		
	}
}

void PID::update_required_value(int required_value){
    
    // Ensure synchronization with other methods.
    protect.lock();

	this->required_value = required_value;
	
	// Reset all accumulating variables.
    previous_error = 0;	   
    integral = 0;
    output = 0;
    
    // Output value is no longer ready to be read.
    output_ready = false;  

    // End of protected segment.
    protect.unlock();
    
}

float PID::get_pid_value(){
    
    float protected_output;

    // Ensure synchronization with other methods.
    protect.lock();
    
	output_ready = false; // Indicate that the output is no longer ready for reading.
    protected_output = output;
    
    // End of protected segment.
    protect.unlock();
    
	return protected_output;
	
}

bool PID::isReady(){ return (output_ready); }




