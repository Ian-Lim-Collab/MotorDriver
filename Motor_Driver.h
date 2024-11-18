#pragma once 
#include "mbed.h"

#define SPEED_UPDATE_RATE 1000000
#define SPEED_UPDATE_RATE_US 1000000us

#define DEBOUNCE_PERIOD 7000us

class Motor_Driver{
    public:
        Motor_Driver(PinName motor_pin, PinName tacho_pin);
        int getSpeed();
        // int setClosedLoopSpeed(int speed);
        int setOpenLoopSpeed(float speed);
        uint16_t pulse_count;
        Timer debounce_period;  
    protected:
        PwmOut motor;
        InterruptIn tacho;
        Ticker tacho_timer; 
        
        uint16_t speed;

        void tacho_rise_callback();
        void tacho_fall_callback();
        void calculate_speed_callback();
};