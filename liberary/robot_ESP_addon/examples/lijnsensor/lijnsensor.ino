#include <robot_ESP_addon.h>

void setup() {
  robot_begin(0,false,false,false,false,true);  //start de robot
}

void loop() {
  delay(1000);  //wacht tussen meten
  get_all_spectrum(); //vraag alle data op
  //print sensor nummer
  //ga alle kanalen af en print de data uit
  for(uint8_t i;i<8;i++){
    spectrum data=spectrum_rezolt[i];   //haal de 1 sensor uit de lijst
    Serial.print("sensor ");
    Serial.print(i+1);
    Serial.print("\tF1:");
    Serial.print(data.F1);
    Serial.print("\tF2:");
    Serial.print(data.F2);
    Serial.print("\tF3:");
    Serial.print(data.F3);
    Serial.print("\tF4:");
    Serial.print(data.F4);
    Serial.print("\tF5:");
    Serial.print(data.F5);
    Serial.print("\tF6:");
    Serial.print(data.F6);
    Serial.print("\tF7:");
    Serial.print(data.F7);
    Serial.print("\tF8:");
    Serial.print(data.F8);
    Serial.print("\tFZ:");
    Serial.print(data.FZ);
    Serial.print("\tFY:");
    Serial.print(data.FY);
    Serial.print("\tFXL:");
    Serial.print(data.FXL);
    Serial.print("\tFNIR:");
    Serial.print(data.FNIR);
    Serial.print("\tVIS:");
    Serial.print(data.VIS);
    Serial.print("\tFD");
    Serial.print(0x100|data.FD,BIN);
  }
  Serial.println();
}