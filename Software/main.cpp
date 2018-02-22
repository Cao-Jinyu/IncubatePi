#include "PWMCtrl.h"

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



    // Disable PWM
    pwm->disable();
    delete pwm;
}
