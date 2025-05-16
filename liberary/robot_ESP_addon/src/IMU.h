#ifndef IMU_H
#define IMU_H
#include "robot_ESP_addon.h"

struct int16_3{
  int16_t x;
  int16_t y;
  int16_t z;
};struct return_IMU{
  int16_t temp;
  int16_3 accel;
  int16_3 gyro;
};struct float_3{
  float x;
  float y;
  float z;
};struct float_IMU{
  float temp;     //[°C]
  float_3 accel;  //[m/s²]
  float_3 gyro;   //[°/s]
};

enum ODR_set {
  Hz1600=5,
  Hz800,
  Hz400,
  Hz200,
  Hz100,
  Hz50,
  Hz25,
  Hz12
};
enum g_set {
  g16,
  g8,
  g4,
  g2
};
enum dps_set {
  dps2000,
  dps1000,
  dps500,
  dps250
};
enum int_def{
  none,
  INT1,
  INT2
};
enum interupt_lation{
  self_test_int=0x8000,
  FSYNC_int    =0x4000,
  PLL_int      =0x2000,
  Reset_int    =0x1000,
  FIFO_ths_int =0x0400,
  FIFO_full_int=0x0200,
  AGC_int      =0x0100,
  SMD_int      =0x0008,
  X_WOM_int    =0x0004,
  Y_WOM_int    =0x0002,
  Z_WOM_int    =0x0001,
  step_det_int =0x0800,
  step_cnt_int =0x0080,
  tilt_det_int =0x0040,
    ff_det_int =0x0020,
  lowG_det_int =0x0010
};

void sensors_config_IMU(ODR_set poll_rate, g_set max_g, dps_set max_dps);
uint32_t int_gen_IMU(int_def self_test,int_def fsync,int_def PLL,int_def Reset,int_def data_ready,int_def FIFO_ths,int_def FIFO_full,int_def AGC,int_def I3C,int_def SMD,int_def X_WOM,int_def Y_WOM,int_def Z_WOM);
void set_int_IMU(uint32_t interupt);
uint16_t get_int_IMU();
float_IMU convert_data_IMU(return_IMU raw);
return_IMU get_data_IMU();

#endif