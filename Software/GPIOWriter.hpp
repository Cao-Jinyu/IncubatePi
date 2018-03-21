#ifndef GPIO_WRITER
#define GPIO_WRITER

/*
    This class is used to set up and write to a GPIO pin on a Raspberry Pi.
*/
class GPIOWriter{

    private:

        int pin;                    // The RPI pin on which this GPIOWriter is operating.
        std::string valueFileName;  // Stores the name of the pin value file.

    public:

        /*
            Creates a new GPIOWriter on the specified BCM pin.
            Throws an range error if the pin number is not valid.
            Throws a runtime error if the neccessary linux system files could not be opened.
        */
        GPIOWriter(int pin);

        /*
            Deconstructs a GPIOWriter and the pin is unexported.
            Throws a runtime error if the pin could not be unexported.
        */
        ~GPIOWriter();

        /*
            Sets the pin high.
            Throws a runtime error if the pin value file could not be accessed.
        */
        void high();

        /*
            Sets the pin low.
            Throws a runtime error if the pin value file could not be accessed.
        */
        void low();
};

#endif
