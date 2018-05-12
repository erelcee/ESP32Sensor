#include <WiFi.h>
#include <HTTPClient.h>

#define NAME "FlowerBox 1"
#define WIFI_SSID "....."
#define WIFI_PASS "....."

void setup() {
  Serial.begin(115200);
  Serial.println("Starting WiFi connection");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected!");

  const int sensoron 12;
  const int redled 13;
  const int greenled 14;// redled and greenled on = orange
  const int inputadc 36;// output value 0 to 4093

  pinMode(sensoron, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);

}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    http.begin("https://william.dobbelaere.solutions/sensordash/api/update/" + String(NAME));
    http.addHeader("Content-Type", "application/json");

    digitalWrite(sensoron, HIGH);
    delay(500);
    int sensorValue = analogRead(inputadc);

    if (sensorValue < 250) {   // redled on = flowerbox is much too dry or sensor not connected
      digitalWrite(redled, HIGH);
      digitalWrite(greenled, LOW);
    }
    if ((sensorValue >= 250) && (sensorValue < 500)) { //orangeled on = the flowerbox needs water
      digitalWrite(redled, HIGH);
      digitalWrite(greenled, HIGH);
    }
    if (sensorValue >= 500 ) { // greenled on =  the flowerbox is okay
      digitalWrite(redled, LOW);
      digitalWrite(greenled, HIGH);
    }

    digitalWrite(sensoron, LOW);

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
