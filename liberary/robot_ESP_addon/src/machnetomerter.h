#ifndef machnetometer_H
#define machnetometer_H
#include "robot_ESP_addon.h"

void start_burst_mode();
void read_voltage();
float_3 change_raw_to_mT(int16_3 raw);
converted_machnetometer convert_data_machnetometer(return_machnetometer data_in);
return_machnetometer get_data_machnetometer();
#endif