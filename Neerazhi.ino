#include <WiFi.h>
#include <HTTPClient.h>

#define TRIG_PIN 5
#define ECHO_PIN 18

const char* ssid = "shreya";
const char* password = "1111";


const char* serverURL = "http://Neerazhi.com/update";

int node_id = 1; 

float getWaterLevel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  float level = ((30 - distance) / 30) * 100;

  if (level < 0) level = 0;
  if (level > 100) level = 100;

  return level;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  float level = getWaterLevel();

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"node_id\":" + String(node_id) +
                  ",\"level\":" + String(level) + "}";

    int response = http.POST(json);

    Serial.println("Sent: " + json);
    Serial.println("Response: " + String(response));

    http.end();
  }

  delay(2000);
}