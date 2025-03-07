#include "Wire.h"
#define ADDR 0x20
#define input__port_ADDR 0x00
#define output_port_ADDR 0x02
#define IO_dir_port_ADDR 0x04
#define PUL_EN_port_ADDR 0x46
#define PULDIR_port_ADDR 0x48
#define INT_EN_port_ADDR 0x4A
#define INT_SR_port_ADDR 0x4C

void setup() {
  Wire.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

}

uint16_t get_output(uint8_t register_ADDR){
  uint16_t output=0;
  Wire.beginTransmission(ADDR);
  Wire.write(register_ADDR);
  Wire.endTransmission(False);
  Wire.requestFrom(ADDR,2,True);
  while(Wire.available()) {
    output=output<<8|Wire.read();
  }
  return output;
}
void set_output(uint16_t output, uint8_t register_ADDR){
  Wire.beginTransmission(ADDR);
  Wire.write(register_ADDR);
  Wire.write(output>>8);
  Wire.write(output&0xFF);
  Wire.endTransmission(True);
}