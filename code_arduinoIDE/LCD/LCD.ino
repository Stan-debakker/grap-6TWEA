#include <Wire.h> 

#define LCD_ADDR 0x63

void setup() {
  Serial.begin(115200);
  Wire.begin(10,11);
  opstarten();
  print_text("hallo ik ben stan   dit is een test     ik ben hier nog 2   lijnen aan het schrijven");
  delay(5000);
  clear_screen();
  
}

void loop() {
  if(Serial.available()){
    print_char(Serial.read());
  }
}

void opstarten(){
  backlight_on();
  clear_screen();
}
void set_cursor(uint8_t positie){
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0);
  Wire.write(2);
  Wire.write(positie);
  Wire.endTransmission();
}
void clear_screen(){
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0);
  Wire.write(12);
  Wire.endTransmission();
}
void backlight_on(){
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0);
  Wire.write(19);
  Wire.endTransmission();
}void backlight_off(){
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0);
  Wire.write(20);
  Wire.endTransmission();
}
void print_char(char karacter){
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0);
  Wire.write(karacter);
  Wire.endTransmission();
}
void print_text(char* text){
  while (*text!=0){
    print_char(*text);
    text++;
  }
}