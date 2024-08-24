#include "BluetoothSerial.h"

// Motor pins
int in1 = 14;  
int in2 = 27;  
int in3 = 26;
int in4 = 25;

BluetoothSerial ESP_BT;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
  ESP_BT.begin("ESP32_Robot");
  Serial.println("Bluetooth Device is Ready to Pair");
}

void loop() {
  if (ESP_BT.available()) {
    char command = ESP_BT.read(); 
    
    Serial.println(command); 
    switch (command) {
      case 'F':  // Forward
        robotFront();
        break;
      case 'B':  // Backward
        robotBack();
        break;
      case 'L':  // Left
        robotLeft();
        break;
      case 'R':  // Right
        robotRight();
        break;
      case 'S':  // Stop
        robotStop();
        break;
      default:
        robotStop();
        break;
    }
  }
}

//robot forward
void robotFront() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

//robot backward
void robotBack() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

//robot left
void robotLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

//robot right
void robotRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

//robot Stop
void robotStop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
