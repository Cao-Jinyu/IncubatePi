#ifndef PID_H
#define PID_H

/*
    This abstract class implements a general PID controller.
  	Any concrete subclasse must implement the virtual get_current_value() function which 
  	specifies how to obtain the current value of the controlled variable for the PID process.
*/
class PID {

    private:

        int Kp;                 // PID proportional coefficient.
        int Ki;                 // PID integral coefficient.
        int Kd;                 // PID differential coefficient.

        int required_value;		// The target value of the PID controller.
        int sample_time;		// The time in seconds between PID iterations.
        int output_clip;		// The symmetric (positive and negative) limiting value of the PID output.
        int integral;			// Keeps track of the integral error.

        int previous_error;		// The calculated error from the previous iteration.
        int output;				// Current output value of the PID controller.
        int output_ready;		// Signifies that the output value is ready to be read.
        
        // Gets the current value of the variable being monitored by the PID controller.
        virtual int get_current_value() = 0;
        
        // Iterates the PID process indefinitely.
        void iterate();

    public:

        // Creates a new PID controller with the specified PID coefficients.
        PID(int sample_time, int require_value, int output_clip, int p_coeff, int i_coeff, int d_coeff);
        	
        // Begins the PID iteration process
        void start_pid();   
        
        // Allows the target value of the PID controller to be changed.
        int update_required_value(int required_value);
        
        // Returns true if the PID output value is ready to read, false otherwise
        bool isReady();
        
        // Returns the current value of the PID output.
        int get_pid_value();

};

#endif
