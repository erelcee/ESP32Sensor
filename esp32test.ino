#include <WiFi.h>
#include <HTTPClient.h>

#define NAME "Grains"
#define WIFI_SSID "TobyWifi"
#define WIFI_PASS "doemdaka090486"

void setup() {  
  Serial.begin(115200);
  Serial.println("Starting WiFi connection");  

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("connected!");

  pinMode(26, OUTPUT);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    
    http.begin("https://william.dobbelaere.solutions/sensordash/api/update/" + String(NAME));
    http.addHeader("Content-Type", "application/json");

    digitalWrite(26, HIGH);
    delay(500);
    int sensorValue = analogRead(36);
    digitalWrite(26, LOW);

    int httpCode = http.POST("{\"value\":" + String(sensorValue) + "}");
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    } else {
      Serial.println("Error on HTTP request");
    }
 
    http.end();
  }
 
  delay(10000);
}
