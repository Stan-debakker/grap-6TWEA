#include <Wire.h>
#include <Arduino.h>
#include "US_afstands_sensor.h"

#define US_ADDR 0x70

uint16_t US_afstands_sensor(){
  uint16_t data;
  Wire.beginTransmission(US_ADDR);
  Wire.write(0);
  Wire.write(0x51);
  Wire.endTransmission();
  delay(70);
  Wire.beginTransmission(US_ADDR);
  Wire.write(2);
  Wire.endTransmission(false);
  Wire.requestFrom(US_ADDR,2);
  while(Wire.available()) {
    data=data<<8;
    data+=Wire.read();
  }
  return data;
}