#pragma once 
#include "mbed.h"

#define SPEED_UPDATE_RATE 600000.0F
#define SPEED_UPDATE_RATE_US 600000us

#define DEBOUNCE_PERIOD 14000us
#define MOTOR_PWM_PERIOD_US 2150

class Motor_Driver{
    public:
        Motor_Driver(PinName motor_pin, PinName tacho_pin);
        int getSpeed();
        // int setClosedLoopSpeed(int speed);
        int setOpenLoopSpeed(float speed);
    protected:
        PwmOut motor;
        InterruptIn tacho;
        
        Ticker tacho_timer; 
        Timer debounce_period;  
        
        uint16_t pulse_count;
        uint16_t avg_speed;

        uint16_t speed_array[4];
        uint8_t speed_pointer = 0;

        // void tacho_rise_callback();
        void tacho_fall_callback();
        void calculate_speed_callback();
};