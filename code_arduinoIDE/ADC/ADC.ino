#include "Wire.h"
#define ADDR 0x48


void setup() {
  delay(1000);
  Serial.begin(115200);
  set_input_pin(1,false);
}

void loop() {
  Serial.println(get_data());
  delay(500);
}

void set_input_pin(uint8_t pin,bool singel){
  Wire.begin(ADDR);
  Wire.write(1);
  Wire.write(0x40|(pin<<4)|(singel?0:1)); //set de pin rest defoult
  Wire.write(0x83);                       //defoult
  Wire.endTransmission();
  Serial.println(0x40|(pin<<4)|(singel?0:1));
}
int16_t get_data(){
  int16_t data;
  Wire.begin(ADDR);
  Wire.write(0);
  Wire.endTransmission(true);
  Wire.requestFrom(ADDR,2);
  while (Wire.available()) {
    data=data<<8|Wire.read();
  }
  return data;
}