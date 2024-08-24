#define BLYNK_TEMPLATE_NAME "Rescue bot"
#define BLYNK_AUTH_TOKEN "paCeW73BlutUb0Q_7UxF6Z-zBfvALpnK"
#define BLYNK_TEMPLATE_ID "TMPL384oQCST2"


#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
char ssid[] = "Tsr";
char pass[] = "123456789";

int IN1 = 4;
int IN2 = 5;
int IN3 = 12;
int IN4 = 13;

BLYNK_WRITE(V1) { //move forward
  int state = param.asInt();
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, state);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, state);
}

BLYNK_WRITE(V2) { //move backward
  int state = param.asInt();
  digitalWrite(IN1, state);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, state);
  digitalWrite(IN4, LOW);
}

BLYNK_WRITE(V3) { //turn left
  int state = param.asInt();
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, state);
  digitalWrite(IN4, LOW);
}

BLYNK_WRITE(V4) { //turn right
  int state = param.asInt();
  digitalWrite(IN1, state);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();

}