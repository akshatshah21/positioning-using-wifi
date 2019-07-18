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

double txPower = -43;
double calculate_distance(double rssi)
{
    double dist;
    dist = pow(10,((txPower-rssi)/16));
    return dist;
}
//Implementing Kalman Filter:
double ARm, prev_Re= 0, prev_Pre = 1 ,Rre, prev_Rre, Pre, prev_Pe = 1, k, R = 0.5, Re, Pe;
double mean ;
void get_mean()
{
    double sum = 0;
    double dist;
    for(int i=0;i<50000;i++)
    {
        sum += WiFi.RSSI();
    }
    mean = sum / 50000;
}
void kalman_filter()
{

ARm = mean;
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
double mean_dist, kalman_dist;
void setup() 
{
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
    Serial.println("Connected to ESP32 WiFi AP");
    //Serial.print("IP Address:");
    //Serial.println(WiFi.localIP());

    delay(1500);
    //Serial.println("Now showing RSSI:");
}



void loop() {
    get_mean();
    kalman_filter();
    //Serial.print("Actual RSSI:");
    //Serial.print(ARm);
    //Serial.print(" ");
    Serial.print(mean);
    Serial.print(" ");
    //Serial.print("\t Estimated RSSI:");
    Serial.print(Re);
    mean_dist = calculate_distance(mean);
    kalman_dist = calculate_distance(Re);
    Serial.print(" ");
    Serial.print(k,8);
    Serial.print(mean_dist);
    Serial.print(" ");
    Serial.println(kalman_dist);
}