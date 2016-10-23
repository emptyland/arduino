#include <PID_v1.h>
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Kalman.h"

#define MOTOR_E1 3
#define MOTOR_M1 2
#define MOTOR_E2 5
#define MOTOR_M2 4



MPU6050 acc;
// Create the Kalman instances
Kalman kalmanY;
Kalman kalmanX;

double gyroXangle, compAngleX, kalAngleX;
double gyroYangle, compAngleY, kalAngleY;

double setPoint = 1.2, output = 0;
double halfRange = 0.0;
const double kP = 30, kD = 65, kI = 1;
const int outputMax = 255, outputMin = -255;
const int minPower = 0;
uint32_t timer;

//Specify the links and initial tuning parameters
PID pid(&kalAngleX, &output, &setPoint, kP, kD, kI, DIRECT);

void motor(int pwm) {
  if (pwm > 0) {
    pwm = (pwm + minPower);
    if (pwm > 255)
      pwm = 255;
    
    // right
    digitalWrite(MOTOR_M2, HIGH);
    analogWrite(MOTOR_E2, pwm);

    // left
    digitalWrite(MOTOR_M1, LOW);
    analogWrite(MOTOR_E1, pwm);
  } 
  else {
    pwm = (-pwm + minPower);
    if (pwm > 255)
      pwm = 255;
    
    // left
    digitalWrite(MOTOR_M1, HIGH);
    analogWrite(MOTOR_E1, pwm);
  
    // right
    digitalWrite(MOTOR_M2, LOW);
    analogWrite(MOTOR_E2, pwm);
  }
}

void motorForward(int pwm) {
  // right
  digitalWrite(MOTOR_M2, LOW);
  analogWrite(MOTOR_E2, pwm);

  // left
  digitalWrite(MOTOR_M1, HIGH);
  analogWrite(MOTOR_E1, pwm);
}

void motorBackward(int pwm) {
  // left
  digitalWrite(MOTOR_M1, LOW);
  analogWrite(MOTOR_E1, pwm);

  // right
  digitalWrite(MOTOR_M2, HIGH);
  analogWrite(MOTOR_E2, pwm);
}

void motorStop() {
  // left
  digitalWrite(MOTOR_E1, LOW);
  digitalWrite(MOTOR_M1, HIGH);

  // right
  digitalWrite(MOTOR_E2, LOW);
  digitalWrite(MOTOR_M2, HIGH);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(MOTOR_E1, OUTPUT);
  pinMode(MOTOR_M1, OUTPUT);

  pinMode(MOTOR_E2, OUTPUT);
  pinMode(MOTOR_M2, OUTPUT);

  timer = micros();

  acc.initialize();

  //turn the PID on
  //fwdPID.Initialize();
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(20);
  pid.SetOutputLimits(outputMin, outputMax);

  int16_t ax, ay, az, gx, gy, gz;
  acc.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  double roll  = atan2(ay, az) * RAD_TO_DEG;
  double pitch = atan(-ax / sqrt(ay * ay + az * az)) * RAD_TO_DEG;

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;
}

void loop() {
  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  int16_t ax, ay, az, gx, gy, gz;
  acc.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  double roll  = atan2(ay, az) * RAD_TO_DEG;
  double pitch = atan(-ax / sqrt(ay * ay + az * az)) * RAD_TO_DEG;

  double gyroXrate = gx / 131.0; // Convert to deg/s
  double gyroYrate = gy / 131.0; // Convert to deg/s

  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
    kalmanX.setAngle(roll);
    compAngleX = roll;
    kalAngleX = roll;
    gyroXangle = roll;
  } 
  else
    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleX) > 90)
    gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);

  gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * dt;
  //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
  //gyroYangle += kalmanY.getRate() * dt;

  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

  // Reset the gyro angle when it has drifted too much
  if (gyroXangle < -180 || gyroXangle > 180)
    gyroXangle = kalAngleX;
  if (gyroYangle < -180 || gyroYangle > 180)
    gyroYangle = kalAngleY;

  pid.Compute();

  if (kalAngleX > setPoint + halfRange < 60) {
    motor(output);
  } 
  else if (kalAngleX < setPoint - halfRange > -60) {
    motor(output);
  } 
  else {
    motorStop();
  }

  Serial.print(roll); 
  Serial.print("\t");
  Serial.print(kalAngleX); 
  Serial.print("\t");
  Serial.print(gyroXangle); 
  Serial.print("\t");
  Serial.print(compAngleX); 
  Serial.print("\t");
  Serial.print(output);
  Serial.print("\n");

  //delay(30);
}













