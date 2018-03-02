#ifndef PWM_CTRL
#define PWM_CTRL

/*
    This class is used to control a PWM chip on a Raspberry Pi
*/
class PWMCtrl {

    private:

        std::string periodFileName;         // Generated linux system PWM period file name based on chip no.
        std::string dutyCycleFileName;      // Generated linux system PWM duty cycle file name based on chip no.
        std::string enableFileName;         // Generated linux system PWM enable file name based on chip no.

    public:

        /*
            Creates a new PWMctl for the specified pwmchip. 
            Throws an exception if the chip number is invalid
            Throws a runtime error if the pwm module check was not successful
            or if the pwm module was not successfully exported
        */
        PWMCtrl(int pwmchip);

        /* 
            Sets the period and duty cycle of the pwm chip
            Throws an exception if the period or duty cycle is invalid
            (period < MIN_PERIOD or duty cycle > period).
            Throws a runtime_error if the period or duty cycle was not written successfully.
        */
        void configure(int period, int dutyCycle);

        /*
            Enables the pwm chip.
            Throws a runtime_error if the linux system PWM enable file could not be accessed.
        */
        void enable();

        /*
            Disables the pwm chip.
            Throws a runtime_error if the linux system PWM enable file could not be accessed.
        */
        void disable();
};

#endif
