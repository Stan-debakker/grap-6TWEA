#include <Arduino.h>
#include "IR_afstandsensor.h"

#define IR_L 0
#define IR_C 1
#define IR_R 2

//stuurt de afstand van de afstandsensor tot een object terug (~10-80 cm)
//analoge_pin is de pin waarop de sensor is bevestigd
//sensor is de sensor (links:IR_L/0,midden:IR_C/1,rechts:IR_R/2)
//stuurt 0 terug als sensor fout is ingeheven
float read_IR_sensor(uint8_t analoge_pin,uint8_t sensor){
  int16_t data=0;
  for (int8_t i=0;i<16;i++){
    data+=analogRead(analoge_pin);
    delay(10);
  }
  //stuur de afstand terug volgens de calibratie
  switch (sensor){
    case IR_R:
      return 16*4286.8927/float(data)+3.6032954;
    case IR_C:
      return 16*7894.2012/float(data)-11.045445;
    case IR_L:
      return 16*2679.1287/float(data)+12.849024;
    default:
      return 0;
  }
}