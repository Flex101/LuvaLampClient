#include <ESP8266WiFi.h>          // ESP8266 Core WiFi Library
#include <DNSServer.h>            // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     // Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266WiFi.h>

#define TOUCH_INPUT   5           // D1
#define PWM_R         2           // D4
#define PWM_G         0           // D3
#define PWM_B         4           // D2 

byte mac[] = { 0x84, 0xF3, 0xEB, 0x1A, 0x2A, 0x71 };
byte server[] = { 51, 148, 146, 218 };
int port = 50005;
int colorWheelIndex = 0;
float colorR = 0;
float colorG = 0;
float colorB = 0;
float brightness = 1.0;
bool fadeOut = false;
void(* resetFunc) (void) = 0;

WiFiManager wifiManager;
WiFiClient client;

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

  digitalWrite(PWM_B, HIGH);
  delay(3000);
  if (digitalRead(TOUCH_INPUT))
  {
    digitalWrite(PWM_R, HIGH);
    wifiManager.startConfigPortal("LuvaLamp");
  }
  else
  {
    wifiManager.autoConnect("LuvaLamp");
  }
  digitalWrite(PWM_B, LOW);
  digitalWrite(PWM_R, LOW);

  delay(1000);

  if (client.connect(server, port))
  {
    displayGreen();
    fadeOut = true;
  }
  else
  {
    displayRed();
    return;
  }
}

void loop() 
{
  if (fadeOut)
  {
    displayFade();
    delay(10);
  }
  else
  {
    if (digitalRead(TOUCH_INPUT))
    {
      colorWheelIndex++;
      displayColor();
      delay(10);
    }
  }

  if (client.connected())
  {
    if (client.available() == 5)
    {
      colorR = client.read();
      colorG = client.read();
      colorB = client.read();
      client.read();
      client.read();
      displayPulse();
    }
    else if (client.available() > 5)
    {
      while (client.available() > 0) client.read();
    }
  }
  else
  {
    resetFunc();
  }
}

void ledToggle(int led)
{
  digitalWrite(led, !digitalRead(led));
}

void displayRed()
{
  colorWheelIndex = 0;
  displayColor();
  brightness = 1.0;
  fadeOut= false;
}

void displayGreen()
{
  colorWheelIndex = 510;
  displayColor();
  brightness = 1.0;
  fadeOut= false;
}

void displayBlue()
{
  colorWheelIndex = 1020;
  displayColor();
  brightness = 1.0;
  fadeOut= false;
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

  analogWrite(PWM_R, colorR * brightness);
  analogWrite(PWM_G, colorG * brightness);
  analogWrite(PWM_B, colorB * brightness);
}

void displayFade()
{
  brightness -= 0.003;
  if (brightness > 0.0)
  {
    displayColor();
  }
  else
  {
    digitalWrite(PWM_R, LOW);
    digitalWrite(PWM_G, LOW);
    digitalWrite(PWM_B, LOW);
    brightness = 1.0;
    fadeOut = false;
  }
}

void displayOff()
{
  digitalWrite(PWM_R, LOW);
  digitalWrite(PWM_G, LOW);
  digitalWrite(PWM_B, LOW);
  brightness = 1.0;
  fadeOut = false;
}

void displayPulse()
{
  analogWrite(PWM_R, colorR);
  analogWrite(PWM_G, colorG);
  analogWrite(PWM_B, colorB);
}
