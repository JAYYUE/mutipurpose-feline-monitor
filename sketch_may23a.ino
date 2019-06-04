#include <FirebaseCloudMessaging.h>
#include <Firebase.h>
#include <FirebaseHttpClient.h>
#include <FirebaseArduino.h>
#include <FirebaseError.h>
#include <FirebaseObject.h>
#include <SoftwareSerial.h>


/* Controlling LED using Firebase console by CircuitDigest(www.circuitdigest.com) */
#include <ESP8266WiFi.h>                                                // esp8266 library
#include <FirebaseArduino.h>                                             // firebase library

#define FIREBASE_HOST "opal-97ef3.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "jnrrfKzTiyerk9nGAg0pOK0Dz36OTomlzdVsgP2e"                    // the secret key generated from firebase
#define WIFI_SSID "Sherlock_2.4G"                                          // input your home or public wifi name 
#define WIFI_PASSWORD "19971024081"                                    //password of wifi ssid
SoftwareSerial s(3,1);
String foodStatus = "";                                                     // led status received from firebase
int food = 16; //D0
int weight;
String cameraStatus="";
//int camera= 0;//D3

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  //Serial1.begin(9600);
  Serial.print("start");
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(food, OUTPUT);       
  //pinMode(camera, OUTPUT);  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
  Firebase.setString("FOOD_STATUS", "OFF"); 
  Firebase.setString("CAMERA_STATUS", "OFF"); 
  Firebase.setString("FOOD_WEIGHT", "0");//send initial string of led status
}

void loop() {
  if(Serial.available()>0){
    weight = Serial.parseInt();
    Serial.println("??");
    Firebase.setInt("FOOD_WEIGHT", weight);
  }
  foodStatus = Firebase.getString("FOOD_STATUS");  
  //cameraStatus=Firebase.getString("CAMERA_STATUS");
  //Serial.println(foodStatus);// get ld status input from firebase
  if (foodStatus == "ON"&&WiFi.status() == WL_CONNECTED) {                                                          // compare the input of led status received from firebase
    //Serial.println("food is being dispensed");                                                      // make bultin led ON
    digitalWrite(food, HIGH);
    delay(1000);
    digitalWrite(food, LOW);                                                         // make external led OFF
    Firebase.setString("FOOD_STATUS", "OFF");
    
    foodStatus='OFF';
    // make external led ON
  } 
  else if (foodStatus == "OFF"&&WiFi.status() == WL_CONNECTED) {                                                  // compare the input of led status received from firebase
    //Serial.println("stop dispensing");                                             // make bultin led OFF
    digitalWrite(food, LOW);                                                         // make external led OFF
  }
  cameraStatus=Firebase.getString("CAMERA_STATUS");
  if (cameraStatus == "ON"&&WiFi.status() == WL_CONNECTED) {                                                          // compare the input of led status received from firebase
    //Serial.println("camera engaged");                                                 // make bultin led ON
    Serial1.println(1);
    delay(1000);
                                                       // make external led OFF
    Firebase.setString("CAMERA_STATUS", "OFF");
    
    cameraStatus='OFF';
    // make external led ON
  } 
  else if (cameraStatus == "OFF"&&WiFi.status() == WL_CONNECTED) {                                                  // compare the input of led status received from firebase
    //Serial.println("no camera!!!");                                             // make bultin led OFF
                                                           // make external led OFF
  }
  else if(WiFi.status() != WL_CONNECTED){
    //Serial.println("fuck! wifi disconnected!");
    }
  else {
    //Serial.println("Wrong Credential! Please send ON/OFF");
  }
}
