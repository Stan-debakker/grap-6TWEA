#include "LCD.h"
#include "servo.h"
#include "motor.h"
void init(uint8_t servo_mode, bool LCD_backlight, bool motor_turn){
    backlight(LCD_backlight);
    clear_screen();
    init_servo(servo_mode);
    reset_encoders();
    set_mode(motor_turn?1:3);
}