#include "Arduino.h"
#include "Wire.h"

#define ADDRESS_AM2321 0x5C //not 0xB8
#define SIGN_WRITE 0x00
#define SDA_PIN A4
#define SCL_PIN A5

class Adafruit_AM2321{
public:
	Adafruit_AM2321();
	boolean begin(void);
	float readTemperature(void);
	float readHumidity(void);
	//void readTemp&Hum(void);
	
private:
	boolean readData(void);
	void SendWakeUp(void);
	float humidity, temperature;
};









//
//
////INO
//
//#include <Wire.h>
//#include <AM2321.h>
//
//Adafruit_AM2321 am2321;
//
//void setup()
//{
//	Serial.begin(115200);
//	Serial.println("AM2321 Test!");
//
//	if (!am2321.begin()) {
//		Serial.println("Sensor not found, check wiring & pullups!");
//		while (1);
//	}
//}
//
//void loop()
//{
//	Serial.print("Hum: "); Serial.println(am2321.readHumidity());
//	Serial.print("Temp: "); Serial.println(am2321.readTemperature());
//
//	delay(4000);
//}
