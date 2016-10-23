#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define MAX_INPUT 430

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Clear the buffer.
  display.clearDisplay();
  display.display();
}

// 12 34
void printHumidity(int percent) {
  Serial.println(percent); 
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  display.write(' ');
  display.write(' ');
  
  char c = (percent / 1000) % 10;
  display.write(c + '0');
  c = (percent / 100) % 10;
  display.write(c + '0');
  
  display.write('.');
  
  c = (percent / 10) % 10;
  display.write(c + '0');
  c = (percent % 10);
  display.write(c + '0');
  
  display.write('%');
  display.println();
}

void drawHumidity(int percent) {
  
  display.drawRect(0, 34, 128, 16, WHITE);
  display.fillRect(0, 34, percent, 16, WHITE);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t input = analogRead(5);

  Serial.print("Moisture Sensor Value:");
  Serial.println(input);     
  delay(200);
  
  display.clearDisplay();
  printHumidity(input * 10000 / MAX_INPUT);
  drawHumidity(input * 128 / MAX_INPUT);
  display.display();
}
