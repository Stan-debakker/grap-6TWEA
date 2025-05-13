#ifndef servo_h
#define servo_h

#define Hooft_horizontaal 0
#define Hooft_vertikaal 1
#define Grip_L 2
#define Grip_R 3

#define alles_af  0
#define hooft_aan 1
#define grips_aan 2
#define alles_aan 3

void init_servo(uint8_t mode);
bool set_pos(uint8_t servo,uint8_t posietie);
bool set_ofset(uint8_t servo,uint8_t ofset);

#endif