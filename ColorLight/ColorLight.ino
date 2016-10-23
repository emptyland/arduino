#define PIN_RED   0
#define PIN_GREEN 1
#define PIN_BLUE  2

void setup() {
  //Serial.begin(9600);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(4, INPUT);
  //pinMode(0, OUTPUT);
}

void loop() {
  int val = digitalRead(4);
  digitalWrite(PIN_RED,   val);
  digitalWrite(PIN_GREEN, val);
  digitalWrite(PIN_BLUE,  val);
}
