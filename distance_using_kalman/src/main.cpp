#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <math.h>

#ifndef STA_SSID
#define STA_SSID "esp32"
#define STA_PWD "password"
#endif
const char * ssid = STA_SSID;
const char * pwd = STA_PWD;

ESP8266WiFiMulti WiFiMulti;
//Implementing Kalman Filter:

double ARm, 	//Actual RSSI measurement, from WiFi.RSSI()
prev_Re= 2, 	//Previous RSSI estimate (Time update and Measurement Update)
prev_Pre = 1 ,  //Previous prior estimate (Time update only)
Rre,			//Current RSSI rough RSSI estimate
prev_Rre,		//Previous RSSI rough estimate
Pre,			//Current Prior RSSI estimate
prev_Pe = 1,	//Previous
k,				//Kalman Gain
R = 0.001,		//Some matrix
Re,				//Current RSSI estimate
Pe;				//

void kalman_filter()
{
	ARm =WiFi.RSSI();
	Rre	= prev_Re;
	Pre = prev_Pe;
	k = Pre/(Pre + R);
	Re = Rre + k*(ARm-Rre);
	Pe = (1-k)*Pre;
	// Serial.println("RSSI estimated :");
	// Serial.println(Re);
	//Serial.print(k);
	//Serial.print(" ");
	prev_Re = Re;
	prev_Pe = Pe;
}
//Calculates distance from final mean RSSI value, using
//Log-distance path loss model
double txPower = -45.5;
double calculate_distance(double rssi)
{
    double dist;
    if(WiFi.RSSI()>=txPower)
    {

        dist = pow(10,((txPower-rssi)/17));
        if(isnan(dist)){
            //Serial.print("NAN condition!");
            return -1;
        }
        else
            return dist;

    }
    else
    {
        dist = pow(10,((txPower-rssi)/25));
        if(isnan(dist))  return -1;
        else
            return dist;
    }
}

double normal_dist, distance_kalman;

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
    //Serial.println("Now showing RSSI and distance values");
}

void loop()
{
    //Calculate distance from measured RSSI (from WiFi.RSSI())
    normal_dist = calculate_distance(WiFi.RSSI());
	//Call kalman_filter function
    kalman_filter();
	//Calculate distance from RSSI output from Kalman filter
    distance_kalman = calculate_distance(Re);
	//Print the two distances
    Serial.print(distance_kalman);
    Serial.print(" ");
    Serial.println(normal_dist);

    delay(5);
}
