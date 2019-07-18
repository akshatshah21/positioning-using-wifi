
#include <Arduino.h>
#include <ESP8266WiFi.h>
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
 
const char *ssid = "leo";
const char *pwd = "leopassword";
 
void setup() {
 
  Serial.begin(115200);
  WiFi.softAP(ssid, pwd);
  WiFi.setOutputPower(19.5);
  delay(1000);
  Serial.println();
  Serial.print("SSID:");
  Serial.println(ssid);
  Serial.print("Password:");
  Serial.println(pwd);

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  //Serial.print("Tx Power:");
  //Serial.println(WiFi.getTxPower());
 
}
 
void loop() {}