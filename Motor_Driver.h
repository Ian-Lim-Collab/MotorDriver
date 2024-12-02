#pragma once 
#include "mbed.h"

#define SPEED_UPDATE_RATE 150000.0F
#define SPEED_UPDATE_RATE_US 150000us

#define DEBOUNCE_PERIOD 14000us
#define MOTOR_PWM_PERIOD_US 2150

class Motor_Driver{
    public:
        Motor_Driver(PinName motor_pin, PinName tacho_pin);
        int getSpeed();
        // int setClosedLoopSpeed(int speed);
        int setOpenLoopSpeed(float speed);
        float x_hat = 0.0; // Estimated state (RPM)
    protected:
        PwmOut motor;
        InterruptIn tacho;
        
        Ticker tacho_timer; 
        Timer debounce_period;  

        // Kalman filter parameters (tune these based on sensor noise and system dynamics)
        float Q = 0.01; // Process noise covariance
        float R = 1.0; // Measurement noise covariance
        float P = 1.0; // Estimated error covariance
        
        uint16_t pulse_count;
        uint16_t avg_speed;

        // uint16_t speed_array[4];
        // uint8_t speed_pointer = 0;

        void kalmanFilter();
        // void tacho_rise_callback();
        void tacho_fall_callback();
        void calculate_speed_callback();
};