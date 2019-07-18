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
const char* clientID = "Anchor3";
int mqtt_port = 16557;   //will have to change


double mean_R;
double txPower=-35;
WiFiClient espClient;
PubSubClient client(espClient);

class AP{
    private:

        //kalman variables for distance
        double ARm_D,
        prev_Re_D = 1,
        prev_Pre_D = 1 ,
        Rre_D,
        prev_Rre_D,
        Pre_D,
        prev_Pe_D = 1,
        k_D,
        R_D = 0.001,
        Pe_D;

        //kalman variables for RSSI
        double ARm_R,
        prev_Re_R = 0,
        prev_Pre_R = 1 ,
        Rre_R,
        prev_Rre_R,
        Pre_R,
        prev_Pe_R = 1,
        k_R,
        R_R = 0.001,
        Pe_R;


    public:
        String ssid;
        double Re_R,Re_D,distance,mean_distance;
        AP() {;}
        AP(String s)    {ssid  = s;}
        void kalman_filter_R();
        void kalman_filter_D();
        void reset_kalman_R();
        void reset_kalman_D();
        void calculate_distance();
        void calculate_mean_rssi(int i);
};


void calculate_mean_rssi(int i)
{

    for(int i=0;i<10;i++){
        mean_R += WiFi.RSSI(i) ;
    }
    mean_R /= 10;
}


void AP :: reset_kalman_D()
{
    ARm_D = 0;
    prev_Re_D = 0;
    prev_Pre_D = 1;
    prev_Pe_D = 1;
    R_D = 0.001;
}


//Implementing Kalman Filter for distance:
void AP :: kalman_filter_D()
{
	ARm_D = mean_distance;
	Rre_D	= prev_Re_D;
	Pre_D = prev_Pe_D;
	k_D = Pre_D/(Pre_D + R_D);
	Re_D = Rre_D + k_D*(ARm_D-Rre_D);
	Pe_D = (1-k_D)*Pre_D;
	prev_Re_D = Re_D;
	prev_Pe_D = Pe_D;

}


void AP :: reset_kalman_R()
{
    ARm_R = 0;
    prev_Re_R = 0;
    prev_Pre_R = 1;
    prev_Pe_R = 1;
    R_R = 0.001;
}

//Implementing Kalman Filter for RSSI:


void AP ::kalman_filter_R()
{
	ARm_R = mean_R;
	Rre_R	= prev_Re_R;
	Pre_R = prev_Pe_R;
	k_R = Pre_R/(Pre_R + R_R);
	Re_R = Rre_R + k_R*(ARm_R-Rre_R);
	Pe_R = (1-k_R)*Pre_R;
	prev_Re_R = Re_R;
	prev_Pe_R = Pe_R;

}


void AP :: calculate_distance()
{

    if(Re_R>=txPower)    //Less than 1m
    {

        distance = pow(10,((txPower-Re_R)/17));
        if(isnan(distance))
            Serial.print("NAN conditon! Less than 1m");

    }
    else                        //Greater than 1m
    {
        distance = pow(10,((txPower-Re_R)/25));
        if(isnan(distance))
            Serial.println("NAN condition, Greater than 1m");
    }
}

ESP8266WiFiSTAClass WiFi_STA;

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
        if(client.connect(clientID,brokerUser,brokerPass)){
            Serial.println("connected to");
            Serial.println(broker);
        } else{
            Serial.println("Trying to connect again");
            delay(200);
        }

    }
}

long int loop_count = 0;
AP AP_arr[20];

String ssid,pub_msg_str;
char pub_dist[10], pub_msg_char[100],ssid_msg[100];
int n,i,j;
void setup()
{
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

void loop()
{
    Serial.print("APs found:");
    n = WiFi.scanNetworks();
    Serial.println(n);
    for(int i = 0;i<n;i++)
    {
        AP_arr[i] = AP(WiFi.SSID(i));
        strcpy(ssid_msg,AP_arr[i].ssid.c_str());
        client.publish("ssid",ssid_msg);
        Serial.println("Sent ssid");
        delay(10);
    }

    for(i=0;i<n;i++) // i is index for in built scanned AP objects
    {

        ssid = WiFi.SSID(i);

        //rssi = WiFi.RSSI(i);



        for(j=0;j<n;j++)   //j is index number for array of AP objects
        {
            if(ssid==AP_arr[j].ssid)    break;
        }
        for(int l=0;l<10;l++)
        {
            AP_arr[j].mean_distance=0;
            for(int k=0;k<10;k++) //k for mean
            {
                calculate_mean_rssi(i);
                AP_arr[j].kalman_filter_R();
                AP_arr[j].calculate_distance();
                AP_arr[j].mean_distance += AP_arr[j].distance;
            }
            AP_arr[j].mean_distance /= 10;
            AP_arr[j].kalman_filter_D();
        }


        dtostrf(AP_arr[j].Re_D,3,3, pub_dist);
        pub_msg_str = ssid + ":" + pub_dist;
        strcpy(pub_msg_char,pub_msg_str.c_str());
        client.publish("a3",pub_msg_char); //Topics: a1,a2,a3 for the three anchors

        delay(10);
    }
    loop_count++;
    if(loop_count %1000 == 0)
    {

        Serial.println("**************************Resetting\n************************");
        for(int a=0;a<20;a++)
        {
            AP_arr[a].reset_kalman_R();
            AP_arr[a].reset_kalman_D();
            AP_arr[a].mean_distance = 0;
        }
    }


    
    Serial.print("Still connected to:");
    Serial.println(WiFi.SSID());

    if (!client.connected()){
        reconnect();
    }
    // delay(500); //Remember to comment
    client.loop();

}
