#ifndef _MOTOR_HPP_
#define _MOTOR_HPP_

void motor_init();
void set_left_motor(double new_speed, bool forward);
void set_right_motor(double new_speed, bool forward);

#endif //_MOTOR_HPP_
