#include "PWMCtrl.h"
#include <stdlib.h>

/*
    uname -r                        : Check kernel version
    /boot/config.txt                : Edit this file
    dtoverlay=pwm,pin=19,func=2     : Add this line
*/

static const int DEFAULT_PERIOD = 40000;    // Fan PWM period in nanoseconds

int main () {

    // Setup PWM controller
 	PWMCtrl *pwm = new PWMCtrl();
 	if (pwm->setup()) {
        delete pwm;
        return 1;
    }

 	if (pwm->configure(DEFAULT_PERIOD,0)) {
        delete pwm;
        return 1;
    }
 	pwm->enable();

    // PID controller

    const int sample_rate = 0;
    const int body_temp = 37;
    int neonate_temp = 0;

    int previous_error = 0;
    int current_error = 0;
    int integral = 0;
    int derivative = 0;
    int Kp = 5;                  // TODO
    int Ki = 5;                  // TODO
    int Kd = 5;                  // TODO
    int output = 0;

    while(true) {
        // Read temperatures
        neonate_temp = 0;        // Read from temp sensor

        current_error = abs(body_temp - neonate_temp);
        integral = integral + (current_error*sample_rate);
        derivative = abs(current_error - previous_error)/sample_rate;
        output = Kp*current_error + Ki*integral + Kd*derivative;         // May require bias also?
        previous_error = current_error;
    }

    // Disable PWM controller
    pwm->disable();
    delete pwm;
}
