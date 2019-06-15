#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <math.h>
#include <PubSubClient.h>


//broker details
const char* internet_AP_ssid = "leo";
const char* internet_AP_password = "leopassword";
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



double kalman_filter(double rssi)
{
	ARm = rssi;
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
double txPower = -55.2;
double calculate_distance(double rssi)
{
    double dist;
    dist = pow(10,((txPower-rssi)/16));
    return dist;
}

double normal_dist, distance_kalman;

void setup_internet_AP_wifi() {

    delay(100);
    // We start by connecting to a WiFi network
    
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(internet_AP_ssid);

    WiFi.begin(internet_AP_ssid, internet_AP_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Connected to");
    //Serial.println("IP address: ");
    Serial.println(internet_AP_ssid);
    
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
    WiFi.mode(WIFI_STA);

    setup_internet_AP_wifi(); //For confirmation that Wemos can connect to the AP providing internet
    client.setServer(broker, mqtt_port); //For confirmation of connection to MQTT broker
    Serial.println("Connected ");
    Serial.print("MQTT Server ");
    Serial.print(broker);
    Serial.print(":");
    Serial.println(String(mqtt_port));
    Serial.print("ESP8266 IP: ");
    Serial.println(WiFi.localIP());


 //client.setCallback(callback);


 // Serial.println("Modbus RTU Master Online");
}


double rssi,kalman_rssi, dist;
String ssid,pub_msg_str;
char * pub_dist,* pub_msg_char;
void loop(){

    delay(1000);
    Serial.println("APs found:");
    for(int i=0;i<WiFi.scanNetworks();i++)
    {
        Serial.print(WiFi.SSID(i));
        ssid = WiFi.SSID(i);
        Serial.print(":");
        Serial.println(WiFi.RSSI(i));
        rssi = WiFi.RSSI(i);
        kalman_rssi = kalman_filter(rssi);
        dist = calculate_distance(Re);
        dtostrf(dist,3,3, pub_dist);
        pub_msg_str = ssid + ":" + pub_dist;
        strcpy(pub_msg_char,pub_msg_str.c_str());
        client.publish("d1",pub_msg_char);
    }

    Serial.print("Still connected to:");
    Serial.println(WiFi.SSID());
    /*

    //Calculate distance from measured RSSI (from WiFi.RSSI())
    //get_mean();
    //normal_dist = calculate_distance(mean_rssi);
	//Call kalman_filter function
    //kalman_filter();
	//Calculate distance from RSSI output from Kalman filter
    //distance_kalman = calculate_distance(Re);
	//Print the two distances
    //Serial.print(distance_kalman);
    //Serial.print(" ");
    //Serial.println(normal_dist);
    //Serial.print(" ");
    //Serial.println(k,8); //Printing Kalman gain to 8dp
    //Serial.println(ARm);




    //double dist_1 = distance_kalman;        //distance
    //Serial.println("distance calculated is :");
    //Serial.println(dist_1);     //printing value of d

    //char distance_1[10], raw_dist[10];
    //dtostrf(dist_1,3,3, distance_1); //// convert float to char parameter= value ,width,precision,arr_to_store
    //client.publish("d3", distance_1);
    //Serial.println("Published");
    //dtostrf(normal_dist,3,3, raw_dist);
    //client.publish("d1",raw_dist) ;/// send char array

    if (!client.connected()){
        reconnect();
    }
    // delay(500); //Remember to comment
    client.loop();
    */
}
