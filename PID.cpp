#include "PID.h"

using namespace reaction_wheels_pid;

PID::PID(double &input, double &output, double &setpoint, const double Kp, const double Ki, const double Kd)
: _input(input), _output(output), _setpoint(setpoint), _Kp(Kp), _Ki(Ki), _Kd(Kd),
  _error_acc(0), _error_prev(0), _time_prev(0), _clamping(false), _clamp_min(0), _clamp_max(0), _exp_moving_average_prev(0) {
}

PID::~PID() {

}

void PID::Initialize() {
    _error_acc += _setpoint - _input;
    _error_prev = _setpoint - _input;
    _time_prev = (double) millis();
}

void PID::ClampingOn(double min, double max) {
    if (min < max) {
        _clamp_min = min;
        _clamp_max = max;
        _clamping = true;
    }   
}

void PID::ClampingOff() {
    _clamping = false;
}

bool PID::ShouldClamp(double error_curr, double integral_term_curr) {
    return (_output + integral_term_curr <= _clamp_min || _output + integral_term_curr >= _clamp_max) && ( (_output < 0) == (error_curr < 0) );
}

void PID::LowPassFilterOn(double factor) {
    // Set initial derivative exponential moving average to just the derivative
        //! ^ or do we want to leave it as 0?
    double time_curr = (double) millis();
    double time_elapsed = time_curr - _time_prev;
    double error_curr = _setpoint - _input;
    _exp_moving_average_prev = (error_curr - _error_prev) / time_elapsed;

    _smoothing_factor = factor;
    _low_pass_filter = true;
}

void PID::LowPassFilterOff() {
    _low_pass_filter = false;
}

void PID::Iterate() {
    double time_curr = (double) millis();
    double time_elapsed = time_curr - _time_prev;
    double error_curr = _setpoint - _input;
    double error_rate = (error_curr - _error_prev) / time_elapsed;
    _output = (_Kp * error_curr) + (_Kd * error_rate);  // leave out integral and derivative terms to check for clamping and low pass filter first

    double error_acc_now = error_curr * time_elapsed;  // the error accumulated since the last iteration
    double integral_term = _Ki * (error_acc_now + _error_acc);
    // leave out integral term if clamping and saturated
    if (!( _clamping && ShouldClamp(error_curr, integral_term) ))  {
        _error_acc += error_acc_now;
        _output += integral_term;
    }

    // Either use low pass filter for derivative term, or just use simple derivative
    double derivative_term = _Kd;
    if (_low_pass_filter) {
        double exp_moving_average = (1 - _smoothing_factor) * _exp_moving_average_prev + _smoothing_factor * error_rate;
        derivative_term *= exp_moving_average;
        _exp_moving_average_prev = exp_moving_average;
    }
    else {
        derivative_term *= error_rate;
    }
    _output += derivative_term;

    // Iterate previous time and error
    _time_prev = time_curr;
    _error_prev = error_curr;
}