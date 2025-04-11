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

/**
 * @brief Return a suffix based on the NIC specific part of the MAC address
 *
 * @return The final three octets of the MAC address, prefixed with underscore
 */
String get_suffix()
{
   // Remove vendor ID
  String mac = WiFi.macAddress().substring(9, 17);
  // Remove colons
  mac.replace(":", "")
  return "_" + mac;
}
