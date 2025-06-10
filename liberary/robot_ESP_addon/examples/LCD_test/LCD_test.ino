#include <robot_ESP_addon.h>

void setup() {
    robot_begin(0,false,true,false,false,false);

    print_text("Hello world!");    //print Hello world!
    print_char('\n');    //laat de cursor zakken
    print_text("LCD test");    //print LDC test
    set_cursor(20*3+0);    //verplaats naar onderste rij links
    print_text("gemaakt door Stan");
    delay(10000);    //wacht 10s
    backlight(false);//leg het schermlicht af (false)
    delay(10000);    //wacht 10s
    clear_screen();
}

void loop() {
    //doe niets
}