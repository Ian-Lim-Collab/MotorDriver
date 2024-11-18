#include "Motor_Driver.h"

Motor_Driver::Motor_Driver(PinName motor_pin, PinName tacho_pin)
    :motor(motor_pin), tacho(tacho_pin){
    motor.period_ms(3);
    this->tacho.fall(callback(this,&Motor_Driver::tacho_fall_callback));
    this->tacho_timer.attach(callback(this, &Motor_Driver::calculate_speed_callback), SPEED_UPDATE_RATE_US);
};

void Motor_Driver::tacho_fall_callback(){
    this->debounce_period.stop();
    if(this->debounce_period.elapsed_time() > DEBOUNCE_PERIOD){
        this->pulse_count++;
    }
    this->debounce_period.reset();
    this->debounce_period.start();
}

void Motor_Driver::calculate_speed_callback(){
    this->speed = ((float) this->pulse_count * 30.) * 1000000. / SPEED_UPDATE_RATE ;
    this->pulse_count = 0;
};

int Motor_Driver::setOpenLoopSpeed(float speed){
    this->motor.write(speed);
    return 0;
};

int Motor_Driver::getSpeed(){
    return this->speed;
};

