#include <Wire.h>

#define Hooft_horizontaal 0
#define Hooft_vertikaal 1
#define Grip_L 2
#define Grip_R 3

#define alles_af  0
#define hooft_aan 1
#define grips_aan 2
#define alles_aan 3

#define Servo_ADDR 0x55

void setup() {
  Wire.begin();
  Serial.begin(115200);
  init_servo(alles_aan);
}

void loop() {
  for (int servo=0;servo<4;servo++){
    switch (servo){
      case Hooft_horizontaal:
        Serial.print("Hooft_horizontaal");
        break;
      case Hooft_vertikaal:
        Serial.print("Hooft_vertikaal");
        break;
      case Grip_L:
        Serial.print("Grip_L");
        break;
      case Grip_R:
        Serial.print("Grip_R");
        break;
    }
    set_pos(servo,0);
    Serial.print("\t0");
    delay(1000);
    set_pos(servo,64);
    Serial.print("\t64");
    delay(1000);
    set_pos(servo,128);
    Serial.print("\t128");
    delay(1000);
    set_pos(servo,192);
    Serial.print("\t129");
    delay(1000);
    set_pos(servo,255);
    Serial.println("\t255");
    delay(1000);
  }
}

void init_servo(uint8_t mode){
  Wire.beginTransmission(Servo_ADDR);
  Wire.write(0);
  Wire.write(mode);
  Wire.endTransmission();
}
bool set_pos(uint8_t servo,uint8_t posietie){
  Wire.beginTransmission(Servo_ADDR);
  switch (servo){
    case Hooft_horizontaal:
      Wire.write(1);
      break;
    case Hooft_vertikaal:
      Wire.write(2);
      break;
    case Grip_L:
      Wire.write(3);
      break;
    case Grip_R:
      Wire.write(4);
      break;
    default:
      Wire.endTransmission();
      return false;
  }
  Wire.write(posietie);
  Wire.endTransmission();
  return true;
}
bool set_ofset(uint8_t servo,uint8_t ofset){
  Wire.beginTransmission(Servo_ADDR);
  switch (servo){
    case Hooft_horizontaal:
      Wire.write(5);
      break;
    case Hooft_vertikaal:
      Wire.write(6);
      break;
    case Grip_L:
      Wire.write(7);
      break;
    case Grip_R:
      Wire.write(8);
      break;
    default:
      Wire.endTransmission();
      return false;
  }
  Wire.write(ofset);
  Wire.endTransmission();
  return true;
}