#ifndef PWM_CTRL
#define PWM_CTRL

/*
    This class is used to control a PWM chip on a Raspberry Pi.
*/
class PWMCtrl {

    private:

        std::string periodFileName;         // Generated linux system PWM period file name based on chip number.
        std::string dutyCycleFileName;      // Generated linux system PWM duty cycle file name based on chip number.
        std::string enableFileName;         // Generated linux system PWM enable file name based on chip number.

        static const int MIN_PERIOD = 10000;
        static const int ENABLE = 1;
        static const int DISABLE = 0;
        static const int NUM_OF_CHIPS = 2;

    public:

        /*
            Creates a new PWMCtl for the specified PWM chip. 
            Throws a range error if the chip number is invalid.
            Throws a runtime error if the PWM module check was not successful
            or if the PWM module was not successfully exported.
        */
        PWMCtrl(int pwmchip);

        /* 
            Sets the period and duty cycle of the PWM chip.
            Must be called prior to enabling the PWM chip.
            Throws a range error if the period or duty cycle is invalid.
            Throws a runtime_error if the period or duty cycle was not written successfully.
        */
        void configure(int period, int dutyCycle);

        /*
            Enables the PWM chip.
            Throws a runtime_error if the linux system PWM enable file could not be accessed.
        */
        void enable();

        /*
            Disables the PWM chip.
            Throws a runtime_error if the linux system PWM enable file could not be accessed.
        */
        void disable();
};

#endif
