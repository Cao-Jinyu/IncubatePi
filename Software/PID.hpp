#ifndef PID_H
#define PID_H

/*
    This class is the PID controller.
*/
class PID {

    private:

        static const int Kp = 5;                  // TODO: Tune value
        static const int Ki = 5;                  // TODO: Tune value
        static const int Kd = 5;                  // TODO: Tune value

        int required_value;
        int sample_time;

        int previous_error;
        int integral;
        int derivative;

    public:

        /*
            Creates a new PID controller.
        */
        PID(int required_value, int sample_time, int p_coeff, int i_coeff, int   );

        /*
            Perform next iteration of PID output calculation.
        */
        int iterate(int current_value);
        
        int update_required_value()

        void find_PID_constants();
};

#endif
