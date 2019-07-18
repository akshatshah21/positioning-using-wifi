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
double txPower=-56;
void calculate_mean_rssi()
{
    for(int i=0;i<100;i++){
        mean += WiFi.RSSI() ;
    }
    mean /= 100;
}

//kalman variables
double ARm, 	//Actual RSSI measurement, from WiFi.RSSI()
prev_Re= 0, 	//Previous RSSI estimate (Time update and Measurement Update)
prev_Pre = 1 ,  //Previous prior estimate (Time update only)
Rre,			//Current rough RSSI estimate
prev_Rre,		//Previous rough RSSI estimate
Pre,			//Current Prior RSSI estimate
prev_Pe = 1,	//Previous Error Covariance
k,				//Kalman Gain
R = 0.001,		//Some matrix
Re,				//Current RSSI estimate
Pe;				//

//Implementing Kalman Filter:


void kalman_filter()
{
	ARm = mean;
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


void calculate_distance()
{
    
    if(Re>=txPower)    //Less than 1m
    {

        distance = pow(10,((txPower-Re)/17));
        if(isnan(distance))
            Serial.print("NAN conditon! Less than 1m");

    }
    else                        //Greater than 1m
    {
        distance = pow(10,((txPower-Re)/25));
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
    kalman_filter();
    calculate_distance();
    Serial.print(Re);
    Serial.print("  :  ");
    Serial.println(distance);
}