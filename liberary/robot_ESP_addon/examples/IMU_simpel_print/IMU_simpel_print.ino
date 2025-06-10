#include <robot_ESP_addon.h>

void setup() {
  robot_begin(0,false,false,true,false,false);  //start de robot

  Serial.println("IMU SIMPEL PRINT TEST");
}

void loop() {
  //vraag de data op en zet het in m/s² en °/s
  float_IMU data=convert_data_IMU(get_data_IMU());
  //vraag de accseleratie op en zet op de seriele monitor x,y,z
  Serial.print(data.accel.x, 4);
  Serial.print(',');
  Serial.print(data.accel.y, 4);
  Serial.print(',');
  Serial.print(data.accel.z, 4);
  Serial.print(',');
  //vraag de accseleratie op en zet op de seriele monitor x,y,z
  Serial.print(data.gyro.x, 4);
  Serial.print(',');
  Serial.print(data.gyro.y, 4);
  Serial.print(',');
  Serial.print(data.gyro.z, 4);
  Serial.print(',');
  //print de temperatuur (°C)
  Serial.println(data.temp, 4);

  //wacht 10ms
  delay(10);
}