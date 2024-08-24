//2.Autonomous Robot with ULTRASONIC SENSOR ,Servo Motor and ESP32,L298N

#include <Servo.h>

// Motor pins
int in1 = 14;
int in2 = 27;
int in3 = 26;
int in4 = 25;

// Ultrasonic sensor pins
int trigPin = 13;
int echoPin = 12;

// Servo motor pin
int servoPin = 16;

long duration;
int distance, distanceFront, distanceLeft, distanceRight;
Servo myservo;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  myservo.attach(servoPin);
  
  Serial.begin(9600);
}

void loop() {
  distanceFront = getDistance(90);  // Check the front distance

  if (distanceFront < 20) {  // If obstacle is closer than 20 cm
    robotStop();   
    delay(500);         
    checkSurroundings();  
    if (distanceLeft > distanceRight && distanceLeft > 20) {
      robotLeft(); 
    } else if (distanceRight > 20) {
      robotRight();  
    } else {
      robotBack();  
    }
    
    delay(500);         
    robotStop();        
  } else {
    robotFront();      
  }
}
int getDistance(int angle) {
  myservo.write(angle);  
  delay(500);  // Give time for the servo to reach the position
  
  // Send a pulse from the trig pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

// Function to check surroundings by rotating the servo
void checkSurroundings() {
  distanceLeft = getDistance(0);  // Check left side distance
  distanceRight = getDistance(180);  // Check right side distance
}

// Move robot forward
void robotFront() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Move robot backward
void robotBack() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

// Turn robot left
void robotLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(500);
  robotStop();
}

// Turn robot right
void robotRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(500);
  robotStop();
}

// Stop the robot
void robotStop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
