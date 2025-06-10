#include "robot_ESP_addon.h"
#include "LCD.h"
#include "motor.h"
#include "servo.h"
#include <arduino.h>
#include <Wire.h>
#ifdef _SSID
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#endif

#if not(Serial==HWCDCSerial)
#error "zet USB CDC on boot aan (tools->USB CDC on boot->enabel)"
#endif

void robot_begin(uint8_t servo_mode, bool LCD_backlight, bool motor_turn,bool IMU,bool machnetometer,bool lijnsensor){
    Wire.begin(10,11);
    Serial.begin();

    //ESPOTA
#ifdef _SSID
    WiFi.mode(WIFI_STA);
#ifdef _PASSWOORD
    WiFi.begin(_SSID, _PASSWOORD);
    Serial.println(_SSID);
    Serial.println(_PASSWOORD);
#else
    WiFi.begin(_SSID);
    Serial.println(_SSID);
#endif
    ArduinoOTA
        .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {  // U_SPIFFS
            type = "filesystem";
        }

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
        })
        .onEnd([]() {
        Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });

  ArduinoOTA.begin();
#endif
    //LCD
    backlight(LCD_backlight);
    clear_screen();
    //servo's
    init_servo(servo_mode);
    //motoren
    reset_encoders();
    set_mode(motor_turn?1:3);
    //machnetometer
    start_burst_mode();
    //IMU
    sensors_config_IMU(Hz100,g2,dps250);
    //lijnsensor
    for(uint8_t i;i<8;i++){
        set_sensor(sensors[i]);
        ENABLE(true,false,true);
        set_config20(false,3);
        set_ADC_gain(12);
    }
}