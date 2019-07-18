#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <math.h>
#include <PubSubClient.h>


//broker details
const char* internet_AP_ssid = "leo";
const char* internet_AP_password = "leopassword";
const char* brokerUser = "ocbshoyv";/// MQTT Broker UserName
const char* brokerPass = "u7RF9Xts1g1r"; //broker password
const char* broker = "postman.cloudmqtt.com";  //broker
int mqtt_port = 16557;   //will have to change


int n; //Count of number of APs scanned
int i,j; //Counters for for loops
WiFiClient espClient;
PubSubClient client(espClient);

//kalman variables
class kalman{
    private:

    double ARm, 	//Actual RSSI measurement, from WiFi.RSSI()
    prev_Re= 0, 	//Previous RSSI estimate (Time update and Measurement Update)
    prev_Pre = 1 ,  //Previous prior estimate (Time update only)
    Rre,			//Current rough RSSI estimate
    prev_Rre,		//Previous rough RSSI estimate
    Pre,			//Current Prior RSSI estimate
    prev_Pe = 1,	//Previous Error Covariance
    k,	    			//Kalman Gain
    R = 0.001,		//Some matrix				//Current RSSI estimate
    Pe;				//
    public:
    String ssid;
    double Re;
    kalman() {;}
    kalman(String s)    {ssid  = s;}
    void kalman_filter(double );
};
//Implementing Kalman Filter:



void kalman :: kalman_filter(double rssi)
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
double txPower = -70;
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
        if(client.connect("a2",brokerUser,brokerPass)){
            Serial.println("connected to");
            Serial.println(broker);
        } else{
            Serial.println("Trying to connect again");
            delay(200);
        }

    }
}
kalman AP_arr[20];
char ssid_msg[100];

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
}


double rssi,kalman_rssi, dist;
String ssid,pub_msg_str;
char pub_dist[10], pub_msg_char[100];
void loop(){

    //delay(1000);
    Serial.print("APs found:");
    n = WiFi.scanNetworks();

    for(int i = 0;i<n;i++)
    {
        AP_arr[i] = kalman(WiFi.SSID(i));
        strcpy(ssid_msg,AP_arr[i].ssid.c_str());
        client.publish("ssid",ssid_msg);
        Serial.println("Sent ssid");
        delay(10);
    }

    Serial.println(n);
    for(i=0;i<n;i++)
    {

        ssid = WiFi.SSID(i);

        rssi = WiFi.RSSI(i);

        for(j=0;j<n;j++)
        {
            if(ssid==AP_arr[j].ssid)    break;
        }
        AP_arr[j].kalman_filter(rssi);

        kalman_rssi = AP_arr[j].Re;
        dist = calculate_distance(kalman_rssi);
        dtostrf(dist,3,3, pub_dist);
        pub_msg_str = ssid + ":" + pub_dist;
        strcpy(pub_msg_char,pub_msg_str.c_str());
        client.publish("a2",pub_msg_char); //Topics: a1,a2,a3 for the three anchors
        delay(10);
    }

    Serial.print("Still connected to:");
    Serial.println(WiFi.SSID());

    if (!client.connected()){
        reconnect();
    }
    // delay(500); //Remember to comment
    client.loop();

}
