#include <ESP32Servo.h>

int distancet = 100;  // Distance threshold in centimeters

// Motor pins
const int in1 = 14;
const int in2 = 27;
const int in3 = 26;
const int in4 = 25;

// ENA pins (for speed control)
const int ena1 = 32;  // Enable pin for Motor 1
const int ena2 = 33;  // Enable pin for Motor 2

// Ultrasonic sensor pins
const int trigPin = 13; 
const int echoPin = 12;

// Servo motor pin
const int servoPin = 15;

long duration;
int distance, distanceFront, distanceLeft, distanceRight;
Servo myservo;

// Motor speed (0-255)
int speed = 200;  // Adjust this value to control the speed
int speed1 = 255;

// Constants
const int maxRetries = 3;  // Maximum retries for getting a valid distance
const int servoDelay = 300; // Delay to allow the servo to reach the position

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
  
  Serial.begin(9600);  // Serial communication for debugging
}

void loop() {
  distanceFront = getDistance(90);  // Check the front distance
  
  if (distanceFront < distancet && distanceFront > 0) {  // If obstacle is closer than threshold
    robotStop();        // Stop the robot
    delay(500);   
    decision:      
    checkSurroundings();  // Check left and right surroundings
    
    // Determine the preferred direction
    int preferredDirection = (distanceLeft > distanceRight) ? 0 : 180;

    // Decide the direction based on the preferred direction and distances
    if ((preferredDirection == 0 && distanceLeft > distancet) ||
        (preferredDirection == 180 && distanceRight > distancet)) {
        if (preferredDirection == 0) {
            robotLeft();  // Move left if left side is clear
        } else {
            robotRight();  // Move right if right side is clear
        }
    } else {
      robotBack();  // Move backward if neither side is clear
      delay(1000);
      goto decision;
    }
    
    delay(500);         
    robotStop();  // Stop after turning
  } else if (distanceFront > distancet) {
    robotFront();  // Continue moving forward
  }
}

// Function to measure distance from the ultrasonic sensor at a given angle
int getDistance(int angle) {
  myservo.write(angle);  // Rotate servo to the specified angle
  delay(servoDelay);  // Give time for the servo to reach the position
  
  int validDistance = 0;
  for (int i = 0; i < maxRetries; i++) {
    // Send a pulse from the trig pin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Measure the time of the echo pulse
    duration = pulseIn(echoPin, HIGH);
    
    // Calculate distance in centimeters
    validDistance = duration * 0.034 / 2;
    
    // If a valid distance is obtained, break the loop
    if (validDistance > 0) {
      break;
    }
  }
  
  Serial.print("Distance at angle ");
  Serial.print(angle);
  Serial.print(": ");
  Serial.print(validDistance);
  Serial.println(" cm");

  return validDistance;
}

// Function to check surroundings by rotating the servo
void checkSurroundings() {
  distanceLeft = getDistance(0);  // Check left side distance
  distanceRight = getDistance(180);  // Check right side distance
}

// Move robot forward
void robotFront() {
  Serial.println("Robot Moving front");
  setMotor(in1, in2, HIGH, LOW);  // Set direction forward for Motor 1
  setMotor(in3, in4, HIGH, LOW);  // Set direction forward for Motor 2
  
  analogWrite(ena1, constrainSpeed(speed));  // Set speed for Motor 1
  analogWrite(ena2, constrainSpeed(speed));  // Set speed for Motor 2
}

// Move robot backward
void robotBack() {
  Serial.println("Robot Moving Back");
  setMotor(in1, in2, LOW, HIGH);  // Set direction backward for Motor 1
  setMotor(in3, in4, LOW, HIGH);  // Set direction backward for Motor 2
  
  analogWrite(ena1, constrainSpeed(speed));  // Set speed for Motor 1
  analogWrite(ena2, constrainSpeed(speed));  // Set speed for Motor 2
}

// Turn robot left
void robotLeft() {
  Serial.println("Robot Moving Left");
  setMotor(in1, in2, LOW, LOW);  // Stop Motor 1
  setMotor(in3, in4, HIGH, LOW);  // Run Motor 2 forward
  
  analogWrite(ena1, 0);  // Stop Motor 1
  analogWrite(ena2, constrainSpeed(speed1));  // Set speed for Motor 2
  delay(500);  // Turn for a certain time
  robotStop();
}

// Turn robot right
void robotRight() {
  Serial.println("Robot Moving Right");
  setMotor(in1, in2, HIGH, LOW);  // Run Motor 1 forward
  setMotor(in3, in4, LOW, LOW);  // Stop Motor 2
  
  analogWrite(ena1, constrainSpeed(speed1));  // Set speed for Motor 1
  analogWrite(ena2, 0);  // Stop Motor 2
  delay(500);  // Turn for a certain time
  robotStop();
}

// Stop the robot
void robotStop() {
  analogWrite(ena1, 0);  // Stop Motor 1
  analogWrite(ena2, 0);  // Stop Motor 2
}

// Utility function to set motor direction
void setMotor(int pin1, int pin2, int state1, int state2) {
  digitalWrite(pin1, state1);
  digitalWrite(pin2, state2);
}

// Utility function to constrain motor speed
int constrainSpeed(int speedValue) {
  return constrain(speedValue, 0, 255);
}
