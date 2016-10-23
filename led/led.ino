#define LED_PORT 3

void setup() {
  pinMode(LED_PORT, OUTPUT);
}

void loop() {
  for (int i = 0; i < 256; ++i) {
    analogWrite(LED_PORT, i);
    delay(5);
  }
  
  for (int i = 0; i < 256; ++i) {
    analogWrite(LED_PORT, 255 - i);
    delay(5);
  }
}
