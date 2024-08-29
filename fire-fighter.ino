int in1 = 2;
int in2 = 4;
int in3 = 7;
int in4 = 8;

int ena = 10;
int enb = 3;

int fanPin = 11; // Pin controlling the fan

int defaultSpeed = 100; // Default slow speed
int maxSpeed = 180;
int minSpeed = 60;

int flameLeft = A0;
int flameCenter = A1;
int flameRight = A2;

int stopThreshold = 500; // Threshold value to stop the robot when very close to the flame

void setup() {
  fireFightingRobotInit();
}

void loop() {
  int fLValue = analogRead(flameLeft);
  int fCValue = analogRead(flameCenter);
  int fRValue = analogRead(flameRight);

  Serial.print("flameLeft: ");
  Serial.print(fLValue);
  Serial.print(" flameCenter: ");
  Serial.print(fCValue);
  Serial.print(" flameRight: ");
  Serial.println(fRValue);

  // Check if the robot is very close to the flame
  if (fLValue < stopThreshold || fCValue < stopThreshold || fRValue < stopThreshold) {
    roboStop();  // Stop the robot if any sensor value is below the threshold
    extinguishFire(); // Turn on the fan to extinguish the fire
  } else {
    // Default movement is forward at a slow speed
    roboFront(defaultSpeed);

    // Adjust movement based on flame intensity (lower values indicate more intense flame)
    if (fLValue < fCValue && fLValue < fRValue) {
      // Move towards the left with speed proportional to intensity
      int speed = map(fLValue, 1023, 0, maxSpeed, minSpeed);
      roboLeft(speed);
    } else if (fRValue < fCValue && fRValue < fLValue) {
      // Move towards the right with speed proportional to intensity
      int speed = map(fRValue, 1023, 0, maxSpeed, minSpeed);
      roboRight(speed);
    } else if (fCValue < fLValue && fCValue < fRValue) {
      // Move forward with speed proportional to intensity
      int speed = map(fCValue, 1023, 0, maxSpeed, minSpeed);
      roboFront(speed);
    }
  }
  
  delay(200); // Small delay for stability
}

void fireFightingRobotInit(){
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(fanPin, OUTPUT); // Initialize the fan pin as output
  pinMode(flameLeft, INPUT);
  pinMode(flameCenter, INPUT);
  pinMode(flameRight, INPUT);
  digitalWrite(fanPin,1);
  Serial.begin(9600);
}

void robotControl(int ina, int inb, int inc, int ind, int speeda, int speedb){
  digitalWrite(in1, ina);
  digitalWrite(in2, inb);
  digitalWrite(in3, inc);
  digitalWrite(in4, ind);
  analogWrite(ena, speeda);
  analogWrite(enb, speedb); 
}

void roboFront(int speed) {
  robotControl(1, 0, 1, 0, speed, speed);
}

void roboBack(int speed) {
  robotControl(0, 1, 0, 1, speed, speed);
}

void roboRight(int speed) {
  robotControl(0, 0, 1, 0, 0, speed);
}

void roboLeft(int speed) {
  robotControl(1, 0, 0, 0, speed, 0);
}

void roboStop() {
  robotControl(0, 0, 0, 0, 0, 0);
}

void extinguishFire() {
  digitalWrite(fanPin, 0); // Turn on the fan
  delay(5000); // Keep the fan on for 5 seconds (adjust as needed)
  digitalWrite(fanPin, 1); // Turn off the fan
}
