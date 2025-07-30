#define REMOTEXY_MODE  WIFI_CLOUD 
#include <ESP8266WiFi.h> 
// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "your wifi ssid" 
#define REMOTEXY_WIFI_PASSWORD "your wifi password" 
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com" 
#define REMOTEXY_CLOUD_PORT 6376 
#define REMOTEXY_CLOUD_TOKEN "78fe5013673a0156216d0194be437a40" 
#define REMOTEXY_ACCESS_PASSWORD "enter wifi password here" 
#include <ESP8266WiFi.h> 
#include <LiquidCrystal_I2C.h> 
#include <DHT.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2); 
#define FLOW_RATE_LPM 2.0 
// Liters per minute (your pump spec or measured) 
# define RELAY_PIN_1 D3 
# define Fan D4 
DHT dht(D5, DHT11); 
#define soil A0 
#include <RemoteXY.h> 
// RemoteXY GUI configuration 
#pragma pack(push, 1) 
21 | P a g e  
uint8_t RemoteXY_CONF[] =  // 379 bytes 
{ 255,1,0,18,0,116,1,19,0,0,0,105,111,116,95,99,108,111,117,100, 
95,112,108,97,110,116,95,99,97,114,101,0,31,2,106,200,200,84,1,1, 
18,0,4,15,177,79,17,170,32,15,44,128,2,26,70,75,100,12,12,131, 
37,10,10,16,1,132,0,67,68,74,31,10,63,29,48,11,78,2,26,2, 
67,68,59,30,9,64,12,44,11,78,2,26,2,67,68,88,32,9,112,24, 
75,4,78,2,26,2,129,6,7,97,9,56,4,13,5,64,6,73,79,84, 
32,69,110,97,98,108,101,100,32,80,108,97,110,116,32,67,97,114,101,32, 
58,0,129,32,19,43,8,50,14,13,5,64,15,77,111,110,105,116,111,114, 
105,110,103,32,32,0,129,10,49,3,12,46,21,13,5,64,17,0,129,7, 
76,59,10,50,19,13,5,64,8,84,101,109,112,101,114,97,117,114,101,32, 
58,0,129,8,60,45,10,48,27,13,5,64,8,77,111,105,115,116,117,114, 
101,32,58,0,129,8,90,41,9,50,35,13,5,64,8,72,117,109,105,100, 
105,116,121,32,58,0,129,48,27,7,10,92,16,13,5,64,15,38,0,129, 
25,36,55,9,82,21,13,5,64,15,77,97,105,110,116,97,105,110,97,110, 
99,101,0,129,8,105,56,9,43,47,13,5,64,8,80,117,109,112,32,115, 
116,97,116,117,115,32,58,0,70,75,117,12,12,166,56,8,8,16,35,146, 
0,129,8,119,47,9,43,56,13,5,64,8,70,97,110,32,115,116,97,116, 
117,115,32,58,0,129,9,148,36,9,88,75,13,5,64,8,83,101,116,112, 
111,105,110,116,58,0,67,62,149,39,10,104,61,75,4,78,2,26,2 }; 
// this structure defines all the variables and events of your control interface 
struct { 
// input variables 
int8_t threshold; // from 0 to 100 
// output variables 
22 | P a g e  
uint8_t pump; // from 0 to 1 
float temperature; 
float moisture; 
float humidity; 
uint8_t fan; // from 0 to 1 
float set_point; 
// other variable 
uint8_t connect_flag; // =1 if wire connected, else =0 
} RemoteXY; 
#pragma pack(pop) 
 
///////////////////////////////////////////// 
// END RemoteXY include // 
///////////////////////////////////////////// 
void setup() 
{ 
RemoteXY_Init (); 
Serial.begin(9600); 
 
pinMode(soil,INPUT); 
pinMode(RELAY_PIN_1, OUTPUT); 
pinMode(Fan,OUTPUT); 
digitalWrite(Fan, LOW); 
digitalWrite(RELAY_PIN_1, LOW); 
dht.begin(); 
// TODO you setup code 
 
 
} 
void loop() 
{ 
RemoteXY_Handler (); 
float set_point; 
float temperature = dht.readTemperature(); 
float humidity = dht.readHumidity(); 
 
if(temperature >30) 
{ 
digitalWrite(Fan, HIGH); 
Serial.print(temperature); 
RemoteXY.fan= 1; 
} 
else 
{ 
digitalWrite(Fan,LOW); 
Serial.print("t =" ); 
Serial.print(temperature); 
RemoteXY.fan = 0; 
} 

int sensor_analog = analogRead(A0); 
float moisture = ( 100 - ( (sensor_analog/1023.00) * 100 ) ); 
int threshold = RemoteXY.threshold; 
set_point=threshold; 
if(moisture <= threshold ) 
{ 
 
 
digitalWrite(RELAY_PIN_1, HIGH); 
 
 
RemoteXY.pump = 1; 
} 
else if(moisture >= threshold) 
{ 
 
 
digitalWrite(RELAY_PIN_1, LOW); 
 
 
RemoteXY.pump = 0; 
} 
 
 
RemoteXY.temperature = temperature; 
RemoteXY.humidity = humidity; 
RemoteXY.moisture = moisture; 
RemoteXY.set_point = set_point; 

// use the RemoteXY structure for data transfer 
// do not call delay(), use instead RemoteXY_delay() 
}
