//
// Created by debakkers on 13-5-2025.
//

#include "IO_expander.h"
#include <Wire.h>

#include "Wire.h"
#define ADDR_IO 0x20

uint16_t get_output(uint8_t register_ADDR){
    uint16_t output=0;
    Wire.beginTransmission(ADDR_IO);
    Wire.write(register_ADDR);
    Wire.endTransmission(false);
    Wire.requestFrom(ADDR,2,true);
    while(Wire.available()) {
        output=output<<8|Wire.read();
    }
    return output;
}
void set_output(uint16_t output, uint8_t register_ADDR){
    Wire.beginTransmission(ADDR_IO);
    Wire.write(register_ADDR);
    Wire.write(output>>8);
    Wire.write(output&0xFF);
    Wire.endTransmission(true);
}