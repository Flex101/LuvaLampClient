#include <ESP8266WiFi.h>          // ESP8266 Core WiFi Library

#include <DNSServer.h>            // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     // Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#define TOUCH_INPUT 5             // D1

void setup() 
{
  pinMode(TOUCH_INPUT, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFiManager wifiManager;

  if (digitalRead(TOUCH_INPUT)) wifiManager.startConfigPortal("LuvaLamp");
  else wifiManager.autoConnect("LuvaLamp");
}

void loop() 
{
  digitalWrite(LED_BUILTIN, LOW);   // LED ON
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);   // LED OFF
  delay(500);
}
