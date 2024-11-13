#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define BUZZER_PIN 12
#define BUTTON_PIN 4
#define BUTTON_STATE digitalRead(BUTTON_PIN)
#define SIGNAL digitalWrite(BUZZER_PIN, HIGH)
#define SILENT digitalWrite(BUZZER_PIN, LOW)

const char* ssid = "ESP32_AP";
const char* password = "password";

bool lock = false, lastState = false;
WiFiServer server(8080);
WiFiClient clients[2];

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Connecting...");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  
  server.begin();

  int clientCount = 0;

  while (clientCount < 2) {
    if (server.hasClient()) {
      for (int i = 0; i < 2; i++) {
        if (!clients[i] || !clients[i].connected()) {
          clients[i] = server.available();
          clientCount++;
          for (int j = 0; j < 3; j++) {
            SIGNAL;
            delay(100);
            SILENT;
            delay(100);
          }
          break;
        }
      }
    }
    delay(100);
  }
  delay(1000);

  lcd.clear();
  lcd.print("Ready");

  for (int i = 0; i < 3; i++) {
    SIGNAL;
    delay(500);
    SILENT;
    delay(500);
  }
}

void loop() {
  if (BUTTON_STATE == 0) lock = true;
  else lock = false;

  if (lastState != lock) {
    if (lock) {
      lcd.clear();
      lcd.print("Hold ");
    } else {
      lcd.clear();
      lcd.print("Ready ");
    }
  }
  lastState = lock;

  for (int i = 0; i < 2; i++) {
    if (clients[i].available()) {
      char receivedChar = clients[i].read();
      if (!lock) {
        Serial.write(receivedChar);
        Serial.write("\n");
      } 
      SIGNAL;
      delay(10);
      SILENT;
    }
  }
}
