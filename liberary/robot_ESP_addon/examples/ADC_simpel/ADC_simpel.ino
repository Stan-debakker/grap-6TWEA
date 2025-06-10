#include <robot_ESP_addon.h>

void setup() {
    //begin de robot
    robot_begin(0,false,false,false,false,false);

    //start met meten op pin 0, constant
    set_input_pin_ADC(0,false);
}

void loop() {
    //print de data van de ADC
    Serial.println(get_data_ADC());
    delay(500);    //wacht 0,5s
}