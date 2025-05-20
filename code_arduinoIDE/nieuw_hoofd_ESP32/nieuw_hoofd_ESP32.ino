#include <Wire.h>
#define addr_hoofd 0x55

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
uint8_t R[18],G[18],B[18];
void set_all_leds(){
  Wire.beginTransmission(addr_hoofd);
  Wire.write(0);
  for (int i=0;i<18;i++){
    Wire.write(R[i]);
    Wire.write(G[i]);
    Wire.write(B[i]);
  }
  Wire.endTransmission();
}

void set_led(uint8_t num,uint8_t R,uint8_t G,uint8_t B){
  Wire.beginTransmission(addr_hoofd);
  Wire.write(num);
  Wire.write(R);
  Wire.write(G);
  Wire.write(G);
  Wire.endTransmission();
}