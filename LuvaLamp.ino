#include <ESP8266WiFi.h>          // ESP8266 Core WiFi Library
#include <DNSServer.h>            // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     // Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <Ethernet.h>

#define TOUCH_INPUT   5           // D1
#define PWM_R         2           // D4
#define PWM_G         0           // D3
#define PWM_B         4           // D2 

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 0, 177 };
byte server[] = { 192, 168, 0, 2 };
int port = 50005;
int colorWheelIndex = 0;
int colorR = 0;
int colorG = 0;
int colorB = 0;

WiFiManager wifiManager;
EthernetClient client;

void setup() 
{
  pinMode(TOUCH_INPUT, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(PWM_G, OUTPUT);
  pinMode(PWM_B, OUTPUT);
  digitalWrite(PWM_R, LOW);
  digitalWrite(PWM_G, LOW);
  digitalWrite(PWM_B, LOW);
  digitalWrite(LED_BUILTIN, LOW);   // LED ON

  Serial.println("Starting Wifi...");
  
//  if (digitalRead(TOUCH_INPUT))
//    wifiManager.startConfigPortal("LuvaLamp");
//  else
//  {
    wifiManager.autoConnect("LuvaLamp");
//  }

  ledToggle(LED_BUILTIN);
  delay(1000);
  ledToggle(LED_BUILTIN);

  Ethernet.begin(mac);

  if (client.connect(server, port)) ledToggle(LED_BUILTIN);

  displayColor();
}

void loop() 
{
  if (digitalRead(TOUCH_INPUT))
  {
    colorWheelIndex++;
    displayColor();
    delay(10);
  }
}

void ledToggle(int led)
{
  digitalWrite(led, !digitalRead(led));
}

void displayColor()
{
  colorWheelIndex = colorWheelIndex % 1530;

  if (colorWheelIndex < 255)
  {
    colorR = 255;
    colorG = colorWheelIndex;
    colorB = 0;
  }
  else if (colorWheelIndex < 510)
  {
    colorR = 255 - (colorWheelIndex % 255);
    colorG = 255;
    colorB = 0;
  }
  else if (colorWheelIndex < 765)
  {
    colorR = 0;
    colorG = 255;
    colorB = (colorWheelIndex % 255);
  }
  else if (colorWheelIndex < 1020)
  {
    colorR = 0;
    colorG = 255 - (colorWheelIndex % 255);
    colorB = 255;
  }
  else if (colorWheelIndex < 1275)
  {
    colorR = colorWheelIndex % 255;
    colorG = 0;
    colorB = 255;
  }
  else
  {
    colorR = 255;
    colorG = 0;
    colorB = 255 - (colorWheelIndex % 255);
  }

  analogWrite(PWM_R, colorR);
  analogWrite(PWM_G, colorG);
  analogWrite(PWM_B, colorB);
}
