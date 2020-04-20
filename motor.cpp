#include "mbed.h"

#define MOTOR_PERIOD 0.05

static PwmOut motorLeft(p23);
static PwmOut motorRight(p22);
static DigitalOut fwdLeft(p5);
static DigitalOut fwdRight(p6);

void motor_init() {
    motorLeft.period(MOTOR_PERIOD);
    motorRight.period(MOTOR_PERIOD);
    motorLeft.write(0.0);
    motorRight.write(0.0);
    fwdLeft = true;
    fwdRight = true;
}

void set_left_motor(double new_speed, bool forward) {
    motorLeft.write(new_speed);
    fwdLeft = !forward;
}

void set_right_motor(double new_speed, bool forward) {
    motorRight.write(new_speed);
    fwdRight = !forward;
}
