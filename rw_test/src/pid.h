#ifndef PID_H_
#define PID_H_

/*
 * This class implements a PID controller given references to input, output, and setpoint
 * and gains for P, I, and D terms. Given an input and a setpoint, it can be used to calculate
 * an output to a system (plant) to drive the error to zero.
 */

class PID {
    public:
        // Constructor: requires all parameters
        PID(float &_input, float &_output, float &_setpoint, const float _Kp, const float _Ki, const float _Kd);

        // Destructor
        ~PID();

        // Initializes error accumulator and error derivative.
        // Should be called after input and setpoint are usable
        // and before starting controller iteration.
        void Initialize();

        // Turns on integral clamping using the given min/max outputs.
        // Requires min < max, does nothing otherwise.
        void ClampingOn(float min, float max);
        // Turns off integral clamping
        void ClampingOff();

        // Turns on a low pass filter for the derivative using the given smoothing factor.
        void LowPassFilterOn(float factor);
        // Turns off low pass filter.
        void LowPassFilterOff();

        // Performs one iteration of the controller loop.
        void Iterate();

        // Returns the error on the previous iteration of the PID loop.
        float GetError() { return _error_prev; }

    private:
        float &_input;  // reference to controller input (plant sensor)
        float &_output;  // reference to controller output (plant command)
        float &_setpoint;  // reference to setpoint (desired plant result)

        const float _Kp;  // proportional gain
        const float _Ki;  // integral gain
        const float _Kd;  // derivative gain

        float _error_acc;  // error accumulator
        float _error_prev;  // previous error term (for calculating derivative)
        unsigned long _time_prev;  // time of previous iteration (for calculating derivative)

        // Integral clamping settings
        bool _clamping;  // set true to turn on, false to turn off
        float _clamp_min;  // minimum saturation value
        float _clamp_max;  // maximum saturation value

        // Derivative low pass filter settings
        // E_t = (1 - r) * E_t-1 + r * de/dt
        // where r = smoothingFactor, e = error
        bool _low_pass_filter;  // set true to turn on, false to turn off
        float _smoothing_factor;  // smoothing factor 0-1; smaller for greater smoothing
        float _exp_moving_average_prev;  // previous exponential moving average term

        /*
         * Checks if the controller should be clamped. Clamp if both are true:
         *   -output with the integral term is outside of accepted range
         *   -output and current error are the same sign
         * Does not check if clamping is turned on.
         * Takes in the current error and current integral term.
         */
        bool ShouldClamp(float error_curr, float integral_term_current);

        float millis();
};

#endif  // PID_H_
