#include <Wire.h>

#define US_ADDR 0x70

void setup() {
  Serial.begin(115200);
  Wire.setPins(10, 11);
  Wire.begin();
}

void loop() {
  Serial.print("afstand cm: ");
  Serial.println(US_afstands_sensor());
  delay(930);
}

uint16_t US_afstands_sensor(){
  uint16_t data=0;
  Wire.beginTransmission(US_ADDR);
  Wire.write(0);
  Wire.write(0x51);
  Wire.endTransmission();
  delay(70);
  Wire.beginTransmission(US_ADDR);
  Wire.write(2);
  Wire.endTransmission(false);
  Wire.requestFrom(US_ADDR,2,true);
  if (2 == Wire.available()){
    data=(Wire.read()<<8)|Wire.read();
  }
  return data;
}