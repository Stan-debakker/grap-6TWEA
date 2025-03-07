#include <Wire.h>
#include <math.h>

#ifndef addr_IMU
#define addr_IMU 0x68
#endif

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
typedef enum intf_CLK_SEL{
  RC,
  PLL_then_RC,
  no_clok=3,
}intf_CLK_SEL_t;
typedef enum int_def{
  none,
  INT1,
  INT2
}int_def_t;
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
typedef enum ODR_set {
  Hz1600=5,
  Hz800,
  Hz400,
  Hz200,
  Hz100,
  Hz50,
  Hz25,
  Hz12
}ODR_set_t;
typedef enum g_set {
  g16,
  g8,
  g4,
  g2
} g_set_t;
typedef enum dps_set {
  dps2000,
  dps1000,
  dps500,
  dps250
} dps_set_t;
typedef enum sens_mode {
  IMU_off,
  IMU_standby,
  IMU_LN=3
} sens_mode_t;

struct uint16_3{
  uint16_t x;
  uint16_t y;
  uint16_t z;
};struct return_IMU{
  uint16_t temp;
  uint16_3 accel;
  uint16_3 gyro;
};struct float_3{
  float x=0;
  float y=0;
  float z=0;
};struct float_IMU{
  float temp;     //[°C]
  float_3 accel;  //[m/s²]
  float_3 gyro;   //[°/s]
};

hw_timer_t *timer = NULL;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  
}

class IMU{
public:
  IMU(uint8_t addr){
    addr_imu=addr;
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

  //set de thermometer digital lop pas filter
  void temp_config(DLPF_t DLPF_config){ //0x00
    Wire.beginTransmission(addr_imu);
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
    Wire.beginTransmission(addr_imu);
    Wire.write(0x21);
    Wire.write((g_config<<5)|accel_ODR);
    Wire.endTransmission();
  }void accel_config1(avrege_accel_t avrege,LPF_t LPF_config){  //0x41
    Wire.beginTransmission(addr_imu);
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
    Wire.beginTransmission(addr_imu);
    Wire.write(0x20);
    Wire.write((dps_config<<5)|gyro_ODR);
    Wire.endTransmission();
  }void gyro_config1(LPF_t LPF_config){ //0x31
    Wire.beginTransmission(addr_imu);
    Wire.write(0x23);
    Wire.write(LPF_config);
    Wire.endTransmission();
  }void set_power(bool LP_CLK_SEL,sens_mode_t gyro_mode,sens_mode_t accel_mode){  //0x00
    Wire.beginTransmission(addr_imu);
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
    Wire.beginTransmission(addr_imu);
    Wire.write(0x35);
    Wire.write(intf&0x70);
    Wire.write(intf&0x0F);
    Wire.endTransmission();
  }


  //maakt het makelijker om interupts te maken (return 0 als alles none is)
  uint32_t int_gen(int_def_t self_test,int_def_t fsync,int_def_t PLL,int_def_t Reset,int_def_t data_ready,int_def_t FIFO_ths,int_def_t FIFO_full,int_def_t AGC,int_def_t I3C,int_def_t SMD,int_def_t X_WOM,int_def_t Y_WOM,int_def_t Z_WOM){
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
    Wire.beginTransmission(addr_imu);
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
    Wire.beginTransmission(addr_imu);
    Wire.write(0x39);
    Wire.endTransmission(false);
    Wire.requestFrom(addr_imu, 4, true);
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
    data.temp=((float)raw.temp/128.0)+25.0;
    data.accel.x=(float)raw.accel.x*_max_G/pow(2,31);
    data.accel.y=(float)raw.accel.y*_max_G/pow(2,31);
    data.accel.z=(float)raw.accel.z*_max_G/pow(2,31);
    data.gyro.x=(float)raw.gyro.x*_max_dps/pow(2,31);
    data.gyro.y=(float)raw.gyro.y*_max_dps/pow(2,31);
    data.gyro.z=(float)raw.gyro.z*_max_dps/pow(2,31);
    return data;
  }return_IMU get_data(){
    return_IMU value;
    uint8_t get[14],i=0;
    Wire.beginTransmission(addr_imu);
    Wire.write(0x09);
    Wire.endTransmission(false);
    Wire.requestFrom(addr_imu, 14, true);
    if (14==Wire.available()){  //data corect gekregen
      while(Wire.available()) {
        get[i] = Wire.read();
        i++;
      }
      value.temp=get[0]<<8|get[1];
      value.accel.x=get[2]<<8|get[4 ];
      value.accel.x=get[4]<<8|get[5 ];
      value.accel.x=get[6]<<8|get[7 ];
      value.gyro.x=get[8 ]<<8|get[9 ];
      value.gyro.x=get[10]<<8|get[11];
      value.gyro.x=get[12]<<8|get[12];
    }else{
      value.temp=0;
      value.accel.x=0;
      value.accel.x=0;
      value.accel.x=0;
      value.gyro.x=0;
      value.gyro.x=0;
      value.gyro.x=0;
      while(Wire.available()) {Wire.read();}  //clear the read buffer
    }
    return value;
  }
protected:
  uint8_t addr_imu;
  int _max_G=2;     //[m²/s]
  int _max_dps=250; //[ °/s]
};

class IMU_position: private IMU {
public:
  IMU_position(uint8_t addr):IMU(addr){
    init_IMU();
    timer = timerBegin(1000000);                    //timer 1Mhz resolution
    timerAttachInterrupt(timer, &IMU_position::update_location);  //attach callback
    timerAlarm(timer, 1000000, true, 0);            //set time in us
  }
  //[°/s]
  float_3 get_dps_raw(){
    return dps_raw;
  }
  //[°/s]
  float_3 get_dps(){
    return dps;
  }
  //[°]
  float_3 get_hoek(){
    return hoek;
  }
  //[m/s²]
  float_3 get_versnelling_raw(){
    return versnelling_raw;
  }
  //[m/s²]
  float_3 get_versnelling(){
    return versnelling;
  }
  //[m/s]
  float_3 get_snelhijd(){
    return snelhijd;
  }
  //[m]
  float_3 get_posietie(){
    return posietie;
  }
private:
  static float_3 dps_raw;
  static float_3 dps;
  static float_3 hoek;
  static float_3 versnelling_raw;
  static float_3 versnelling;
  static float_3 snelhijd;
  static float_3 posietie;
  static float temp;
  uint64_t poll_rate=1000000;
  void init_IMU(){
    Wire.beginTransmission(addr_imu);
    Wire.write(0x1F); //ga naar het begin adderes voor alle comando's
    Wire.write(0x9F); //alles aanleggen
    Wire.write(0x45); //±500 dps, ODR:1.6kHz
    _max_dps=500;
    Wire.write(0x45); //±4 g, ODR:1.6kHz
    _max_G=4;
    Wire.write(0x70); // 4Hz LPF (temperatuur)
    Wire.write(0x70); //16Hz LPF (gyro)
    Wire.write(0x07); //16Hz LPF (accel), (2x average only LPM)
    Wire.endTransmission();
    set_int(0);       //leg alle interupts af
  }
  static void ARDUINO_ISR_ATTR update_location(){
    return_IMU raw_data = get_data();
    float_IMU new_data = convert_data(raw_data);
    temp=new_data.temp;
    //updata dps en versnelling door de sensor
    dps_raw=new_data.gyro;
    versnelling_raw=new_data.accel;
    //zet de dps_raw en versnelling_raw om in het vaste referentiefraam
      //https://www.w3schools.com/c/c_ref_math.php
      //https://en.wikipedia.org/wiki/Rotation_matrix#General_3D_rotations
    float_3 cos_hoek,sin_hoek;
    cos_hoek.x=cos(hoek.x); sin_hoek.x=sin(hoek.x);
    cos_hoek.y=cos(hoek.y); sin_hoek.y=sin(hoek.y);
    cos_hoek.z=cos(hoek.z); sin_hoek.z=sin(hoek.z);
    //rotatiematix op versnelling
    versnelling.x=cos_hoek.y*cos_hoek.z*versnelling_raw.x+(sin_hoek.x*sin_hoek.y*cos_hoek.z-cos_hoek.x*sin_hoek.z)*versnelling_raw.y+(cos_hoek.x*sin_hoek.y*cos_hoek.z+sin_hoek.x*sin_hoek.z)*versnelling_raw.z;
    versnelling.y=cos_hoek.y*sin_hoek.z*versnelling_raw.x+(sin_hoek.x*sin_hoek.y*sin_hoek.z-cos_hoek.x*cos_hoek.z)*versnelling_raw.y+(cos_hoek.x*sin_hoek.y*sin_hoek.z-sin_hoek.x*sin_hoek.z)*versnelling_raw.z;
    versnelling.z=-sin_hoek.y*versnelling_raw.x+sin_hoek.x*cos_hoek.y*versnelling_raw.y+cos_hoek.x*cos_hoek.y*versnelling_raw.z;
    //rotatiematix op dps[°/s]
    dps.x=cos_hoek.y*cos_hoek.z*dps_raw.x+(sin_hoek.x*sin_hoek.y*cos_hoek.z-cos_hoek.x*sin_hoek.z)*dps_raw.y+(cos_hoek.x*sin_hoek.y*cos_hoek.z+sin_hoek.x*sin_hoek.z)*dps_raw.z;
    dps.y=cos_hoek.y*sin_hoek.z*dps_raw.x+(sin_hoek.x*sin_hoek.y*sin_hoek.z-cos_hoek.x*cos_hoek.z)*dps_raw.y+(cos_hoek.x*sin_hoek.y*sin_hoek.z-sin_hoek.x*sin_hoek.z)*dps_raw.z;
    dps.z=-sin_hoek.y*dps_raw.x+sin_hoek.x*cos_hoek.y*dps_raw.y+cos_hoek.x*cos_hoek.y*dps_raw.z;
    //interpelaat de snelhijd, positie en hoek
    snelhijd.x+=versnelling.x*1000000/poll_rate;  //dit werkt omdat we dit 1 keer per seconde doen en we een het op een vast referentiefraam gebruiken (het begin)
    snelhijd.y+=versnelling.y*1000000/poll_rate;  //dit werkt omdat we dit 1 keer per seconde doen en we een het op een vast referentiefraam gebruiken (het begin)
    snelhijd.z+=versnelling.z*1000000/poll_rate;  //dit werkt omdat we dit 1 keer per seconde doen en we een het op een vast referentiefraam gebruiken (het begin)
    posietie.x+=snelhijd.x*1000000/poll_rate;     //dit werkt omdat we dit 1 keer per seconde doen en we een het op een vast referentiefraam gebruiken (het begin)
    posietie.y+=snelhijd.y*1000000/poll_rate;     //dit werkt omdat we dit 1 keer per seconde doen en we een het op een vast referentiefraam gebruiken (het begin)
    posietie.z+=snelhijd.z*1000000/poll_rate;     //dit werkt omdat we dit 1 keer per seconde doen en we een het op een vast referentiefraam gebruiken (het begin)
    hoek.x+=dps.x*1000000/poll_rate;              //dit werkt omdat we dit 1 keer per seconde doen en we een het op een vast referentiefraam gebruiken (het begin)
    hoek.y+=dps.y*1000000/poll_rate;              //dit werkt omdat we dit 1 keer per seconde doen en we een het op een vast referentiefraam gebruiken (het begin)
    hoek.z+=dps.z*1000000/poll_rate;              //dit werkt omdat we dit 1 keer per seconde doen en we een het op een vast referentiefraam gebruiken (het begin)
  }
};