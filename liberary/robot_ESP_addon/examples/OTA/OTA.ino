#define _SSID "netwerk naam"
#define _PASSWOORD "paswoord (niet verplicht)"    //indien niet gebruikt verwijderen
//dit moet voor #include <robot_ESP_addon.h>
#include <robot_ESP_addon.h>

void setup() {
    //begin de robot
    robot_begin(0,false,false,false,false,false);
}

void loop() {
    //verijst in de loop
    ArduinoOTA.handle();
}