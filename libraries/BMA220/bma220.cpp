#include "bma220.h"
#include <Wire.h>

void BMA220::init(uint8_t range) {
  Wire.begin();
  Wire.beginTransmission(0x0A); // address of the accelerometer
  // range settings
  Wire.write(0x22); //register address
  Wire.write(range); //can be set at"0x00""0x01""0x02""0x03", refer to Datashhet on wiki
  // low pass filter
  Wire.write(0x20); //register address
  Wire.write(0x05); //can be set at"0x05""0x04"......"0x01""0x00", refer to Datashhet on wiki
  Wire.endTransmission();

  switch(range)  //change the data dealing method based on the range u've set
  {
  case 0x00:
    divi_=16;
    break;
  case 0x01:
    divi_=8;
    break;
  case 0x02:
    divi_=4;
    break;
  case 0x03:
    divi_=2;
    break;
  default:
    //Serial.println("range setting is Wrong,range:from 0to 3.Please check!");
    while(1);
  }
}

void BMA220::getMonitor3(int8_t *ax, int8_t *ay, int8_t *az) {
  Wire.beginTransmission(0x0A); // address of the accelerometer
  // reset the accelerometer
  Wire.write(0x04); // Y data
  Wire.endTransmission();
  Wire.requestFrom(0x0A,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  {
    version_[0] = Wire.read(); // receive a byte as characte
  }
  *ax=(int8_t)version_[0]>>2;

  Wire.beginTransmission(0x0A); // address of the accelerometer
  // reset the accelerometer
  Wire.write(0x06); // Y data
  Wire.endTransmission();
  Wire.requestFrom(0x0A,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  {
    version_[1] = Wire.read(); // receive a byte as characte
  }
  *ay=(int8_t)version_[1]>>2;

  Wire.beginTransmission(0x0A); // address of the accelerometer
  // reset the accelerometer
  Wire.write(0x08); // Y data
  Wire.endTransmission();
  Wire.requestFrom(0x0A,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  {
    version_[2] = Wire.read(); // receive a byte as characte
  }
  *az=(int8_t)version_[2]>>2;
}
