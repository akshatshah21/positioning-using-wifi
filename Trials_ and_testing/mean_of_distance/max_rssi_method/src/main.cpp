#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <math.h>
#include<stdio.h>
#ifndef STA_SSID
#define STA_SSID "esp8266"
#define STA_PWD "password"
#endif
const char * ssid = STA_SSID;
const char * pwd = STA_PWD;

ESP8266WiFiMulti WiFiMulti;

//Implementing Kalman Filter:
/*
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
Pe;       //sum of m maximum value for maximum filter
*/
double mean,sum=0;
// A utility function to swap two elements 
void swap(double* a, double* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 
  
/* This function takes last element as pivot, places 
   the pivot element at its correct position in sorted 
    array, and places all smaller (smaller than pivot) 
   to left of pivot and all greater elements to right 
   of pivot */
int partition (double arr[], int low, int high) 
{ 
    int pivot = arr[high];    // pivot 
    int i = (low - 1);  // Index of smaller element 
  
    for (int j = low; j <= high- 1; j++) 
    { 
        // If current element is smaller than or 
        // equal to pivot 
        if (arr[j] <= pivot) 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 
  
/* The main function that implements QuickSort 
 arr[] --> Array to be sorted, 
  low  --> Starting index, 
  high  --> Ending index */
void quickSort(double arr[], int low, int high) 
{ 
    if (low < high) 
    { 
        /* pi is partitioning index, arr[p] is now 
           at right place */
        int pi = partition(arr, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
} 

double max_RSSI;
double arr[100];
double maximum_filter()
{
    
    int m = 25;
    for(int i=0;i<100;i++)  //array of 100 RSSI value 
    {
        arr[i] = WiFi.RSSI();
    }
    quickSort(arr,0,99);    //sort in ascending order
    for(int i=100-m;i<100;i++)
    {
        sum = sum + arr[i];
    }
    mean = sum/m; //mean of m maximum value
    sum = 0;  
    return mean;  // returning mean of RSSI

}
/*
void kalman_filter()
{
	ARm = max_RSSI;
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
//}

//Calculates distance from final mean RSSI value, using
//Log-distance path loss model
double txPower = -50;
double calculate_distance(double rssi)
{
    double dist;
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
    dist = pow(10,((txPower-rssi)/25));
    if(isnan(dist))  return -1;
        else
            return dist;
    }
}

double normal_dist, distance_kalman,max_dist;

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
    max_RSSI = maximum_filter();
    max_dist = calculate_distance(max_RSSI);
    //Calculate distance from measured RSSI (from WiFi.RSSI())
    //normal_dist = calculate_distance(WiFi.RSSI());
	//Call kalman_filter function
    //kalman_filter();
	//Calculate distance from RSSI output from Kalman filter
    //distance_kalman = calculate_distance(Re);
	//Print the two distances
    //Serial.print(normal_dist);
    //Serial.print(" ");
    //Serial.print(distance_kalman);
    //Serial.print(" ");
    //Serial.print(max_dist);
    //Serial.print(" ");
    //Serial.println(k,8); //Printing Kalman gain to 8dp
    //Serial.println(mean);
    Serial.println("**********");
    for(int i=0;i<100;i++)
    {

    }
    delay(1);
}
