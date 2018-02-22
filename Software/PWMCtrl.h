#ifndef PWM_CTRL
#define PWM_CTRL

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
