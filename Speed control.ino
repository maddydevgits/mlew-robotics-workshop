#include <ESP32Servo.h>

// Motor pins
int in1 = 14;
int in2 = 27;
int in3 = 26;
int in4 = 25;

// ENA pins (for speed control)
int ena1 = 32;  // Enable pin for Motor 1
int ena2 = 33;  // Enable pin for Motor 2

// Ultrasonic sensor pins
int trigPin = 13;
int echoPin = 12;

// Servo motor pin
int servoPin = 15;

long duration;
int distance, distanceFront, distanceLeft, distanceRight;
Servo myservo;

// Motor speed (0-255)
int speed = 160;  // Adjust this value to control the speed

void setup() {
  // Initialize motor control pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Initialize ENA pins
  pinMode(ena1, OUTPUT);
  pinMode(ena2, OUTPUT);
  
  // Initialize other components
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  myservo.attach(servoPin);  // Attach the servo to the defined pin
  
  Serial.begin(115200);  // Serial communication for debugging
}

void loop() {
  distanceFront = getDistance(90);  // Check the front distance

  if (distanceFront < 20) {  // If obstacle is closer than 20 cm
    robotStop();        // Stop the robot
    delay(500);         
    checkSurroundings();  // Check left and right surroundings
    
    // Decide the direction based on the distances
    if (distanceLeft > distanceRight && distanceLeft > 20) {
      robotLeft();  // Move left if left side is clear
    } else if (distanceRight > 20) {
      robotRight();  // Move right if right side is clear
    } else {
      robotBack();  // Move backward if neither side is clear
    }
    
    delay(500);         
    robotStop();        // Stop after turning
  } else {
    robotFront();       // Continue moving forward
  }
}

// Function to measure distance from the ultrasonic sensor at a given angle
int getDistance(int angle) {
  myservo.write(angle);  // Rotate servo to the specified angle
  delay(500);  // Give time for the servo to reach the position
  
  // Send a pulse from the trig pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the time of the echo pulse
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in centimeters
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
  digitalWrite(in1, HIGH);  // Set direction forward for Motor 1
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);  // Set direction forward for Motor 2
  digitalWrite(in4, LOW);
  
  analogWrite(ena1, speed);  // Set speed for Motor 1
  analogWrite(ena2, speed);  // Set speed for Motor 2
}

// Move robot backward
void robotBack() {
  digitalWrite(in1, LOW);  // Set direction backward for Motor 1
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);  // Set direction backward for Motor 2
  digitalWrite(in4, HIGH);
  
  analogWrite(ena1, speed);  // Set speed for Motor 1
  analogWrite(ena2, speed);  // Set speed for Motor 2
}

// Turn robot left
void robotLeft() {
  digitalWrite(in1, LOW);  // Stop Motor 1
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);  // Run Motor 2 forward
  digitalWrite(in4, LOW);
  
  analogWrite(ena1, 0);  // Stop Motor 1
  analogWrite(ena2, speed);  // Set speed for Motor 2
  delay(500);  // Turn for a certain time
  robotStop();
}

// Turn robot right
void robotRight() {
  digitalWrite(in1, HIGH);  // Run Motor 1 forward
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);  // Stop Motor 2
  digitalWrite(in4, LOW);
  
  analogWrite(ena1, speed);  // Set speed for Motor 1
  analogWrite(ena2, 0);  // Stop Motor 2
  delay(500);  // Turn for a certain time
  robotStop();
}

// Stop the robot
void robotStop() {
  analogWrite(ena1, 0);  // Stop Motor 1
  analogWrite(ena2, 0);  // Stop Motor 2
}
