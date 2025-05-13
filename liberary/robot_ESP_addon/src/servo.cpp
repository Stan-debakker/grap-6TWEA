#include <Wire.h>
#include <Arduino.h>
#include "servo.h"

#define Servo_ADDR 0x55

void init_servo(uint8_t mode){
  Wire.beginTransmission(Servo_ADDR);
  Wire.write(0);
  Wire.write(mode);
  Wire.endTransmission();
}
bool set_pos(uint8_t servo,uint8_t posietie){
  Wire.beginTransmission(Servo_ADDR);
  switch (servo){
    case Hooft_horizontaal:
      Wire.write(1);
      break;
    case Hooft_vertikaal:
      Wire.write(2);
      break;
    case Grip_L:
      Wire.write(3);
      break;
    case Grip_R:
      Wire.write(4);
      break;
    default:
      Wire.endTransmission();
      return false;
  }
  Wire.write(posietie);
  Wire.endTransmission();
  return true;
}
bool set_ofset(uint8_t servo,uint8_t ofset){
  Wire.beginTransmission(Servo_ADDR);
  switch (servo){
    case Hooft_horizontaal:
      Wire.write(5);
      break;
    case Hooft_vertikaal:
      Wire.write(6);
      break;
    case Grip_L:
      Wire.write(7);
      break;
    case Grip_R:
      Wire.write(8);
      break;
    default:
      Wire.endTransmission();
      return false;
  }
  Wire.write(ofset);
  Wire.endTransmission();
  return true;
}