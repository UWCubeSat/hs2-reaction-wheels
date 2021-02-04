#ifndef REACTION_WHEELS_PROTO_PID_H_
#define REACTION_WHEELS_PROTO_PID_H_

#include <Arduino.h>

/*
 * This class implements a PID controller given references to input, output, and setpoint
 * and gains for P, I, and D terms. Given an input and a setpoint, it can be used to calculate
 * an output to a system (plant) to drive the error to zero.
 */

namespace reaction_wheels_pid {

class PID {
    public:
        // Constructor: requires all parameters
        PID(double &_input, double &_output, double &_setpoint, const double _Kp, const double _Ki, const double _Kd);

        // Destructor
        ~PID();

        // Initializes error accumulator and error derivative.
        // Should be called after input and setpoint are usable
        // and before starting controller iteration.
        void Initialize();

        // Turns on integral clamping using the given min/max outputs.
        // Requires min < max, throws exception if not.
        void ClampingOn(double min, double max);
        // Turns off integral clamping
        void ClampingOff();

        // Turns on a low pass filter for the derivative using the given smoothing factor.
        void LowPassFilterOn(double factor);
        // Turns off low pass filter.
        void LowPassFilterOff();

        // Performs one iteration of the controller loop.
        void Iterate();

    private:
        double &_input;  // reference to controller input (plant sensor)
        double &_output;  // reference to controller output (plant command)
        double &_setpoint;  // reference to setpoint (desired plant result)

        const double _Kp;  // proportional gain
        const double _Ki;  // integral gain
        const double _Kd;  // derivative gain

        double _error_acc;  // error accumulator
        double _error_prev;  // previous error term (for calculating derivative)
        double _time_prev;  // time of previous iteration (for calculating derivative)

        // Integral clamping settings
        bool _clamping;  // set true to turn on, false to turn off
        double _clamp_min;  // minimum saturation value
        double _clamp_max;  // maximum saturation value

        // Derivative low pass filter settings
        // E_t = (1 - r) * E_t-1 + r * de/dt
        // where r = smoothingFactor, e = error
        bool _low_pass_filter;  // set true to turn on, false to turn off
        double _smoothing_factor;  // smoothing factor 0-1; smaller for greater smoothing
        double _exp_moving_average_prev;  // previous exponential moving average term

        /*
         * Checks if the controller should be clamped. Clamp if both are true:
         *   -output with the integral term is outside of accepted range
         *   -output and current error are the same sign
         * Does not check if clamping is turned on.
         * Takes in the current error and current integral term.
         */
        bool ShouldClamp(double error_curr, double integral_term_current);
};

}  // namespace reaction_wheels_pid

#endif  // _PID_H_