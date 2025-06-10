#include <Wire.h>
#ifndef addr_IMU
#define addr_IMU 0x68
#endif

/*typedef enum intf_CLK_SEL{
  RC,
  PLL_then_RC,
  no_clok=3,
}intf_CLK_SEL_t;
typedef enum avrege_accel{
  x2,
  x4,
  x8,
  x16,
  x32,
  x64,
}avrege_accel_t;
typedef enum DLPF {  //digital low pass filter
  DF_bypassed,
  DF_Hz180,
  DF_Hz72,
  DF_Hz34,
  DF_Hz16,
  DF_Hz8,
  DF_Hz4
}DLPF_t;
typedef enum LPF {  //low pass filter
  F_bypassed,
  F_Hz180,
  F_Hz121,
  F_Hz73,
  F_Hz53,
  F_Hz34,
  F_Hz25,
  F_Hz16
}LPF_t;
typedef enum sens_mode {
  IMU_off,
  IMU_standby,
  IMU_LN=3
} sens_mode_t;
*/

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

int _max_G=2;     //[m²/s]
int _max_dps=250; //[ °/s]

void setup() {
  Serial.begin(115200);
  Wire.begin(10,11);
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x75);
  Wire.endTransmission(true);
  Wire.requestFrom(addr_IMU,1);
  Serial.println(Wire.read());
  sensors_config(Hz100,g2,dps250);
  delay(1000);
  Wire.beginTransmission(0x68);
  Wire.write(0x1F);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  while(Wire.available()){
    Serial.print(Wire.read(),HEX);  //F	9	9	0	31	41
    Serial.print('\t');
  }
  Serial.println();
}

void loop() {
  float_IMU data = convert_data(get_data());
  //return_IMU data =get_data();
  Serial.print(data.accel.x);
  Serial.print('\t');
  Serial.print(data.accel.y);
  Serial.print('\t');
  Serial.print(data.accel.z);
  Serial.print('\t');
  Serial.print(data.gyro.x);
  Serial.print('\t');
  Serial.print(data.gyro.y);
  Serial.print('\t');
  Serial.print(data.gyro.z);
  Serial.print('\t');
  Serial.println(data.temp);
  delay(10);
}

//config
  //apex (0x25-0x26)
  //WOM  (Ox27)
  //fifo (0x28-0x2A)
//data
  //fifo (0x2F-0x30) lost pkt
  //apex (Ox31-0x34)
  //fifo (Ox3D-0x3E) count
  //fifo (0x3F)      data
  //who am i  (0x75)
  //mreg (0x79-0x7E)

//
void sensors_config(ODR_set poll_rate, g_set max_g, dps_set max_dps){
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x1F);
  Wire.write(0x0F);
  Wire.write((max_dps<<5)|poll_rate);
  Wire.write((max_g<<5)|poll_rate);
  Wire.endTransmission();
  switch(max_g){
    case g2: _max_G=2;break;
    case g4: _max_G=4;break;
    case g8: _max_G=8;break;
    case g16: _max_G=16;break;
  }
  switch(max_dps){
    case dps250: _max_dps=250;break;
    case dps500: _max_dps=500;break;
    case dps1000: _max_dps=1000;break;
    case dps2000: _max_dps=2000;break;
  }
}
/*void temp_config(DLPF_t DLPF_config){ //0x00
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x22);
  Wire.write(DLPF_config<<4);
  Wire.endTransmission();
}void accel_config0(g_set_t g_config,ODR_set_t accel_ODR){  //0x06
  switch (g_config){
    case g16:
      _max_G=16;
      break;
    case g8:
      _max_G=8;
      break;
    case g4:
      _max_G=4;
      break;
    case g2:
      _max_G=2;
      break;
  }
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x21);
  Wire.write((g_config<<5)|accel_ODR);
  Wire.endTransmission();
}void accel_config1(avrege_accel_t avrege,LPF_t LPF_config){  //0x41
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x24);
  Wire.write((avrege<<4)|LPF_config);
  Wire.endTransmission();
}void gyro_config0(dps_set_t dps_config,ODR_set_t gyro_ODR){  //0x06
  switch (dps_config){
    case dps2000:
      _max_dps=2000;
      break;
    case dps1000:
      _max_dps=1000;
      break;
    case dps500:
      _max_dps=500;
      break;
    case dps250:
      _max_dps=250;
      break;
  }
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x20);
  Wire.write((dps_config<<5)|gyro_ODR);
  Wire.endTransmission();
}void gyro_config1(LPF_t LPF_config){ //0x31
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x23);
  Wire.write(LPF_config);
  Wire.endTransmission();
}void set_power(bool LP_CLK_SEL,sens_mode_t gyro_mode,sens_mode_t accel_mode){  //0x00
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x1F);
  Wire.write((LP_CLK_SEL?0x80:0x00)|(gyro_mode<<4)|accel_mode);
  Wire.endTransmission();
}

uint8_t intf_gen(bool fifo_count_records,bool fifo_big_endian,bool sensor_big_endian,bool I3C_SDR,bool I3C_DDR,intf_CLK_SEL_t CLK){
  uint8_t data=0;
  if(fifo_count_records){
    data|=0x40;
  }if(fifo_big_endian){
    data|=0x20;
  }if(sensor_big_endian){
    data|=0x10;
  }if(I3C_SDR){
    data|=0x08;
  }if(I3C_DDR){
    data|=0x04;
  }
  data|=(CLK&0x3);
  return data;
}void set_intf(uint8_t intf){
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x35);
  Wire.write(intf&0x70);
  Wire.write(intf&0x0F);
  Wire.endTransmission();
}*/


//maakt het makelijker om interupts te maken (return 0 als alles none is)
uint32_t int_gen(int_def self_test,int_def fsync,int_def PLL,int_def Reset,int_def data_ready,int_def FIFO_ths,int_def FIFO_full,int_def AGC,int_def I3C,int_def SMD,int_def X_WOM,int_def Y_WOM,int_def Z_WOM){
  uint32_t data=0;
  //self test done: interupt
    if(self_test==1){
      data|=0x80000000;
    }else if (self_test==2){
      data|=0x00008000;
    }
  //FSYNC: interrupt
    if(fsync==1){
      data|=0x40000000;
    }else if (fsync==2){
      data|=0x00004000;
    }
  //PLL ready: intreupt
    if(PLL==1){
      data|=0x20000000;
    }else if (PLL==2){
      data|=0x00002000;
    }
  //reset done: interupt (default INT1)
    if(Reset==1){
      data|=0x10000000;
    }else if (Reset==2){
      data|=0x00001000;
    }
  //data ready: interrupt
    if(data_ready==1){
      data|=0x08000000;
    }else if (data_ready==2){
      data|=0x00000800;
    }
  //FIFO threshold ready: intreupt
    if(FIFO_ths==1){
      data|=0x04000000;
    }else if (FIFO_ths==2){
      data|=0x00004000;
    }
  //FIFO full ready: intreupt
    if(FIFO_ths==1){
      data|=0x02000000;
    }else if (FIFO_ths==2){
      data|=0x00002000;
    }
  //UI AGC ready: interupt
    if(AGC==1){
      data|=0x01000000;
    }else if (AGC==2){
      data|=0x00000100;
    }
  //I3C protocol error: interrupt
    if(I3C==1){
      data|=0x00400000;
    }else if (I3C==2){
      data|=0x00000040;
    }
  //SMD: interupt
    if(SMD==1){
      data|=0x00080000;
    }else if (SMD==2){
      data|=0x00000008;
    }
  //WOM X as: interupt
    if(X_WOM==1){
      data|=0x00010000;
    }else if (X_WOM==2){
      data|=0x00000001;
    }
  //WOM Y as: interupt
    if(Y_WOM==1){
      data|=0x00020000;
    }else if (Y_WOM==2){
      data|=0x00000002;
    }
  //WOM Z as: interupt
    if(Z_WOM==1){
      data|=0x00040000;
    }else if (Z_WOM==2){
      data|=0x00000004;
    }
  return data;
}void set_int(uint32_t interupt){
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x2B);
  Wire.write((interupt>>24)&0xFF);
  Wire.write((interupt>>16)&0xFF);
  Wire.write((interupt>> 8)&0xFF);
  Wire.write((interupt>> 0)&0xFF);
  Wire.endTransmission();
}
//set de bit van de interuptbron
uint16_t get_int(){
  uint8_t data[4];
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x39);
  Wire.endTransmission(false);
  Wire.requestFrom(addr_IMU, 4, true);
  for(int i=0;i<4;i++){
    data[i]=Wire.read();
  }
  uint16_t int_return=(data[1]<<8)|data[2];
  if(data[0]==0x00){
    return 0;
  }
  if (data[3]&0x20!=0){
    int_return|=step_det_int;
  }
  int_return|=(data[3]&0x1E)<<3; //krijg de laatste 4 interupts (effisient)
  return int_return;
}

float_IMU convert_data(return_IMU raw){
  float_IMU data;
  data.temp=float(raw.temp)/128.0+25.0;
  data.accel.y=float(raw.accel.y*_max_G)/float(pow(2,15));
  data.accel.z=float(raw.accel.z*_max_G)/float(pow(2,15));
  data.accel.x=float(raw.accel.x*_max_G)/float(pow(2,15));
  data.gyro.x=float(raw.gyro.x*_max_dps)/float(pow(2,15));
  data.gyro.y=float(raw.gyro.y*_max_dps)/float(pow(2,15));
  data.gyro.z=float(raw.gyro.z*_max_dps)/float(pow(2,15));
  return data;
}return_IMU get_data(){
  return_IMU value;
  uint8_t get[14],i=0;
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x09);
  Wire.endTransmission(false);
  Wire.requestFrom(addr_IMU, 14, true);
  if (14==Wire.available()){  //data corect gekregen
    while(Wire.available()) {
      get[i] = Wire.read();
      i++;
    }
    value.temp=get[0]<<8|get[1];
    value.accel.x=get[2]<<8|get[3 ];
    value.accel.y=get[4]<<8|get[5 ];
    value.accel.z=get[6]<<8|get[7 ];
    value.gyro.x=get[8 ]<<8|get[9 ];
    value.gyro.y=get[10]<<8|get[11];
    value.gyro.z=get[12]<<8|get[13];
  }else{
    value.temp=0;
    value.accel.x=0;
    value.accel.y=0;
    value.accel.z=0;
    value.gyro.x=0;
    value.gyro.y=0;
    value.gyro.z=0;
    while(Wire.available()) Wire.read();  //clear the read buffer
  }
  return value;
}