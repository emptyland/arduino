#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <Wire.h>

#define SCLK 2
#define MOSI 3
#define DC   4
#define CS   5
#define RST  6

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

Adafruit_SSD1351 tft = Adafruit_SSD1351(CS, DC, RST);

#define RTC2403_Address   0x32  //RTC_Address 

class RTC2403 {
  public:
    enum { RTC_Address = 0x32 };

    void Begin() {
      Wire.begin();
    }

    void Read() {
      unsigned char date[7];
      unsigned char n = 0;

      Wire.requestFrom(RTC_Address, 7);
      while (Wire.available())
      {
        date[n++] = Wire.read();
      }
      delayMicroseconds(1);
      Wire.endTransmission();

      for (int i = 0; i < 7; i++)
      {
        if (i != 2)
          date[i] = (((date[i] & 0xf0) >> 4) * 10) + (date[i] & 0x0f);
        else
        {
          date[2] = (date[2] & 0x7f);
          date[2] = (((date[2] & 0xf0) >> 4) * 10) + (date[2] & 0x0f);
        }
      }

      second_ = date[0];
      minute_ = date[1];
      hour_   = date[2];
      week_   = date[3];
      day_    = date[4];
      month_  = date[5];
      year_   = date[6] + 1994;
    }

    void Write() {
      On();

      Wire.beginTransmission(RTC_Address);
      Wire.write(byte(0));//Set the address for writing
      Wire.write(byte(second_));//second:59
      Wire.write(byte(minute_));//minute:1
      Wire.write(mode12_ ? byte(hour_) : byte(hour_) | 0x90);//hour
      Wire.write(byte(week_));//weekday:Wednesday
      Wire.write(byte(day_));//day:26th
      Wire.write(byte(month_));//month:December
      Wire.write(byte(year_ - 1994));//year:2012
      Wire.endTransmission();

      Wire.beginTransmission(RTC_Address);
      Wire.write(0x12);   //Set the address for writing
      Wire.write(byte(0));
      Wire.endTransmission();

      Off();
    }

    void On() {
      Wire.beginTransmission(RTC_Address);
      Wire.write(0x10);//Set the address for writing as 10H
      Wire.write(0x80);//Set WRTC1=1
      Wire.endTransmission();

      Wire.beginTransmission(RTC_Address);
      Wire.write(0x0F);//Set the address for writing as OFH
      Wire.write(0x84);//Set WRTC2=1,WRTC3=1
      Wire.endTransmission();
    }

    void Off() {
      Wire.beginTransmission(RTC_Address);
      Wire.write(0x0F);   //Set the address for writing as OFH
      Wire.write(byte(0));//Set WRTC2=0,WRTC3=0
      Wire.write(byte(0));//Set WRTC1=0
      Wire.endTransmission();
    }

    int year() const {
      return year_;
    }
    int month() const {
      return month_;
    }
    int day() const {
      return day_;
    }
    int hour() const {
      return hour_;
    }
    int minute() const {
      return minute_;
    }
    int second() const {
      return second_;
    }
    int week() const {
      return week_;
    }

    void SetYear(int year) {
      year_ = year;
    }
    void SetMonth(int month) {
      month_ = month;
    }
    void SetDay(int day) {
      day_ = day;
    }
    void SetHour(int hour) {
      hour_ = hour;
    }
    void SetMinute(int minute) {
      minute_ = minute;
    }
    void SetSecond(int second) {
      second_ = second;
    }
    void SetWeek(int week) {
      week_ = week;
    }
    
    void SetHourMode(bool mode12) {
      mode12_ = mode12;
    }

  private:
    int year_;
    int month_;
    int day_;
    int hour_;
    int minute_;
    int second_;
    int week_;
    bool mode12_;
};

RTC2403 rtc;

void setup() {
  // put your setup code here, to run once:
  rtc.Begin();
//  rtc.SetYear(2015);
//  rtc.SetMonth(7);
//  rtc.SetDay(12);
//  rtc.SetHour(23 + 6);
//  rtc.SetMinute(59);
//  rtc.SetSecond(0);
//  rtc.SetWeek(0);
//  rtc.SetHourMode(false);
//  rtc.Write();

  tft.begin();
  tft.fillScreen(BLACK);
}

char n2Char(int n) {
  return '0' + n;
}

void printDateTime() {
  rtc.Read();
  
  char buf[] = {"00:00\n"};
  int hour = rtc.hour();
  buf[0] = n2Char(hour / 10);
  buf[1] = n2Char(hour % 10);
  
  int minute = rtc.minute();
  buf[3] = n2Char(minute / 10);
  buf[4] = n2Char(minute % 10);
 
  
  tft.setTextSize(4);
  tft.setTextColor(GREEN);
  tft.setCursor(0, 0);
  tft.print(buf);
  
  int second = rtc.second();
  int with = second / 60.0 * 128.0;
  //tft.fillRect(0, 64, 128, 12, BLACK);
  tft.fillRect(0, 64, with, 12, BLUE);
  
  if (second == 0) {
    tft.fillScreen(BLACK);
  }
}

void loop() {
  //tft.fillScreen(BLACK);
  printDateTime();
  delay(400);
}
