#ifndef GPIO_WRITER
#define GPIO_WRITER

/*
    This class is used to write to a GPIO pin on a Raspberry Pi.
*/
class GPIOWriter{

    private:

        int pin;                    // The RPI pin on which this GPIOWriter is operating.
        std::ofstream valueFile;    // The linux system file to which the pin value should be written. 

    public:

        /*
            Creates a new GPIOWriter on the specified BCM pin.
            Throws an range error if the pin is not valid.
            Throws a runtime error if the neccessary linux system files could not be opened.
        */
        GPIOWriter(int pin);

        /*
            Deconstructs a GPIOWriter. 
            The linux system files are closed and the pin is unexported.
            Throws a runtime errorif the pin could not be unexported. 
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
