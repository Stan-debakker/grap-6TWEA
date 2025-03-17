#include <Wire.h>         //I²C bibliotheek
#include <tinyNeoPixel.h> //RGB led bibliotheek

#define I2C_ADDR 0x55 //I²C adderes kan worden vranerd

#define RGB_leds 20   //aantal leds
#define regester_len 3*RGB_leds //aantal bytes voor de kleur op te slaan
#define pin_LED 2 //pin nummer (6,7)
volatile uint8_t regester[regester_len];  //opslag van de kleuren
tinyNeoPixel pixels = tinyNeoPixel(RGB_leds, pin_LED, NEO_GRB + NEO_KHZ800);

//krijg nieuwe data via I²C
void onReceive(int len) {
  uint8_t regester_ADDR=Wire.read();
  while (Wire.available()) {
    regester[regester_ADDR++]=Wire.read();
    if(regester_ADDR>regester_len){
      regester_ADDR=0;
    }
  }
  update_leds();
}
//update de leds na nieuwe date te hebben gekregen
void update_leds(){
  for (uint8_t i=0;i<RGB_leds;i++){
    pixels.setPixelColor(i,regester[i*3],regester[i*3+1],regester[i*3+2]);
  }
  pixels.show();
}

void setup() {
  //initializeer RGB led
  pixels.begin();
  //initializeer Wire (I²C)
  Wire.onReceive(onReceive);
  Wire.begin(I2C_ADDR);
  //sta interrupts toe nodig voor I²C
  interrupts();
}

void loop() {}