#include <ESP32Servo.h>
#include <Wire.h>
#include <VL53L0X.h>

#define I2C_SDA 8
#define I2C_SCL 9

#define LIDSERVO_PIN 20
#define RACKSERVO_PIN 21

long TRIG_DISTNCE = 500;
int resetCycles = 50;
int cycles = 0;

Servo lidServo; // Create a servo object to control the motor1
Servo rackServo; // Create a servo object to control the motor2

VL53L0X sensor; // Create a sensor object to mesure distace

void setup() {
  Serial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);
  
  lidServo.attach(LIDSERVO_PIN);
  rackServo.attach(RACKSERVO_PIN);

  resetServos();

  sensor.setTimeout(500);
    if (!sensor.init())
    {
      Serial.println("Failed to detect and initialize sensor!");
      while (1) {}
    }

  sensor.startContinuous();
  delay(1000);
}

void loop() {
  if (cycles <= resetCycles) {
    cycles ++;
    Serial.print(cycles);
    Serial.print(" ");
  }
  
  if (checkPerson(TRIG_DISTNCE)) {
    kaboom();
    delay(750);
    resetServos();
    while (true);
  }
}

bool checkPerson(long TRIG_DISTNCE) {
  long distance = sensor.readRangeContinuousMillimeters();
  Serial.println(distance);
  return distance < TRIG_DISTNCE & distance > 50 & cycles > resetCycles;
}

void kaboom() {
  Serial.println("kaboom");
  lidServo.write(90);
  delay(500);
  rackServo.write(90);
  delay(200);
  lidServo.release();
  rackServo.release();
}

void resetServos() {
  for (int pos = 90; pos < 180; pos ++) { 
    lidServo.write(pos);              
    delay(5);
  }
  rackServo.write(180);
}