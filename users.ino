#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "ESP32_AP";
const char* password = "password";
IPAddress serverIP(192, 168, 4, 1);

WiFiClient client;

const int sensorPin = 13;
const int buzzerPin = 12;

int previousSensorState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  
  while (!client.connected()) {
    if (client.connect(serverIP, 8080)) {
    } else {
      delay(1000);
    }
  }
}

void loop() {
  int sensorValue = digitalRead(sensorPin);

  if (sensorValue == LOW) {
    char message = 65;
    client.print(message);
    digitalWrite(buzzerPin, HIGH);
    sensorValue = digitalRead(sensorPin);
    while (sensorValue == LOW) {
      sensorValue = digitalRead(sensorPin);
    }
    digitalWrite(buzzerPin, LOW);
  }
}
