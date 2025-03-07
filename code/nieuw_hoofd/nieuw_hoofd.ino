#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define I2C_ADDR 0xAA

#define RGB_leds 20
#define regester_len 3*RGB_leds
#define pin_LED 
volatile uint8_t regester_ADDR=0;
volatile uint8_t regester[regester_len];
Adafruit_NeoPixel RGB_led(RGB_leds,pin_LED,NEO_GRB + NEO_KHZ800);


void onReceive(int len) {
  regester_ADDR=Wire.read();
  while (Wire.available()) {
    regester[regester_ADDR++]=Wire.read();
    if(regester_ADDR>regester_len){
      regester_ADDR=0;
    }
  }
  update_leds();
}

void update_leds(){
  for (int i;i<RGB_leds;i++){
    RGB_led.setPixelColor(i,regester[i*3],regester[i*3+1],regester[i*3+2]);
  }
  RGB_led.show();
}

void setup() {
  RGB_led.begin();
  Wire.onReceive(onReceive);
  Wire.begin(I2C_ADDR);
  interrupts();
}

void loop() {}