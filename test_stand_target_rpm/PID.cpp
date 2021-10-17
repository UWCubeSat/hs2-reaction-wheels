#include "PID.h"

using namespace reaction_wheels_pid;

PID::PID(float &input, float &output, float &setpoint, float &Kp, const float Ki, const float Kd)
: _input(input), _output(output), _setpoint(setpoint), _Kp(Kp), _Ki(Ki), _Kd(Kd),
  _error_acc(0), _error_prev(0), _time_prev(0), _clamping(false), _clamp_min(0), _clamp_max(0), _exp_moving_average_prev(0) {
}

PID::~PID() {

}

void PID::Initialize() {
    _error_acc += _setpoint - _input;
    _error_prev = _setpoint - _input;
    _time_prev = (float) millis();
}

void PID::ClampingOn(float min, float max) {
    if (min < max) {
        _clamp_min = min;
        _clamp_max = max;
        _clamping = true;
    }   
}

void PID::ClampingOff() {
    _clamping = false;
}

bool PID::ShouldClamp(float error_curr, float integral_term_curr) {
    return (_output + integral_term_curr <= _clamp_min || _output + integral_term_curr >= _clamp_max) && ( (_output < 0) == (error_curr < 0) );
}

void PID::LowPassFilterOn(float factor) {
    // Set initial derivative exponential moving average to just the derivative
    float time_curr = (float) millis();
    float time_elapsed = time_curr - _time_prev;
    float error_curr = _setpoint - _input;
    _exp_moving_average_prev = (error_curr - _error_prev) / time_elapsed;

    _smoothing_factor = factor;
    _low_pass_filter = true;
}

void PID::LowPassFilterOff() {
    _low_pass_filter = false;
}

void PID::Iterate() {
    float time_curr = (float) millis();
    float time_elapsed = time_curr - _time_prev;  // time elapsed in milliseconds
    float error_curr = _setpoint - _input;
    float error_rate = (error_curr - _error_prev) / time_elapsed;  // error per time in millis
    _output = (_Kp * error_curr);  // leave out integral and derivative terms to check for clamping and low pass filter first

    float error_acc_now = error_curr * time_elapsed;  // the error accumulated since the last iteration
    float integral_term = _Ki * (error_acc_now + _error_acc);
    // leave out integral term if clamping and saturated
    if (!( _clamping && ShouldClamp(error_curr, integral_term) ))  {
        _error_acc += error_acc_now;
        _output += integral_term;
    }

    // Either use low pass filter for derivative term, or just use simple derivative
    float derivative_term = _Kd;
    if (_low_pass_filter) {
        float exp_moving_average = (1 - _smoothing_factor) * _exp_moving_average_prev + _smoothing_factor * error_rate;
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