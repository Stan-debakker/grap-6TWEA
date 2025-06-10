//
// Created by debakkers on 13-5-2025.
//

#include "ADC.h"
#include <Wire.h>
#include <arduino.h>

#define ADDR_ADC 0x48

void set_input_pin_ADC(uint8_t pin,bool singel){
    uint8_t data=0xC0|((3-pin)<<4)|(singel?1:0);
    Wire.beginTransmission(ADDR_ADC);
    Wire.write(1);
    Wire.write(data); //set de pin rest defoult
    Wire.write(0x83);                       //defoult
    Wire.endTransmission();
}
int16_t get_data_ADC(){
    int16_t data;
    Wire.beginTransmission(ADDR_ADC);
    Wire.write(0);
    Wire.endTransmission(true);
    Wire.requestFrom(ADDR_ADC,2);
    while (Wire.available()) {
        data=data<<8|Wire.read();
    }
    return data;
}