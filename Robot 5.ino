#include <Arduino.h>

#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25

#define LEFT_SENSOR 32
#define RIGHT_SENSOR 33

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  Serial.begin(9600);
  stopMotors();
}

void loop() {
  int leftState = digitalRead(LEFT_SENSOR);
  int rightState = digitalRead(RIGHT_SENSOR);
  Serial.print("Left Sensor: ");
  Serial.println(leftState);

  // Line-following logic
  if (leftState == LOW && rightState == LOW) {  // Both sensors on the line
    moveForward();
  } else if (leftState == HIGH && rightState == LOW) {  // Left sensor off the line, right sensor on the line
    turnRight();
  } else if (leftState == LOW && rightState == HIGH) {  // Left sensor on the line, right sensor off the line
    turnLeft();
  } else {  // Both sensors off the line
    stopMotors();
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}