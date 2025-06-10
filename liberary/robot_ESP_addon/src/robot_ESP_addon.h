#ifndef Robot_h
#define Robot_h
#include <Arduino.h>
#ifdef _SSID
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
extern ArduinoOTAClass ArduinoOTA;
#endif

struct int16_3{
  int16_t x;
  int16_t y;
  int16_t z;
};struct return_IMU{
  int16_t temp;
  int16_3 accel;
  int16_3 gyro;
};struct float_3{
  float x;
  float y;
  float z;
};struct float_IMU{
  float temp;     //[°C]
  float_3 accel;  //[m/s²]
  float_3 gyro;   //[°/s]
};
struct return_machnetometer{
  bool corect;
  uint8_t status;
  int16_3 magneet;
  int16_t temp;
  int16_t volt;
};struct converted_machnetometer{
  bool corect;
  uint8_t status;
  float_3 magneet;  //[mT] of [µT]
  float temp; //[°C]
  float volt; //[V]
};

#include "ADC.h"
#include "IMU.h"
#include "IO_expander.h"
#include "IR_afstandsensor.h"
#include "LCD.h"
#include "lijnsensor.h"
#include "machnetomerter.h"
#include "motor.h"
#include "servo.h"
#include "US_afstands_sensor.h"



void robot_begin(uint8_t servo_mode, bool LCD_backlight, bool motor_turn,bool IMU,bool machnetometer,bool lijnsensor);

#endif