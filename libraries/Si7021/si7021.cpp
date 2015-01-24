#include "si7021.h"
#include <Wire.h>

void Si7021::init() {
  Wire.begin();
  Wire.beginTransmission(0x40);
  Wire.endTransmission();
}

void Si7021::getMonitor2(double *temp, double *humidity) {
  int X0,X1,Y0,Y1,Y2,Y3;
  double X,Y,X_out,Y_out1,Y_out2;

  /**发送温度测量命令**/
  Wire.beginTransmission(0x40);
  Wire.write(0xE3);                               //发送读温度命令
  Wire.endTransmission();

  /**读取温度数据**/
  Wire.requestFrom(0x40,2);                       //回传数据

  if(Wire.available()<=2);
  {
    X0 = Wire.read();
    X1 = Wire.read();
    X0 = X0<<8;
    X_out = X0+X1;
  }

  /**计算并显示温度**/
  X=(175.72*X_out)/65536;
  *temp=X-46.85;

  /**发送湿度测量命令**/
  Wire.beginTransmission(0x40);
  Wire.write(0xE5);
  Wire.endTransmission();

  /**读取湿度数据**/
  Wire.requestFrom(0x40,2);
  if(Wire.available()<=2);
  {
    Y0 = Wire.read();
    Y2=Y0/100;
    Y0=Y0%100;
    Y1 = Wire.read();
    Y_out1 = Y2*25600;
    Y_out2 = Y0*256+Y1;
  }

  /**计算并显示湿度**/
  Y_out1 = (125*Y_out1)/65536;
  Y_out2 = (125*Y_out2)/65536;
  Y = Y_out1+Y_out2;
  *humidity=Y-6;
}
