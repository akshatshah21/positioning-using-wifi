#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <math.h>


#define broker "postman.cloudmqtt.com"
#define port 16557
#define mqttUsername "oc"
#define mqttPwd ""
#define mqttClientID "Anchor1"
const char *INTERNET_AP_SSID = "leo";
const char *INTERNET_AP_PWD = "leopassword";
double mean, distance;
double txPower=-50;
void calculate_mean_rssi()
{
    for(int i=0;i<100;i++){
        mean += WiFi.RSSI() ;
    }
    mean /= 100;
}

void calculate_distance()
{
    
    if(mean>=txPower)    //Less than 1m
    {

        distance = pow(10,((txPower-mean)/17));
        if(isnan(distance))
            Serial.print("NAN conditon! Less than 1m");

    }
    else                        //Greater than 1m
    {
        distance = pow(10,((txPower-mean)/25));
        if(isnan(distance))
            Serial.println("NAN condition, Greater than 1m");
    }
}

ESP8266WiFiSTAClass WiFi_STA;

void setup()
{
    Serial.begin(115200);
    WiFi.begin(INTERNET_AP_SSID,INTERNET_AP_PWD);
    while(WiFi.status()!=WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

}

void loop()
{
    mean = 0;
    calculate_mean_rssi();
    calculate_distance();
    Serial.print(mean);
    Serial.print("  :  ");
    Serial.println(distance);
}