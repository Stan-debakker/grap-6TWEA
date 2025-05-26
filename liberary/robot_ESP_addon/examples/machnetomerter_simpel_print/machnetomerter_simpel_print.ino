#include <robot_ESP_addon.h>

void setup() {
  init(0,false,false,false,true);  //start de robot
  Serial.begin(115200);
  Serial.println("machnetomer simple print");
  delay(100); //wacht tot de machnetometer is gestart
}

void loop() {
  bool is_mT=false;//mT(true) of µT(false)
  //vraag de data op en zet het in mT(true) of µT(false)
  converted_machnetometer data=convert_data_machnetometer(get_data_machnetometer(),is_mT);

  //vraag veldstrengte op en zet op de seriele monitor x,y,z
  Serial.print(data.magneet.x,4);
  Serial.print(',');
  Serial.print(data.magneet.y,4);
  Serial.print(',');
  Serial.print(data.magneet.z,4);
  Serial.print(',');
  //vraag de temperatuur op
  Serial.print(data.temp,4);
  Serial.print(',');
  //vraag de spanning op over de machnetometer
  Serial.println(data.volt,4);

  //wacht 10ms
  delay(10);
}