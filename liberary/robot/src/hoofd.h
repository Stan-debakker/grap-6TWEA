#ifndef hoofd_h
#define hoofd_h

#define licht_sensor_L 0
#define licht_sensor_R 1
#define speel_geluid 2

void set_RGB_LEDs(bool rechts,uint8_t Rood,uint8_t Groen,uint8_t Blauw);
uint8_t get_sensor_value(uint8_t sensor);
void set_LEDs(uint8_t LEDs);
void set_gevoeligheid (uint8_t gevoeligheid);

#endif