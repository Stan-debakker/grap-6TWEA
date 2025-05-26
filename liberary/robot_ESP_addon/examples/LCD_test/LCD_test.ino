#include <robot.h>

void setup() {
    init(0,false,true,false,false);
    Serial.begin(115200);
    print_text("Hello world!");    //print Hello world!
    print_char('\n');    //laat de cursor zakken
    print_text("LCD test");    //print LDC test
    set_cursor(20*3+0);    //verplaats naar onderste rij links
    print_text("gemaakt door Stan");
    delay(10000);    //wacht 10s
    clear_screen();
}

void loop() {
    //doe niets
}