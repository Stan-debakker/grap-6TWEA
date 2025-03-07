#include <Wire.h>

#define hoofd_ADDR 0x08
typedef enum sensor{
  licht_sensor_L=8,
  licht_sensor_R,
  geluids_sensor
}sensor_t;

uint8_t leds=1;

void setup() {
  Serial.begin(115200);
  Wire.setPins(11,10);
  Wire.begin();
  set_gevoeligheid(0);
}

void loop() {
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(true,i,0,0);
    delay(10);
  }
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(true,0,i,0);
    delay(10);
  }
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(true,0,0,i);
    delay(10);
  }
  set_RGB_LEDs(true,0,0,0);
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(false,i,0,0);
    delay(10);
  }
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(false,0,i,0);
    delay(10);
  }
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(false,0,0,i);
    delay(10);
  }
  set_RGB_LEDs(false,0,0,0);
  if (leds==0){
    leds=1;
  }else{
    leds=leds<<1;
  }
  set_LEDs(leds);
  Serial.print(get_sensor_value(licht_sensor_L));
  Serial.print("\t");
  Serial.print(get_sensor_value(licht_sensor_R));
  Serial.print("\t");
  Serial.println(get_sensor_value(geluids_sensor));
}

void set_RGB_LEDs(bool rechts,uint8_t Rood,uint8_t Groen,uint8_t Blauw){
  Wire.beginTransmission(hoofd_ADDR);
  if (rechts){
    Wire.write(1);
    Wire.write(Blauw);
    Wire.write(Groen);
    Wire.write(Rood);
  }else{
    Wire.write(4);
    Wire.write(Rood);
    Wire.write(Groen);
    Wire.write(Blauw);
  }
  Wire.endTransmission();
}
uint8_t get_sensor_value(sensor_t sensor){
  Wire.beginTransmission(hoofd_ADDR);
  Wire.write(sensor);
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