#pragma once 
#include "mbed.h"

#define SPEED_UPDATE_RATE 1000us    

class Motor_Driver{
    public:
        Motor_Driver(PinName motor_pin, PinName tacho_pin);
        int getSpeed();
        // int setClosedLoopSpeed(int speed);
        // int setOpenLoopSpeed(int speed);
    protected:
        PwmOut motor;
        InterruptIn tacho;
        Ticker tacho_timer;      
        
        uint16_t speed;
        uint16_t pulse_count;

        void tacho_rise_callback();
        void calculate_speed_callback();
};