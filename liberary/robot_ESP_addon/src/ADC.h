//
// Created by debakkers on 13-5-2025.
//

#ifndef ADC_H
#define ADC_H
#include <arduino.h>

void set_input_pin_ADC(uint8_t pin,bool singel);
int16_t get_data_ADC();

#endif //ADC_H
