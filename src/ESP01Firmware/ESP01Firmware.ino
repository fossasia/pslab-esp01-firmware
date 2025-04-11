#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char *ssid = "PSLab";
const char *password = "pslab123"; // Empty or 8-63 characters long

WiFiServer server(80);

void setup() {
  Serial.begin(1000000);

  WiFi.setPhyMode(WIFI_PHY_MODE_11N);

  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  WiFi.softAP(ssid + getSuffix(), password);

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

// Returns a suffix based on the NIC specific part of the MAC address.
String getSuffix() {
  String mac = WiFi.macAddress().substring(9, 17); // remove vendor ID
  mac.remove(2, 1); // remove first colon
  mac.remove(4, 1); // remove second colon
  return "_" + mac;
}
