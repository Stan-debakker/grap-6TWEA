#include <robot.h>

void setup() {
  Wire.setPins(10, 11);
  Wire.begin();
  Serial.begin(115200);
  sensors_config(Hz100,g2,dps250);
}

void loop() {
  print_resolt_IMU();
  delay(100);
}