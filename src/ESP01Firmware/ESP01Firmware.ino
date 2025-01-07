#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char *ssid = "PSLab";
const char *password = "pslab";

WiFiServer server(80);

void setup() {
  Serial.begin(1000000);

  WiFi.setPhyMode(WIFI_PHY_MODE_11N);

  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  WiFi.softAP(ssid, password);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char dataFromClient = client.read();
        Serial.write(dataFromClient);
      }

      if (Serial.available()) {
        char dataFromMCU = Serial.read();
        client.write(dataFromMCU);
      }
    }
    client.stop();
  }
}