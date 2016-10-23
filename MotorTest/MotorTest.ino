#define MOTOR_E1 3
#define MOTOR_M1 2
#define MOTOR_E2 5
#define MOTOR_M2 4

void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_E1, OUTPUT);
  pinMode(MOTOR_M1, OUTPUT);

  pinMode(MOTOR_E2, OUTPUT);
  pinMode(MOTOR_M2, OUTPUT);
}

void loop() 
{ 
  int value;
  for(value = 0 ; value <= 255; value+=5) 
  { 
    digitalWrite(MOTOR_M1, HIGH);   
    digitalWrite(MOTOR_M2, HIGH);       
    analogWrite(MOTOR_E1, value);   //PWM调速
    analogWrite(MOTOR_E2, value);   //PWM调速
    delay(100); 
  }
}

