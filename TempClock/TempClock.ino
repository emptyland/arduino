#include "si7021.h"
#include "bma220.h"
#include "DS1307.h"
#include "qs301.h"
#include <Wire.h>

DS1307 clock;
QS301Module qs31;
BMA220 acc;
Si7021 temp;

#define ROUTINE_TIME     0
#define ROUTINE_TEMP     1
#define ROUTINE_HUMIDITY 2
#define ROUTINE_MAX      3

int state = ROUTINE_TIME;

void accMontior(int mills) {
  static unsigned long o_acc_millis = millis();
  
  mills /= 10;
  while (mills--) {
    int8_t ax = 0, ay = 0, az = 0;
    acc.getMonitor3(&ax, &ay, &az);
    
    if ((ax > 20 || ay > 20 || az > 30) && millis() - o_acc_millis >= 100) {
      //    Serial.print("X=");   
      //    Serial.print(ax);         // print the character
      //    Serial.print("  "); 
      //    Serial.print("Y=");   
      //    Serial.print(ay);         // print the character
      //    Serial.print("  "); 
      //    Serial.print("Z=");           // print the character
      //    Serial.println(az); 

      state = (state + 1) % ROUTINE_MAX;
      o_acc_millis = millis();
      return;
    }
    delay(10);
  }
}

void clockRoutine() {
  static unsigned long o_clk_millis = millis();
  static int colonCounter = 0;

  clock.getTime();

  // hour
  qs31.setColor(COLOR_OFF);
  qs31.setNumber(clock.hour / 10);
  qs31.setColon(COLON_OFF);
  qs31.flush();
  qs31.setColor(COLOR_OFF);
  qs31.setNumber(clock.hour % 10);

  if (millis() - o_clk_millis >= 500) {
    o_clk_millis = millis();
    colonCounter++;
  }

  if (colonCounter % 2 == 0) {
    qs31.setColon(COLON_DOT | COLON_TOP);
  } 
  else {
    qs31.setColon(COLON_OFF);
  }

  qs31.flush();

  // minute
  qs31.setColor(COLOR_OFF);
  qs31.setNumber(clock.minute / 10);
  qs31.setColon(COLON_OFF);
  qs31.flush();
  qs31.setColor(COLOR_OFF);
  qs31.setNumber(clock.minute % 10);
  qs31.setColon(COLON_OFF);
  qs31.flush();  

  qs31.commit();
  accMontior(100);
}

void tempRoutine(bool isTemp) {
  double x = 0, y = 0;

  temp.getMonitor2(&x, &y);

  int color = 0;
  if (isTemp) {
    if (x < 0) {
      color = COLOR_BLUE;
    } 
    else {
      color = COLOR_YELLOW;
    }
  } 
  else {
    color = COLOR_GREEN;
  }

  // 12.34
  // 1234
  // / 1000 = 1
  // (/ 100) % 10
  // (/ 10) % 10
  int value = 0;
  if (isTemp) {
    value = abs((int)(x * 100));
  } 
  else {
    value = abs((int)(y * 100));
  }
//  Serial.print("value=");
//  Serial.print(value);
//  Serial.print("\n");

  qs31.setColor(color);
  qs31.setNumber(value / 1000);
  qs31.setColon(COLON_OFF);
  qs31.flush();

  qs31.setColor(color);
  qs31.setNumber((value / 100) % 10);
  qs31.setColon(COLON_DOT);
  qs31.flush();

  qs31.setColor(color);
  qs31.setNumber((value / 10) % 10);
  qs31.setColon(COLON_OFF);
  qs31.flush();

  qs31.setColor(color);
  qs31.setNumber(value % 10);
  qs31.setColon(COLON_OFF);
  qs31.flush();

  qs31.commit();
  accMontior(300);
}

void setup() {
  qs31.init(8, 7, 6, 5);
  clock.begin();

  acc.init(0);

  Serial.begin(9600);
//  clock.second = 0;
//  clock.minute = 05;
//  clock.hour = 11;
//  clock.dayOfMonth = 29;
//  clock.month = 8;
//  clock.year = 2015;
//  clock.setTime();
}


void loop() {

  switch (state) {
  case ROUTINE_TIME:
    clockRoutine();
    break;

  case ROUTINE_TEMP:
    tempRoutine(true);
    break;

  case ROUTINE_HUMIDITY:
    tempRoutine(false);
    break;
  }
}














