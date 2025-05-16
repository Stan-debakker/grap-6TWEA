#ifndef motor_h
#define motor_h
#include <Arduino.h>

int32_t get_4byte_command(uint8_t command);
uint8_t get_1byte_command(uint8_t command);

int32_t get_encoder1();
int32_t get_encoder2();
uint8_t get_snelheid1();
uint8_t get_snelheid2();
uint8_t get_volt();
uint8_t get_stroom1();
uint8_t get_stroom2();
uint8_t get_versnelling();
uint8_t get_mode();

//sent commando's
void set_acceleration(uint8_t versnelling);
void set_mode(uint8_t mode);
void set_regulator(bool set_to);
void set_time_out(bool set_to);
void reset_encoders();
void set_snelheid1(int8_t snelheid);
void set_snelheid2(int8_t snelheid);

#endif