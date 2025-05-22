#include <Wire.h>
#define base_addr_opt 0x44

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


uint32_t get_ID(uint8_t addr_pin){
  uint32_t data;
  Wire.beginTransmission(base_addr_opt|(addr_pin&0x03));
  Wire.write(0x7E);
  Wire.endTransmission(false);
  Wire.requestFrom(base_addr_opt|(addr_pin&0x03),4);
  while (Wire.available()) data=(data<<8)|Wire.read();
  return data;
}