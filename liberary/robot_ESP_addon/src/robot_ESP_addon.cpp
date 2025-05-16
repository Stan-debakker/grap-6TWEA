#include "robot_ESP_addon.h"
#include "LCD.h"
#include "motor.h"
#include "servo.h"
#include <arduino.h>
#include <Wire.h>

void init(uint8_t servo_mode, bool LCD_backlight, bool motor_turn,bool IMU,bool machnetometer){
    Wire.begin(10,11);
    //LCD
    backlight(LCD_backlight);
    clear_screen();
    //servo's
    init_servo(servo_mode);
    //motoren
    reset_encoders();
    set_mode(motor_turn?1:3);
}