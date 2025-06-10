#ifndef lijnsensor_h
#define lijnsensor_h
#include "robot_ESP_addon.h"

struct spectrum{
  uint16_t F1;  //405nm
  uint16_t F2;  //425nm
  uint16_t FZ;  //450nm
  uint16_t F3;  //475nm
  uint16_t F4;  //515nm
  uint16_t FY;  //555nm
  uint16_t F5;  //550nm
  uint16_t FXL; //600nm
  uint16_t F6;  //640nm
  uint16_t F7;  //690nm
  uint16_t F8;  //745nm
  uint16_t FNIR;//855nm
  uint16_t VIS; //clear chanel
  uint8_t FD;   //flicker(only AS7343 not AS7343L)
};
enum FD_starus:uint8_t{
  FD_100Hz_FLICKER,
  FD_120Hz_FLICKER,
  FD_100Hz_VALID,
  FD_120Hz_VALID,
  FD_SATURATION,
  FD_VALID7343
};
const uint8_t sensors[]={
    0x01,   //sensor 1
    0x02,   //sensor 2
    0x04,   //sensor 3
    0x08,   //sensor 4
    0x10,   //sensor 5
    0x20,   //sensor 6
    0x40,   //sensor 7
    0x80,   //sensor 8
};
extern spectrum spectrum_rezolt[8];

void get_all_spectrum();
void set_FD_persistence(uint8_t FD_pers);
void set_int_chanel(uint8_t CH,uint16_t low_th,uint16_t high_th,bool FD,bool smux_don,bool satuated,bool sp_int,bool fifo_int,bool system_int);
void set_GPIO(bool input,bool on);
void ENABLE(bool flicker_enable, bool wait, bool smux_en);
void set_ADC_timing(uint8_t Atime, uint16_t Astep);
void set_wait(uint8_t Wtime);
void set_power(bool low_power,bool wlong);
void set_ADC_gain(uint8_t gain);
void sleep_after_int(bool SAI);
void fifo_th(uint8_t fifo_th);
void led_config(bool LED ,uint8_t LED_drive);
void set_pers(uint8_t apres);
void set_config20(bool FD_8b,uint8_t auto_smux);
void set_ADC_FD_max_gain(uint8_t max_gain);
void set_AZ_config(uint8_t AZ_frequency);
void fifo_flicker_detect(bool fifo_FD);
void set_FD_timing(uint8_t gain, uint16_t time);
void smux_cmd(uint8_t smux_cmd);
void control(bool restart, bool auto_zero, bool fifo_clear, bool clear_SAI);
void fifo_map(bool status, bool ch0, bool ch1, bool ch2, bool ch3, bool ch4, bool ch5);
void write_fifo(uint8_t addr,uint16_t data);
uint16_t read_fifo(uint8_t addr);
spectrum get_spectrum();
uint16_t get_ID();
//byte"s: corect,A status,FD status, status[5:1]
uint64_t get_status();

void set_sensor(uint8_t switch_mask);

void set_register(uint8_t addr, uint8_t value);
void set_REG_BANK(bool reg_bank);
void get_byte_buff(uint8_t addr,uint8_t num);

#endif