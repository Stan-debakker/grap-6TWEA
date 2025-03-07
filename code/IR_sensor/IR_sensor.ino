#define IR_L 0
#define IR_C 1
#define IR_R 2

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(read_IR_sensor(A0,IR_C));
  delay(500);
}

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
      break;
    case IR_C:
      return 16*7894.2012/float(data)-11.045445;
      break;
    case IR_L:
      return 16*2679.1287/float(data)+12.849024;
      break;
    default:
      return 0;
  }
}