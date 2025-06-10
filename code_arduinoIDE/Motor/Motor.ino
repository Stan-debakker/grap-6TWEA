#include <Wire.h>

#define driver_ADDR 0x58

void setup() {
  Serial.begin(115200);
  Wire.begin(10,11);
  set_mode(1);
  set_snelheid2(127);
}

void loop() {/*
  Serial.print("encoder 1: ");
  Serial.print(get_encoder1());
  Serial.print("\tencoder 2: ");
  Serial.print(get_encoder2());
  Serial.print("\tsnelhijd 1: ");
  Serial.print(get_snelheid1());
  Serial.print("\tsnelhijd 2: ");
  Serial.print(get_snelheid2());
  set_snelheid2(127);
  Serial.print("\tstroom 1: ");
  Serial.print(get_stroom1());
  Serial.print("\tstroom 2: ");
  Serial.print(get_stroom2());
  set_snelheid2(127);
  Serial.print("\tspanning: ");
  Serial.print(get_volt());
  Serial.print("\tversnelling: ");
  Serial.print(get_versnelling());
  Serial.print("\tmode: ");
  Serial.println(get_mode());
  delay(5000);*/
  set_snelheid1(30);
  delay(1000);
  set_snelheid2(30);
  delay(10000);
  set_snelheid1(-30);
  delay(1000);
  set_snelheid2(-30);
  delay(10000);
}
//https://www.robot-electronics.co.uk/htm/md25i2c.htm
// get data
  int32_t get_encoder1(){
    return get_4byte_command(0x2);
  }int32_t get_encoder2(){
    return get_4byte_command(0x6);
  }
  uint8_t get_snelheid1(){
    return get_1byte_command(0x0);
  }uint8_t get_snelheid2(){
    return get_1byte_command(0x1);
  }uint8_t get_volt(){
    return get_1byte_command(0xA);
  }uint8_t get_stroom1(){
    return get_1byte_command(0xB);
  }uint8_t get_stroom2(){
    return get_1byte_command(0xC);
  }uint8_t get_versnelling(){
    return get_1byte_command(0xE);
  }uint8_t get_mode(){
    return get_1byte_command(0xF);
  }

  int32_t get_4byte_command(uint8_t command){
    int32_t data=0;
    Wire.beginTransmission(driver_ADDR);
    Wire.write(command);
    Wire.endTransmission(false);
    Wire.requestFrom(driver_ADDR,4,true);
    while(Wire.available()){
      data=(data<<8)+Wire.read();
    }
    return data;
  }
  uint8_t get_1byte_command(uint8_t command){
    uint8_t data;
    Wire.beginTransmission(driver_ADDR);
    Wire.write(command);
    Wire.endTransmission(false);
    Wire.requestFrom(driver_ADDR,1,true);
    while(Wire.available()) {
      data=Wire.read();
    }
    return data;
  }
//sent commando's
  void set_acceleration(uint8_t versnelling){
    Wire.beginTransmission(driver_ADDR);
    Wire.write(14);
    Wire.write(versnelling);
    Wire.endTransmission();
  }
  void set_mode(uint8_t mode){
    Wire.beginTransmission(driver_ADDR);
    Wire.write(15);
    Wire.write(mode);
    Wire.endTransmission();
  }
  void set_regulator(bool set_to){
    Wire.beginTransmission(driver_ADDR);
    Wire.write(16);
    if (set_to){
      Wire.write(0x31);
    }else{
      Wire.write(0x30);
    }
    Wire.endTransmission();
  }
  void set_time_out(bool set_to){
    Wire.beginTransmission(driver_ADDR);
    Wire.write(16);
    if (set_to){
      Wire.write(0x33);
    }else{
      Wire.write(0x32);
    }
    Wire.endTransmission();
  }
  void reset_encoders(){
    Wire.beginTransmission(driver_ADDR);
    Wire.write(16);
    Wire.write(0x20);
    Wire.endTransmission();
  }
  void set_snelheid1(int8_t snelheid){
    Wire.beginTransmission(driver_ADDR);
    Wire.write(0);
    Wire.write(snelheid);
    Wire.endTransmission();
  }
  void set_snelheid2(int8_t snelheid){
    Wire.beginTransmission(driver_ADDR);
    Wire.write(1);
    Wire.write(snelheid);
    Wire.endTransmission();
  }