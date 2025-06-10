#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel RGB_led(1,8,NEO_GRB + NEO_KHZ800);

void setup() {
  RGB_led.begin();
}

void loop() {
  RGB_led.clear();
  RGB_led.show();
  delay(500);
  RGB_led.setPixelColor(0,0xFF0000);
  RGB_led.show();
  delay(500);
  RGB_led.setPixelColor(0,0x00FF00);
  RGB_led.show();
  delay(500);
  RGB_led.setPixelColor(0,0x0000FF);
  RGB_led.show();
  delay(500);
}
