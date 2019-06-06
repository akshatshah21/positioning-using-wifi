#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STA_SSID
#define STA_SSID "esp32"
#define STA_PWD "password"
#endif
const char * ssid = STA_SSID;
const char * pwd = STA_PWD;

ESP8266WiFiMulti WiFiMulti;


//Implementing Kalman Filter:
double ARm, prev_Re= 0, prev_Pre = 1 ,Rre, prev_Rre, Pre, prev_Pe = 1, k, R = 0.1, Re, Pe;

void kalman_filter()
{
ARm =WiFi.RSSI();
Rre = prev_Re;
Pre = prev_Pe;
k = Pre/(Pre + R);
Re = Rre + k*(ARm-Rre);
Pe = (1-k)*Pre;
// Serial.println("RSSI estimated :");
// Serial.println(Re);
prev_Re = Re;
prev_Pe = Pe;

}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid,pwd);

    //Serial.println();
    //Serial.println();
    //Serial.println("Waiting for ESP32 WiFi AP");
    
    while(WiFiMulti.run() != WL_CONNECTED)
    {
        //Serial.println(".");
        delay(500);
    }

    //Serial.println();
    //Serial.println("Connected to ESP32 WiFi AP");
    //Serial.print("IP Address:");
    //Serial.println(WiFi.localIP());

    delay(500);
    //Serial.println("Now showing RSSI:");
}



void loop() {
    kalman_filter();
    //Serial.print("Actual RSSI:");
    Serial.println(Re);

    //Serial.print("\t Estimated RSSI:");
    //Serial.println(Re);
}