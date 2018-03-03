#include "PID.h"

PID::PID(int required_value, int sample_time) {

    this->required_value = required_value;
    this->sample_time = sample_time;

    previous_error = 0;
    integral = 0;
    derivative = 0;
}

int PID::iterate(int current_value) {

    int current_error = 0;
    int output = 0;

    current_error = current_value - required_value;
    integral = integral + current_error*this->sample_time;
    derivative = (current_error - this->previous_error)/sample_time;
    output = this->Kp*current_error + Ki*integral + Kd*derivative;         // May require bias also?
    this->previous_error = current_error;

    return output;
}

void PID::find_PID_constants() {
    // TODO
}
