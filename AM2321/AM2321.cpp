#include "AM2321.h"
#if defined(__AVR__)
#include <util/delay.h>
#endif

Adafruit_AM2321::Adafruit_AM2321() {
}


boolean Adafruit_AM2321::begin(void) {
	Wire.begin();

	// try to read data, as a test
	return readData();
}

boolean Adafruit_AM2315::readData(void){

	byte fuctionCode = 0;
	byte dataLength = 0;
	byte humiHigh = 0;
	byte humiLow = 0;
	byte tempHigh = 0;
	byte tempLow = 0;
	byte crcHigh = 0;
	byte crcLow = 0;

	humidity = 0;
	temperature = 0;
	unsigned int crcCode = 0;

	byte backupTWCR = 0;

	//step 1. wake up the sensor激活传感器
	SendWakeUp();
	backupTWCR = TWCR;

	//step 2. send command 
	Wire.begin();//建立IIC通信
	Wire.beginTransmission(ADDRESS_AM2321);//建立与传感器的通信
	Wire.write(0x03);//温度低位
	Wire.write(0x00);//湿度高位
	Wire.write(0x04);//保留
	Wire.endTransmission();

	delayMicroseconds(1500);

	//step 3. read data, and recover the TWCR register
	Wire.requestFrom(ADDRESS_AM2321, 8);
	fuctionCode = Wire.read();
	dataLength = Wire.read();
	humiHigh = Wire.read();
	humiLow = Wire.read();
	tempHigh = Wire.read();
	tempLow = Wire.read();
	crcLow = Wire.read();
	crcHigh = Wire.read();

	//get the result
	humidity = ((humiHigh << 8) | humiLow)/10.0;
	temperature = ((tempHigh << 8) | tempLow)/10.0;
	crcCode = (crcHigh << 8) | crcLow;
	CheckCRC();

	//recover the TWCR register, e.g. disable the I2C bus
	TWCR = backupTWCR;

	delay(4000);

	return true;
}

float Adafruit_AM2321::readTemperature(void) {
	if (!readData()) return NAN;
	return temperature;
}

float Adafruit_AM2321::readHumidity(void) {
	if (!readData()) return NAN;
	return humidity;
}

void SendWakeUp()
{
	//set pinmode
	pinMode(SCL_PIN, OUTPUT);
	pinMode(SDA_PIN, OUTPUT);
	digitalWrite(SCL_PIN, HIGH);
	digitalWrite(SDA_PIN, HIGH);

	//issue a START condition
	delayMicroseconds(5);
	digitalWrite(SDA_PIN, LOW);
	delayMicroseconds(5);
	digitalWrite(SCL_PIN, LOW);
	delayMicroseconds(5);

	//send ADDRESS+W
	shiftOut(SDA_PIN, SCL_PIN, MSBFIRST, ((ADDRESS_AM2321 << 1) | SIGN_WRITE));

	//send clock for ack
	pinMode(SDA_PIN, INPUT_PULLUP);// or INPUT mode
	delayMicroseconds(5);
	digitalWrite(SCL_PIN, HIGH);
	delayMicroseconds(5);
	digitalWrite(SCL_PIN, LOW);
	pinMode(SDA_PIN, OUTPUT);
	digitalWrite(SDA_PIN, LOW);

	delayMicroseconds(1000);

	//issue a STOP condition
	digitalWrite(SCL_PIN, HIGH);
	delayMicroseconds(5);
	digitalWrite(SDA_PIN, HIGH);
}

void CheckCRC() //from the datesheet
{
	byte backValues[] = { fuctionCode, dataLength, humiHigh, \
		humiLow, tempHigh, tempLow };
	unsigned int crc = 0xFFFF;
	int i;
	int len = 6;
	int j = 0;
	while (len--)
	{
		crc ^= backValues[j];
		j++;
		for (i = 0; i < 8; i++)
		{
			if (crc & 0x01)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}
	if (crc == crcCode)
	{
		Serial.println("CRC checked.");
	}
	else
	{
		Serial.println("CRC Error!");
	}
}
