#include <Wire.h>

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT);
}

void loop() {
  int val = digitalRead(3);
  Serial.print("val=");
  Serial.print(val);
  Serial.print("\n");
  delay(100);
}
