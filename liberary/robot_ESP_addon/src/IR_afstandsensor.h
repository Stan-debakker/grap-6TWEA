#ifndef IR_afstandsensor_h
#define IR_afstandsensor_h

#define IR_L 0
#define IR_C 1
#define IR_R 2

//stuurt de afstand van de afstandsensor tot een object terug (~10-80 cm)
//analoge_pin is de pin waarop de sensor is bevestigd
//sensor is de sensor (links:IR_L/0,midden:IR_C/1,rechts:IR_R/2)
//stuurt 0 terug als sensor fout is ingeheven
float read_IR_sensor(uint8_t analoge_pin,uint8_t sensor);

#endif