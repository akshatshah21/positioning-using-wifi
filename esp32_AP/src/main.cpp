
#include <Arduino.h>
/*
#include <WiFi.h>
#include <WiFiAP.h>

const char * ssid = "esp32";
const char * pwd = "esp32password";


void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    //Serial.println();
    Serial.println("Configuring Accces Point");
    WiFi.softAP(ssid,pwd);
    Serial.print("IP address:");
    Serial.println(WiFi.softAPIP());

}

void loop() {
  // put your main code here, to run repeatedly:
    WiFiClient client;
    if(client)
    {
        Serial.println("Client connected");

    }
}
*/

#include "WiFi.h"
 
const char *ssid = "esp32";
const char *pwd = "passwordx";
 
void setup() {
 
  Serial.begin(115200);
  WiFi.softAP(ssid, pwd);
  delay(1000);
  Serial.println();
  Serial.print("SSID:");
  Serial.println(ssid);
  Serial.print("Password:");
  Serial.println(pwd);

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
 
}
 
void loop() {}