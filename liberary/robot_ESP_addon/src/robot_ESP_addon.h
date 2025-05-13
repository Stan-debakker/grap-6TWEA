#ifndef Robot_h
#define Robot_h

#include "IMU.h"
#include "IR_afstandsensor.h"
#include "LCD.h"
#include "machnetometer.h"
#include "motor.h"
#include "servo.h"
#include "US_afstands_sensor.h"

struct int16_3{
    int16_t x;
    int16_t y;
    int16_t z;
};
struct float_3{
    float x;
    float y;
    float z;
};
struct return_IMU{
    int16_t temp;
    struct int16_3 accel;
    struct int16_3 gyro;
};
struct return_machnetometer{
    bool corect;
    uint8_t status;
    struct int16_3 magneet;
    int16_t temp;
    int16_t volt;
};
struct float_IMU{
    float temp;     //[°C]
    struct float_3 accel;  //[m/s²]
    struct float_3 gyro;   //[°/s]
};
struct converted_machnetometer{
    bool corect;
    uint8_t status;
    struct float_3 magneet;  //[mT]
    float temp; //[°C]
    float volt; //[V]
};
void init(uint8_t servo_mode, bool LCD_backlight, bool motor_turn,bool IMU,bool machnetometer);
#endif