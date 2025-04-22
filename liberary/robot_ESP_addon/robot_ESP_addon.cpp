#include "robot_ESP_addon.h"
#include "IMU.h"
#include "LCD.h"
#include "machnetometer.h"
#include "motor.h"
#include "servo.h"

void init(uint8_t servo_mode, bool LCD_backlight, bool motor_turn,bool IMU,bool machnetometer){
    Wire.begin(10,11);
    backlight(LCD_backlight);
    clear_screen();
    init_servo(servo_mode);
    reset_encoders();
    set_mode(motor_turn?1:3);
    if (IMU) sensors_config(Hz100,g2,dps250);
    if (machnetometer) start_burst_mode();
}