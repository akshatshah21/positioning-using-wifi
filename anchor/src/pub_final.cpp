#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <math.h>
#include <PubSubClient.h>


//broker details
const char* ssid = "leo";
const char* password = "leopassword";
const char* brokerUser = "ocbshoyv";/// MQTT Broker USerName
const char* brokerPass = "u7RF9Xts1g1r"; //broker password
const char* broker = "postman.cloudmqtt.com";  //broker
int mqtt_port = 16557;   //will have to change

double mean_rssi = 0;
void get_mean()
{
    for(int i=0;i<100;i++)
        mean_rssi += WiFi.RSSI();
    mean_rssi /= 100;
}

/*
//esp WiFi details
#ifndef STA_SSID
#define STA_SSID "esp32"
#define STA_PWD "password"
#endif
//const char * ssid = STA_SSID;
//const char * pwd = STA_PWD;
*/

WiFiClient espClient;
PubSubClient client(espClient);

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
	ARm = WiFi.RSSI();
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
    /*
    if(Pe>=0.00001)
    {
        prev_Pe = Pe;
    }
    else
    {
        prev_Pe=1;
    }
    */
}
//Calculates distance from final mean RSSI value, using
//Log-distance path loss model
double txPower = -55.2;
double calculate_distance(double rssi)
{
    double dist;
    /*
    if(WiFi.RSSI()>=txPower)       //DISTANCE LESSER THAN 1m
    {
        dist = pow(10,((txPower-rssi)/20));
       if(isnan(dist)){
            //Serial.print("NAN condition!");
            return -1;
        }
        else
            return dist;
    }
    else        //DISTANCE GREATER THAN 1m
    {
    dist = pow(10,((txPower-rssi)/17));
    if(isnan(dist))  return -1;
        else
            return dist;
    }
    */
   dist = pow(10,((txPower-rssi)/16));
   return dist;
}

double normal_dist, distance_kalman;

void setup_wifi() {

    delay(100);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Connected to");
    //Serial.println("IP address: ");
    Serial.println(ssid);
}


void reconnect(){
    while(!client.connected()){
        Serial.println("connecting to");
        Serial.println(broker);
        if(client.connect(broker,brokerUser,brokerPass)){
            Serial.println("connected to");
            Serial.println(broker);
        } else{
            Serial.println("Trying to connect again");
            delay(200);
        }


    }
}
void setup(){
     Serial.begin(115200);


    setup_wifi();
    client.setServer(broker, mqtt_port);



 //client.setCallback(callback);

 Serial.println("Connected ");
 Serial.print("MQTT Server ");
 Serial.print(broker);
 Serial.print(":");
 Serial.println(String(mqtt_port));
 Serial.print("ESP8266 IP: ");
 Serial.println(WiFi.localIP());
 // Serial.println("Modbus RTU Master Online");
}



void loop(){
    //Calculate distance from measured RSSI (from WiFi.RSSI())
    get_mean();
    normal_dist = calculate_distance(mean_rssi);
	//Call kalman_filter function
    //kalman_filter();
	//Calculate distance from RSSI output from Kalman filter
    //distance_kalman = calculate_distance(Re);
	//Print the two distances
    //Serial.print(distance_kalman);
    //Serial.print(" ");
    Serial.println(normal_dist);
    //Serial.print(" ");
    //Serial.println(k,8); //Printing Kalman gain to 8dp
    //Serial.println(ARm);
    



    double dist_1 = distance_kalman;        //distance
    //Serial.println("distance calculated is :");
    //Serial.println(dist_1);     //printing value of d

    char distance_1[10], raw_dist[10];
    dtostrf(dist_1,3,3, distance_1); //// convert float to char parameter= value ,width,precision,arr_to_store
    //client.publish("d3", distance_1);
    //Serial.println("Published");
    dtostrf(normal_dist,3,3, raw_dist);
    client.publish("d1",raw_dist) ;/// send char array
    
    if (!client.connected()){
        reconnect();
    }
    // delay(500); //Remember to comment
    client.loop();

}
