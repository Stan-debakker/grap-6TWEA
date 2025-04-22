//
// Created by debakkers on 15-4-2025.
//

#include "machnetometer.h"
#include "robot_ESP_addon.h"
#include <arduino.h>
#include <Wire.h>

void print_resolt_machnetomerter(bool millie){
    converted_machnetometer data = convert_data(get_data(),millie);
    Serial.print(data.magneet.x);
    Serial.print("\t");
    Serial.print(data.magneet.y);
    Serial.print("\t");
    Serial.print(data.magneet.z);
    Serial.print("\t");
    Serial.print(data.temp);
    Serial.print("\t");
    Serial.print(data.volt);
    Serial.print("\t");
    Serial.print(data.corect?"corect":"fault");
    Serial.print("\t");
    Serial.println(data.status,BIN);
}

void start_burst_mode(){
    Wire.beginTransmission(addr_machnetometer);
    Wire.write(0x02<<1);
    Wire.write(0x22); //leg VmeasEN aan (voltage meter)
    Wire.write(0xA0);
    Wire.endTransmission(true);
    Wire.beginTransmission(addr_machnetometer);
    Wire.write(0x80);
    Wire.write(0x10);
    Wire.endTransmission();
}
struct float_3 change_raw_to_T(struct int16_3 raw,bool millie){
    float_3 output;
    if (millie){
        output.x=(float)raw.x*gainMultipliers[_gain]*7.14/1000; //7.14[µT/lsb16] raw[lsb16] 1000[mT/µT]
        output.y=(float)raw.y*gainMultipliers[_gain]*7.14/1000; //7.14[µT/lsb16] raw[lsb16] 1000[mT/µT]
        output.z=(float)raw.z*gainMultipliers[_gain]*7.14/1000; //7.14[µT/lsb16] raw[lsb16] 1000[mT/µT]
    }else{
        output.x=(float)raw.x*gainMultipliers[_gain]*7.14;      //7.14[µT/lsb16] raw[lsb16]
        output.y=(float)raw.y*gainMultipliers[_gain]*7.14;      //7.14[µT/lsb16] raw[lsb16]
        output.z=(float)raw.z*gainMultipliers[_gain]*7.14;      //7.14[µT/lsb16] raw[lsb16]
    }
    return output;
}
struct converted_machnetometer convert_data(struct return_machnetometer data_in,bool millie){
    converted_machnetometer data;
    data.corect=data_in.corect;
    data.status=data_in.status;
    data.magneet=change_raw_to_T(data_in.magneet,millie);
    data.temp=(float)data_in.temp/ 50 ; //50[lsb14/°C] temp[lsb14]
    data.volt=(float)data_in.volt/5500; //5500[lsb16/V] volt[lsb16]
    return data;
}
struct return_machnetometer get_data_machnetometer(){
    return_machnetometer value;
    uint8_t CRC=0,i=0;
    uint8_t get[12];
    Wire.beginTransmission(addr_machnetometer);
    Wire.write(0x80);
    Wire.endTransmission(false);
    Wire.requestFrom(addr_machnetometer, 12, true);
    while(Wire.available()) {
        get[i] = Wire.read();
        if(i>1){
            CRC=CRC_TABLE[CRC^get[i]];
        }
        i++;
    }
    value.corect=(CRC==get[1]);
    value.status=get[0];
    value.magneet.x=(get[2]<<8)|get[3];
    value.magneet.y=(get[4]<<8)|get[5];
    value.magneet.z=(get[6]<<8)|get[7];
    value.temp=(get[8 ]<<8)|get[9 ];
    value.volt=(get[10]<<8)|get[11];
    return value;
}