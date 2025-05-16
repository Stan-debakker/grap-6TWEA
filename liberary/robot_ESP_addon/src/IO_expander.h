//
// Created by debakkers on 13-5-2025.
//

#ifndef IO_EXPANDER_H
#define IO_EXPANDER_H
#include <arduino.h>

#define input__port_ADDR 0x00
#define output_port_ADDR 0x02
#define IO_dir_port_ADDR 0x04
#define PUL_EN_port_ADDR 0x46
#define PULDIR_port_ADDR 0x48
#define INT_EN_port_ADDR 0x4A
#define INT_SR_port_ADDR 0x4C

uint16_t get_output_IO(uint8_t register_ADDR);
void set_output_IO(uint16_t output, uint8_t register_ADDR);

#endif //IO_EXPANDER_H
