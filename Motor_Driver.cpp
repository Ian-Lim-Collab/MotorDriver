#include "Motor_Driver.h"

Motor_Driver::Motor_Driver(PinName motor_pin, PinName tacho_pin)
    :motor(motor_pin), tacho(tacho_pin){
    motor.period_us(MOTOR_PWM_PERIOD_US);
    this->tacho.rise(callback(this,&Motor_Driver::tacho_rise_callback));
    this->tacho.fall(callback(this,&Motor_Driver::tacho_fall_callback));
    this->clear_val.start();
    this->speed_update_timer.start();
    this->tacho_timer.attach(callback(this, &Motor_Driver::calculate_speed_callback), SPEED_UPDATE_RATE_US);
};

void Motor_Driver::tacho_fall_callback(){
    this->debounce_period.stop();
    if(this->debounce_period.elapsed_time() > DEBOUNCE_PERIOD){
        this->speed_update_timer.stop();
        this->inst_speed = (30000000) / this->speed_update_timer.elapsed_time().count();
        if(inst_speed < MAX_SPEED){
            this->speed_array[speed_pointer%AVERAGE_WINDOW] = inst_speed;
            speed_pointer++;
        } 
        this->speed_update_timer.reset();
        this->speed_update_timer.start();
        this->clear_val.reset();
    }
}
void Motor_Driver::tacho_rise_callback(){
    this->debounce_period.reset();
    this->debounce_period.start();
}

void Motor_Driver::calculate_speed_callback(){
    sum_speed = speed_array[0] + speed_array[1] + speed_array[2] + speed_array[3];
    avg_speed = sum_speed >> 2; // Divide by 2
};

int Motor_Driver::setOpenLoopSpeed(float speed){
    this->motor.write(speed);
    return 0;
};

int Motor_Driver::getSpeed(){
    if (this->clear_val.elapsed_time() > CLEAR_TIMER){
        return 0;
    }
    return this->avg_speed;
};

