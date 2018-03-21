#ifndef PID_H
#define PID_H

#include <mutex>

/*
    This abstract class implements a general PID controller.
    Any concrete subclasse must implement the virtual get_current_value() function which
    specifies how to obtain the current value of the controlled variable for the PID process.
*/
class PID {

    private:

        float Kp;               // PID proportional coefficient.
        float Ki;               // PID integral coefficient.
        float Kd;               // PID differential coefficient.

        float required_value;   // The target value of the PID controller.
        int sample_time;        // The time in seconds between PID iterations.
        float min;              // The lower limiting value of the PID output.
        float max;              // The upper limiting value of the PID output.
        float integral;         // Keeps track of the integral error.
        float previous_error;   // The calculated error from the previous iteration.
        float output;           // Current output value of the PID controller.
        bool output_ready;      // Signifies that the output value is ready to be read.

        std::mutex protect;     // Ensures correct operation of parallelly executed methods.

        // Gets the current value of the variable being monitored by the PID controller.
        virtual float get_current_value() = 0;

        // Iterates the PID process indefinitely.
        void iterate();

    public:

        // Creates a new PID controller with the specified PID coefficients.
        PID(int sample_time, float required_value, float min, float max, float p_coeff, float i_coeff, float d_coeff);

        // Should be called once to begin the PID iteration process
        void start_pid();

        // Allows the target value of the PID controller to be changed.
        void update_required_value(float required_value);

        // Returns true if the PID output value is ready to read, false otherwise.
        bool isReady();

        // Returns the current value of the PID output.
        float get_pid_value();

};

#endif
