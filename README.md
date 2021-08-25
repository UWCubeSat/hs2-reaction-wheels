# HSL Reaction Wheels Prototyping   
This repository contains all sources which implement functionality of a reaction wheel driver. There are 4 primary modules which allow us to control a single Faulhaber motor.

## PID
The PID module exposes an interface for a simple Proportional-Integral-Derivative controller. The reaction wheel motor speed is set via this interface in the software.

## DRV10970
The DRV10970 module exposes an interface for the DRV10970 Motor Driver IC. It exposes all functions of the IC necessary for controlling a single motor.

## BNO055
The BNO055 module exposes an interface for the BNO055 Inertial Measurement Unit IC. This IC generates fused sensor data which informs the systems notion of its dynamic state.

## PWM
The PWM module translates the current speed value from the PID controller for the motor into a frequency which the DRV10970 module can interpret to control the motor speed.