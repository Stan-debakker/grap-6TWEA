//
// Created by debakkers on 13-5-2025.
//

#ifndef ADC_H
#define ADC_H
#include <arduino.h>

void set_input_pin(uint8_t pin,bool singel);
int16_t get_data();

#endif //ADC_H
