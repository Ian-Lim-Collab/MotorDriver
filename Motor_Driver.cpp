#include "Motor_Driver.h"

Motor_Driver::Motor_Driver(PinName motor_pin, PinName tacho_pin)
    :motor(motor_pin), tacho(tacho_pin){
    motor.period_us(MOTOR_PWM_PERIOD_US);
    this->tacho.rise(callback(this,&Motor_Driver::tacho_low_speed_rise_callback));
    this->tacho.fall(callback(this,&Motor_Driver::tacho_low_speed_fall_callback));
    this->clear_val.start();
};

void Motor_Driver::tacho_low_speed_fall_callback(){
    this->debounce_period.stop();
    if(this->debounce_period.elapsed_time() > DEBOUNCE_PERIOD){
        this->avg_speed = (15000000) / this->debounce_period.elapsed_time().count();
        this->clear_val.reset();
    }
};
void Motor_Driver::tacho_low_speed_rise_callback(){
    this->debounce_period.reset();
    this->debounce_period.start();
};

void Motor_Driver::tacho_high_speed_fall_callback(){
    this->debounce_period.stop();
    if(this->debounce_period.elapsed_time() > DEBOUNCE_PERIOD){
        this->pulse_count++;
    }
    this->debounce_period.reset();
    this->debounce_period.start();
};

void Motor_Driver::calculate_speed_callback(){
    this->speed_array[speed_pointer%4] = ((float) this->pulse_count * 30.) * 1000000. / SPEED_UPDATE_RATE ;
    this->pulse_count = 0;
    speed_pointer++;
    avg_speed = speed_array[0] + speed_array[1] + speed_array[2] + speed_array[3];
    avg_speed = avg_speed >> 2; // Divide by 2
};

int Motor_Driver::setOpenLoopSpeed(float speed){
    this->motor.write(speed);
    return 0;
};

int Motor_Driver::getSpeed(){
    if(this->avg_speed > HIGH_SPEED_CUTOFF) {
        if (!high_speed_cal){
            this->swapToHighSpeedCal();
            high_speed_cal = !high_speed_cal;
        }
        return this->avg_speed;
    } else {
        if (high_speed_cal){
            this->swapToLowSpeedCal();
            high_speed_cal = !high_speed_cal;
        }
        if (this->clear_val.elapsed_time() > CLEAR_TIMER){
            return 0;
        }
        return this->avg_speed;
    }
    
};

void Motor_Driver::swapToHighSpeedCal(){
    printf("Changing to High Speed");
    this->tacho_timer.attach(callback(this, &Motor_Driver::calculate_speed_callback), SPEED_UPDATE_RATE_US);
    while(true){
        if(!tacho){
            this->tacho.fall(callback(this,&Motor_Driver::tacho_high_speed_fall_callback));
            break;
        }
    }
};

void Motor_Driver::swapToLowSpeedCal(){
    this->tacho_timer.detach();
    this->clear_val.reset();
    this->clear_val.start();
    while(true){
        if(!tacho || clear_val.elapsed_time() > 100ms){
            printf("AAAAA");
            this->tacho.fall(callback(this,&Motor_Driver::tacho_low_speed_fall_callback));
            break;
        }
    }
    this->clear_val.reset();
    this->clear_val.start();
};