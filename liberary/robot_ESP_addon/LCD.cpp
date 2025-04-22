#include <Arduino.h>
#include <Wire.h>
#include "LCD.h"

#define LCD_ADDR 0x63

//zet de cursor posietie=X+20Y
void set_cursor(uint8_t positie){
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0);
  Wire.write(2);
  Wire.write(positie);
  Wire.endTransmission();
}
//wist het hele scherm
void clear_screen(){
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0);
  Wire.write(12);
  Wire.endTransmission();
}
//zet de backlight (aan of af) volgens vairalble aan 
void backlight(bool aan){
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0);
  Wire.write(aan?19:20);
  Wire.endTransmission();
}
//print 1 karacter op het scherm
void print_char(char karacter){
  Wire.beginTransmission(LCD_ADDR);
  Wire.write(0);
  Wire.write(karacter);
  Wire.endTransmission();
}
//print een lijst met karacters of string
void print_text(char* text){
  while (*text!=0){
    print_char(*text);
    text++;
  }
}