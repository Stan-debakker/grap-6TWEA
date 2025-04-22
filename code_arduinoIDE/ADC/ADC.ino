#include "Wire.h"
#define ADDR 0x48


void setup() {
  delay(1000);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("start");
  set_input_pin(0,false);
  delay(1000);
}

void loop() {
  Serial.println(get_data());
  delay(2000);
}

void set_input_pin(uint8_t pin,bool singel){
  uint8_t data=0xC0|((3-pin)<<4)|(singel?1:0);
  Wire.beginTransmission(ADDR);
  Wire.write(1);
  Wire.write(data); //set de pin rest defoult
  Wire.write(0x83);                       //defoult
  Wire.endTransmission();
  Serial.println(data,BIN);
}
int16_t get_data(){
  int16_t data;
  Wire.beginTransmission(ADDR);
  Wire.write(0);
  Wire.endTransmission(true);
  Wire.requestFrom(ADDR,2);
  while (Wire.available()) {
    data=data<<8|Wire.read();
  }
  return data;
}