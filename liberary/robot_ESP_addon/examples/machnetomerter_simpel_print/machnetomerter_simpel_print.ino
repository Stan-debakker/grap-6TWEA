#include <robot_ESP_addon.h>

void setup() {
  Serial.begin(115200);
  start_burst_mode();
  delay(100); //wacht tot de machnetometer is gestart
}

void loop() {
  print_resolt_machnetomerter(false);
  delay(10);
}