#ifndef PWM_CTRL
#define PWM_CTRL

#include <string>

/*
    This class is used to control a PWM chip on a Raspberry Pi.
    It allows the period and duty cycle to be set
    and the PWM signal to be enabled and disabled as required.
*/
class PWMCtrl {

    private:

        std::string periodFileName;         // Name of linux system file were the PWM period should be written to.
        std::string dutyCycleFileName;      // As above for duty cycle.
        std::string enableFileName;         // As above for enable.

    public:

        /*
            Creates a new PWMCtl for the specified PWM chip.
            Throws a range error if the PWM chip number is invalid.
            Throws a runtime error if the PWM kernal module is not currently loaded
            or if the PWM chip was not successfully exported.
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
