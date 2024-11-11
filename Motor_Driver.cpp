#include "Motor_Driver.h"

Motor_Driver::Motor_Driver(PinName motor_pin, PinName tacho_pin)
    :motor(motor_pin), tacho(tacho_pin){
    
    this->tacho.rise(&Motor_Driver::tacho_rise_callback);
    this->tacho_timer.attach(&Motor_Driver::calculate_speed_callback, SPEED_UPDATE_RATE);
};

void Motor_Driver::tacho_rise_callback(){
    tacho_timer.detach();
    this->pulse_count++;
    tacho_timer.attach(&Motor_Driver:calculate_speed_callback, SPEED_UPDATE_RATE);
};

void Motor_Driver::calculate_speed_callback(){
    this->tacho.disable_irq();
    this->speed = this->pulse_count * 30 / (SPEED_UPDATE_RATE/ 1000);
    this->pulse_count = 0;
    this->tacho.enable_irq();
}

int Motor_Driver::getSpeed(){
    return this->speed;
};

