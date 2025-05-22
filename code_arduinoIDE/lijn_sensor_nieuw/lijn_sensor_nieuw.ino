#include <Wire.h>
#define ADDR_SA 0x39  //SA: spectrum analizer
#define ADDR_switch 0x71
#define ADDR_IO 0x21

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

spectrum spectrum_rezolt[8];
bool _LOW_POWER=false;
bool _REG_BANK=false;
bool _WLONG=false;

void setup() {
  Wire.begin(10,11);
}

void loop() {
  // put your main code here, to run repeatedly:

}
void begin(){
  for(uint8_t i;i!=0;i=i<<1){
    set_sensor(i);
    set_config20(false,3);
    init(true,false,true);
  }
}
spectrum* get_all_spectrum(){
  for(uint8_t i;i<8;i++){
    set_sensor(1<<i);
    spectrum_rezolt[i]=get_spectrum();
  }
  return spectrum_rezolt;
}

//CFG10 [0x65]
void set_FD_persistence(uint8_t FD_pers){
  set_register(0x65,FD_pers&0x07);
}
//CFG12 [0x66], int treshold [0x84-0x87], CFG9 [0xCA], INT EN [0xF9]
void set_int_chanel(uint8_t CH,uint16_t low_th,uint16_t high_th,bool FD,bool smux_don,bool satuated,bool sp_int,bool fifo_int,bool system_int){
  set_register(0x66,CH&0x07);
  set_REG_BANK(0x84<0x80);
  Wire.beginTransmission(ADDR_SA);
  Wire.write(0x84);
  Wire.write(low_th&0xFF);
  Wire.write(low_th>>8);
  Wire.write(high_th&0xFF);
  Wire.write(high_th>>8);
  Wire.endTransmission();
  set_register(0xCA,(FD?0x40:0x00)|(smux_don?0x10:0x00));
  set_register(0xF9,(satuated?0x80:0x00)|(sp_int?0x08:0x00)|(fifo_int?0x04:0x00)|(system_int?0x01:0x00));
}
//GPIO [0x6B]
void set_GPIO(bool input,bool on){
  if (input){
    set_register(0x6B,0x04);
  }else{
    set_register(0x6B,on?0x0A:0x02);
  }
}
//ENABLE [0x80]
//smux_en: auto reset
void init(bool flicker_enable, bool wait, bool smux_en){
  set_register(0x80,(flicker_enable?0x40:0x00)|(smux_en?0x10:0x00)|(wait?0x08:0x00)|0x03);
}
//ATIME [0x81], ASTEP [0xD4-0xD5]
void set_ADC_timing(uint8_t Atime, uint16_t Astep){
  set_register(0x81,Atime);
  Wire.beginTransmission(ADDR_SA);  //beter voor preformece dan 2 set_register
  Wire.write(0xD4);
  Wire.write(Astep&0xFF);
  Wire.write(Astep>>8);
  Wire.endTransmission();
}
//Wtime [0x83]
void set_wait(uint8_t Wtime){
  set_register(0x83,Wtime);
}
//CFG0 [0xBF]
void set_power(bool low_power,bool wlong){
  _LOW_POWER=low_power;
  _WLONG=wlong;
  _REG_BANK=true; //nodig om te gebruiken
  set_REG_BANK(false);  //nodig voor sturing (is vaak zo)
}
//CFG1 [0xC6]
void set_ADC_gain(uint8_t gain){
  set_register(0xC6,gain&0x1F);
}
//CFG3 [0xC7]
void sleep_after_int(bool SAI){
  set_register(0x7C,SAI?0x10:0x00);
}
//CFG8 [0xC9]
void fifo_th(uint8_t fifo_th){
  set_register(0xC9,fifo_th<<5);
}
//LED [0xCD]
void led_config(bool LED ,uint8_t LED_drive){
  set_register(0xCD,(LED?0x80:0x00)|(LED_drive&0x7F));
}
//PERS [0xCF]
void set_pers(uint8_t apres){
  set_register(0xCF,apres&0x0F);
}
//CFG20 [0xD6]
void set_config20(bool FD_8b,uint8_t auto_smux){
  set_register(0xD6,(FD_8b?0x80:0x00)|((auto_smux&0x03)<<5));
}
//AGC GAIN MAX [0xD7]
void set_ADC_FD_max_gain(uint8_t max_gain){
  set_register(0xD7,max_gain<<4);
}
//auto zero [0xDE]
void set_AZ_config(uint8_t AZ_frequency){
  set_register(0xDE,AZ_frequency);
}
//FIFO CFG0 [0xDF]
void fifo_flicker_detect(bool fifo_FD){
  set_register(0xDF,fifo_FD?0x80:0x00);
}
//FD TIME [0xE0,0XE2]
void set_FD_timing(uint8_t gain, uint16_t time){
  set_register(0xE0,time&0xFF);
  set_register(0xE2,(time>>8)&0x07|(gain<<3)&0xF8);
}
//CFG6 [0xF5]
void smux_cmd(uint8_t smux_cmd){
  set_register(0xF5,(smux_cmd&0x03)<<2);
}
//control [0xFA]
void control(bool restart, bool auto_zero, bool fifo_clear, bool clear_SAI){
  set_register(0xFA,(restart?0x08:0x00)|(auto_zero?0x04:0x00)|(fifo_clear?0x20:0x00)|(clear_SAI?0x01:0x00));
}
//fifo map [0xFC]
void fifo_map(bool status, bool ch0, bool ch1, bool ch2, bool ch3, bool ch4, bool ch5){
  set_register(0xFC,(status?0x01:0x00)|(ch0?0x02:0x00)|(ch1?0x04:0x00)|(ch2?0x08:0x00)|(ch3?0x10:0x00)|(ch4?0x20:0x00)|(ch5?0x40:0x00));
}
//fifo data(r/w) [0xFD-0xFF]
void write_fifo(uint8_t addr,uint16_t data){
  set_REG_BANK(false);
  Wire.beginTransmission(ADDR_SA);
  Wire.write(0xFD);
  Wire.write(addr);
  Wire.write(data&0xFF);
  Wire.write(data>>8);
  Wire.endTransmission();
}uint16_t read_fifo(uint8_t addr){
  while (Wire.available()) Wire.read(); //clear Wire buffer
  set_register(0xFD,addr);
  get_byte_buff(0xFE,2);
  if (Wire.available()==2){ //juiste aantal bytes gekregen
    return (Wire.read()|(Wire.read()<<8));
  }else{
    while (Wire.available()) Wire.read(); //clear Wire buffer
    return 0x0000;
  }
}
//data registers [0x95-0xB8,0xE3]
spectrum get_spectrum(){
  spectrum data;
  while (Wire.available()) Wire.read(); //clear Wire buffer
  get_byte_buff(0x95,36);
  get_byte_buff(0xE3,1);
  uint32_t temp_vis;
  data.FZ  =Wire.read()|Wire.read()<<8;
  data.FY  =Wire.read()|Wire.read()<<8;
  data.FXL =Wire.read()|Wire.read()<<8;
  data.FNIR=Wire.read()|Wire.read()<<8;
  temp_vis =Wire.read()|Wire.read()<<8;
  temp_vis+=Wire.read()|Wire.read()<<8;
  data.F2  =Wire.read()|Wire.read()<<8;
  data.F3  =Wire.read()|Wire.read()<<8;
  data.F4  =Wire.read()|Wire.read()<<8;
  data.F6  =Wire.read()|Wire.read()<<8;
  temp_vis+=Wire.read()|Wire.read()<<8;
  temp_vis+=Wire.read()|Wire.read()<<8;
  data.F1  =Wire.read()|Wire.read()<<8;
  data.F7  =Wire.read()|Wire.read()<<8;
  data.F8  =Wire.read()|Wire.read()<<8;
  data.F5  =Wire.read()|Wire.read()<<8;
  temp_vis+=Wire.read()|Wire.read()<<8;
  temp_vis+=Wire.read()|Wire.read()<<8;
  data.FD  =Wire.read();
  data.VIS =temp_vis/6;
  return data;
}

//ID's [0x58-0x5A]
//[15] ID corect, [14:11] AUXID, [10:8] REV ID, [7:0] ID
uint16_t get_ID(){
  while (Wire.available()) Wire.read(); //clear Wire buffer
  set_REG_BANK(true);
  get_byte_buff(0x58,3);
  if (Wire.available()==3){ //juiste aantal bytes gekregen
    return (((Wire.read()&0x0F)<<11)|((Wire.read()&0x07)<<8)|Wire.read());
  }else{
    while (Wire.available()) Wire.read(); //clear Wire buffer
    return 0x8000;
  }
}
//status regs [0x90-0x94,0xBB-0xBC,0xE3]
//byte's: corect,A status,FD status, status[5:1]
uint64_t get_status(){
  while (Wire.available()) Wire.read(); //clear Wire buffer
  set_REG_BANK(false);
  uint64_t data=0;
  get_byte_buff(0xE3,1);
  get_byte_buff(0xBB,2);
  get_byte_buff(0x90,5);
  //buff: 0xE3,0xBB,0xBC,0X9[0:4]
  if (Wire.available()==8){
    data|=Wire.read()<<(8*6);
    data|=Wire.read()<<(8*4);
    data|=Wire.read()<<(8*3);
    data|=Wire.read()<<(8*1);
    data|=Wire.read()<<(8*2);
    Wire.read();  //eat this place
    data|=Wire.read()<<(8*0);
    data|=Wire.read()<<(8*7);
    return data;
  }else{
    while (Wire.available()) Wire.read(); //clear Wire buffer
    return 0xFF00000000000000;
  }
}
//request num bytes from addr set them to Wire buff
void get_byte_buff(uint8_t addr,uint8_t num){
  Wire.beginTransmission(ADDR_SA);
  Wire.write(addr);
  Wire.endTransmission(true);
  Wire.requestFrom(ADDR_SA,num);
}
void set_REG_BANK(bool reg_bank){
  if (reg_bank!=_REG_BANK){
    _REG_BANK=reg_bank;
    Wire.beginTransmission(ADDR_SA);
    Wire.write(0xBF);
    Wire.write((_LOW_POWER?0x20:0x00)|(_REG_BANK?0x10:0x00)|(_WLONG?0x02:0x00));
    Wire.endTransmission();
  }
}
void set_register(uint8_t addr, uint8_t value){
  set_REG_BANK(addr<0x80);
  Wire.beginTransmission(ADDR_SA);
  Wire.write(addr);
  Wire.write(value);
  Wire.endTransmission();
}
void set_sensor(uint8_t switch_mask){
  Wire.beginTransmission(ADDR_switch);
  Wire.write(switch_mask);
  Wire.endTransmission();
}