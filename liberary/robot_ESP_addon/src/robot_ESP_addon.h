#ifndef Robot_h
#define Robot_h
#include <cstdint>

#include "IR_afstandsensor.h"
#include "LCD.h"
#include "motor.h"
#include "servo.h"
#include "US_afstands_sensor.h"

void init(uint8_t servo_mode, bool LCD_backlight, bool motor_turn,bool IMU,bool machnetometer);

#endif