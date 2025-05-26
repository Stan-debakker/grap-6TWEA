#include <robot.h>

void setup() {
  init(0,false,false,true,false);  //start de robot
  Serial.begin(115200);
  Serial.println("IMU SIMPEL PRINT TEST");
}

void loop() {
  //vraag de data op en zet het in m/s² en °/s
  float_IMU data=convert_data_IMU(get_data_IMU());
  //vraag de accseleratie op en zet op de seriele monitor x,y,z
  Serial.print(float_IMU.accel.x, 4);
  Serial.print(',');
  Serial.print(float_IMU.accel.y, 4);
  Serial.print(',');
  Serial.print(float_IMU.accel.z, 4);
  Serial.print(',');
  //vraag de accseleratie op en zet op de seriele monitor x,y,z
  Serial.print(float_IMU.gyro.x, 4);
  Serial.print(',');
  Serial.print(float_IMU.gyro.y, 4);
  Serial.print(',');
  Serial.print(float_IMU.gyro.z, 4);
  Serial.print(',');
  //print de temperatuur (°C)
  Serial.println(float_IMU.temp, 4);

  //wacht 10ms
  delay(10);
}