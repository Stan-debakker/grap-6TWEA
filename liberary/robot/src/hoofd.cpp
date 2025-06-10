#include <Arduino.h>
#include <Wire.h>
#include "hoofd.h"

#define licht_sensor_L 0
#define licht_sensor_R 1
#define speel_geluid 2

#define hoofd_ADDR 0x08

void set_RGB_LEDs(bool rechts,uint8_t Rood,uint8_t Groen,uint8_t Blauw){
  Wire.beginTransmission(hoofd_ADDR);
  if (rechts){
    Wire.write(4);
    Wire.write(Rood);
    Wire.write(Groen);
    Wire.write(Blauw);
  }else{
    Wire.write(1);
    Wire.write(Blauw);
    Wire.write(Groen);
    Wire.write(Rood);
  }
  Wire.endTransmission();
}
uint8_t get_sensor_value(uint8_t sensor){
  Wire.beginTransmission(hoofd_ADDR);
  switch (sensor){
    case licht_sensor_L:
      Wire.write(8);
      break;
    case licht_sensor_R:
      Wire.write(9);
      break;
    case speel_geluid:
      Wire.write(10);
    default:
      Wire.endTransmission();
      return 0;
  }
  Wire.endTransmission(false);
  uint8_t data=0;
  Wire.requestFrom(hoofd_ADDR,1,true);
  while(Wire.available()) {
    data=Wire.read();
  }
  return data;
}
void set_LEDs(uint8_t LEDs){
  Wire.beginTransmission(hoofd_ADDR);
  Wire.write(7);
  Wire.write(LEDs);
  Wire.endTransmission();
}
void set_gevoeligheid (uint8_t gevoeligheid){
  Wire.beginTransmission(hoofd_ADDR);
  Wire.write(11);
  Wire.write(gevoeligheid);
  Wire.endTransmission();
}