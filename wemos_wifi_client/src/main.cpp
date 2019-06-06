#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <math.h>
// import processing.serial.*;

#ifndef STA_SSID
#define STA_SSID "esp32"
#define STA_PWD "password"
#endif
const char * ssid = STA_SSID;
const char * pwd = STA_PWD;

ESP8266WiFiMulti WiFiMulti;

double pp_prev_distance = 0;
double p_prev_distance = 0;
double prev_distance = 0;
double p_prev_speed = 0;
double prev_speed=0;
double speed=0,acceleration=0,mean_gradient_rssi=0,predicted_distance;

double prev_error = 0;
double distance = 0,distance_kalman=0,ratio,txPower = -45.5;//Hardcoded
int rssi[100],count=1;
double mean, prev_mean, final_mean, variance, sd;
int n;

//Measures RSSI values, stores in array and calculates mean
void getRSSI_and_mean()
{
    for(int i=0;i<100;i++)
    {
        rssi[i] = WiFi.RSSI();

        mean += rssi[i];
        delay(5);
    }
    mean /= 100;
}

//Implementing Kalman Filter:
double ARm, prev_Re= 2, prev_Pre = 1 ,Rre, prev_Rre, Pre, prev_Pe = 1, k, R = 0.001 , Re, Pe;

void kalman_filter()
{
ARm =final_mean;
Rre = prev_Re;
Pre = prev_Pe;
k = Pre/(Pre + R);
Re = Rre + k*(ARm-Rre);
Pe = (1-k)*Pre;
// Serial.println("RSSI estimated :");
// Serial.println(Re);
Serial.print(k);
prev_Re = Re;
prev_Pe = Pe;

}

//Calculates variance
void calculate_variance()
{
    variance = 0;
    sd = 0;

    for(int i=0;i<100;i++)
    {
        variance += ((rssi[i] - mean)*(rssi[i]-mean));
    }
    variance /= 99;
    sd = sqrt(variance);
}

//Calculates final mean, which is the mean of the values in the central part of
//the Gaussian distribution of RSSI values,
//bounded by (mean - variance) and (mean + variance)
void calculate_final_mean()
{
    final_mean = 0;
    n = 0;
    for(int i=0;i<100;i++)
    {
        if(rssi[i]<=mean)
        {
            if(rssi[i]>(mean-sd))
            {
                final_mean+=rssi[i];
                n++;
            }
        }
        else
        {
            if(rssi[i]<(mean+sd))
            {
                final_mean+=rssi[i];
                n++;
            }
        }
    }
    if(n!=0)
    {
        final_mean = final_mean/n;
        prev_mean = final_mean;
    }
    else
    {
        final_mean = prev_mean;
    }
    mean_gradient_rssi+=final_mean;
}

//Calculates distance from final mean RSSI value, using Path Loss Model
//Or Log-distance model
double calculate_distance(double final_mean)
{
    double dist;
    if(WiFi.RSSI()>=txPower)
    {

        dist = pow(10,((txPower-final_mean)/17));
        if(isnan(dist)){
            //Serial.print("NAN condition!");
            return -1;
        }
        else
            return dist;

    }
    else
    {
        dist = pow(10,((txPower-final_mean)/25));
        if(isnan(dist))  return -1;
        else
            return dist;
    }
}

void gradient_filter()
{
p_prev_speed = p_prev_distance - pp_prev_distance;
prev_speed = prev_distance - p_prev_distance;
speed = distance - prev_distance;
acceleration = prev_speed - p_prev_speed;


//if(speed>(prev_speed+acceleration))
//{  
    predicted_distance=(calculate_distance(mean_gradient_rssi/count))+(prev_speed+acceleration)/count+(prev_error*0.1);
//}
/*else
{
    predicted_distance=distance;
}*/
//Serial.print("Predicted distance :");
Serial.print(" ");
Serial.print(predicted_distance);

prev_error = distance - predicted_distance;
pp_prev_distance = p_prev_distance;
p_prev_distance = prev_distance;
prev_distance = distance;
p_prev_speed=prev_speed;
prev_speed=speed;
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
    //Serial.println("Now showing RSSI and distance values");
}

void loop() 
{
    mean = 0;
    //Serial.println();
    getRSSI_and_mean();
    calculate_variance();
    calculate_final_mean();
    kalman_filter();
    distance = calculate_distance(final_mean);
    distance_kalman = calculate_distance(Re);
    
    gradient_filter();

    //Serial.print("Distance:");
    Serial.print(" ");
    Serial.print(distance);
    //Serial.print("\tEstimated dist:");
    Serial.print(" ");
    Serial.println(distance_kalman);
    //Serial.print("RSSI:");
    //Serial.println(final_mean);
    //Serial.print("\tEst. RSSI:");
    //Serial.println(Re);
    count++;    
    mean = 0;
    delay(5);
}