#include <AM2321.h>
Adafruit_AM2321 am2321;
void setup()
{
  Serial.begin(115200);
  Serial.println("AM2321 Test!");
//自检是否连接，并输出检测反馈
//if (!am2321.begin()) {
 // Serial.println("Sensor not found, check wiring & pullups!");
 // while (1);
 //}
}

void loop()
{
  Serial.print("Hum: "); Serial.println(am2321.readHumidity());
  Serial.print("Temp: "); Serial.println(am2321.readTemperature());
  
  delay(5000);
}

