#ifndef READ_TEMP
#define READ_TEMP

#include <string>

/*
    This class is used to read the temperature from a DS18B20 using the RPI's one wire interface.
*/
class TempReader{

    private:
        std::string w1FileName;     // Name of the 1-wire linux system file associated with this device

    public:

        /*
            Adds the neccessary loadable kernel modules to use the RPI 1-wire interface.
            This must be run prior to executing any other functions in this class.
            Throws a runtime error if the modules were not successfully loaded.
        */
        static void loadKernelModules();

        /*
            Creates a new TempReader for the specified device.
            Throws a runtime error if the device name was not valid.
        */
        TempReader(std::string deviceName);

        /*
            Returns the measured temperature in degrees celcius from the DS18B20 device.
            Throws a runtime error if the 1-wire device file could not be read correctly.
        */
        float readTemp();

};

#endif
