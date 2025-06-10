#include "IMU.h"
#include <Wire.h>
#include <Arduino.h>
#define addr_IMU 0x68

int _max_G=2;     //[m²/s]
int _max_dps=250; //[ °/s]

void sensors_config_IMU(ODR_set poll_rate, g_set max_g, dps_set max_dps){
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

//maakt het makelijker om interupts te maken (return 0 als alles none is)
uint32_t int_gen_IMU(int_def self_test,int_def fsync,int_def PLL,int_def Reset,int_def data_ready,int_def FIFO_ths,int_def FIFO_full,int_def AGC,int_def I3C,int_def SMD,int_def X_WOM,int_def Y_WOM,int_def Z_WOM){
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
}void set_int_IMU(uint32_t interupt){
  Wire.beginTransmission(addr_IMU);
  Wire.write(0x2B);
  Wire.write((interupt>>24)&0xFF);
  Wire.write((interupt>>16)&0xFF);
  Wire.write((interupt>> 8)&0xFF);
  Wire.write((interupt>> 0)&0xFF);
  Wire.endTransmission();
}
//set de bit van de interuptbron
uint16_t get_int_IMU(){
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

float_IMU convert_data_IMU(return_IMU raw){
  float_IMU data;
  data.temp=float(raw.temp)/128.0+25.0;
  data.accel.y=float(raw.accel.y*_max_G)/float(pow(2,15));
  data.accel.z=float(raw.accel.z*_max_G)/float(pow(2,15));
  data.accel.x=float(raw.accel.x*_max_G)/float(pow(2,15));
  data.gyro.x=float(raw.gyro.x*_max_dps)/float(pow(2,15));
  data.gyro.y=float(raw.gyro.y*_max_dps)/float(pow(2,15));
  data.gyro.z=float(raw.gyro.z*_max_dps)/float(pow(2,15));
  return data;
}return_IMU get_data_IMU(){
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