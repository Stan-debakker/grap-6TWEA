#include <robot.h>

void setup(){
  Serial.begin(115200);
}
void loop(){
  //linker oog word rood
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(true,i,0,0);
    delay(10);
  }
  //linker oog word groen
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(true,0,i,0);
    delay(10);
  }
  //linker oog word blauw
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(true,0,0,i);
    delay(10);
  }
  //leg het linker oog af
  set_RGB_LEDs(true,0,0,0);

  //rechter oog word rood
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(false,i,0,0);
    delay(10);
  }
  //rechter oog word groen
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(false,0,i,0);
    delay(10);
  }
  //rechter oog word blauw
  for (int i=0; i<=255;i++){
    set_RGB_LEDs(false,0,0,i);
    delay(10);
  }
  //leg het rechter oog af
  set_RGB_LEDs(false,0,0,0);
}