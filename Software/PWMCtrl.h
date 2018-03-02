#ifndef PWM_CTRL
#define PWM_CTRL

/*
    This class is used to control the PWM1 chip on the RPI
*/
class PWMCtrl {
    private:
        static int setupCheck;
        static int configureCheck;
        static int period;

        static int moduleCheck();
        static int exportPWM();
        static int setPeriod(int period);
        static int setDutyCycle(int dutyCycle);

    public:
        PWMCtrl();
        static int setup();
        static int configure(int period, int dutyCycle);
        static int enable();
        static int disable();
};

#endif
