#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h> 

const char *ssid = "PSLab";
const char *password = "pslab123"; 

WiFiServer tcpServer(80);
WebSocketsServer webSocket(81);
WiFiClient activeTcpClient;


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT || type == WStype_BIN) {
    Serial.write(payload, length);
  }
}

String get_suffix() {
  String mac = WiFi.macAddress().substring(9, 17);
  mac.replace(":", "");
  return "_" + mac;
}


void setup() {
  Serial.setRxBufferSize(4096); 
  
  Serial.begin(1000000);
  Serial.setTimeout(0);

  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  WiFi.setOutputPower(10.0); 

  WiFi.softAP(ssid + get_suffix(), password);

  tcpServer.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}


void loop() {
  webSocket.loop();

  if (tcpServer.hasClient()) {
    if (!activeTcpClient || !activeTcpClient.connected()) {
      if (activeTcpClient) activeTcpClient.stop();
      activeTcpClient = tcpServer.available();
      activeTcpClient.setTimeout(0);
    } else {
      WiFiClient extraClient = tcpServer.available();
      extraClient.stop();
    }
  }


  size_t tcpAvailable = activeTcpClient.available();
  if (activeTcpClient && activeTcpClient.connected() && tcpAvailable > 0) {
    uint8_t inBuf[512];
    size_t toRead = (tcpAvailable > sizeof(inBuf)) ? sizeof(inBuf) : tcpAvailable;
    activeTcpClient.read(inBuf, toRead);
    
    Serial.write(inBuf, toRead); 
  }


  size_t bytesAvailable = Serial.available();
  if (bytesAvailable > 0) {
    uint8_t outBuf[1024]; 
    
    size_t toRead = (bytesAvailable > sizeof(outBuf)) ? sizeof(outBuf) : bytesAvailable;
    Serial.readBytes(outBuf, toRead);
    if (activeTcpClient && activeTcpClient.connected()) {
      activeTcpClient.write(outBuf, toRead);
    }
    
    webSocket.broadcastBIN(outBuf, toRead);
  }
}
