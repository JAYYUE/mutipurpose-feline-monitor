//motor
#include <Servo.h>
int servoPin1 = 7;
Servo Servo1;
int wifistatus;
int wifipin = 9;
int disdelay;
int weight;

// camera
int cameraWifi = 8;
int cameraStatus;

//load cell
#include "HX711.h"
#define calibration_factor 1941.1 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT  3
#define CLK  2
HX711 scale;
int RunCamControl = 10;

void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);

  disdelay = 0;
  wifistatus = 0;
  cameraStatus = 0;

  pinMode(wifipin, INPUT);
  pinMode(cameraWifi, INPUT);

  Serial.println(cameraStatus);
  Servo1.attach(servoPin1);

  attachInterrupt(digitalPinToInterrupt(wifipin), dispenser, RISING);

  Servo1.write(1);
  delay(2000);
  Servo1.write(180);

  //load cell
  Serial.println("HX711 scale demo");
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  Serial.println("Readings:");

  //CAMERA
  digitalWrite(RunCamControl, LOW); //insure we start out with the control line low
  Serial.println("Setup complete");
}

void loop() {

  if (disdelay == 1) {
    dispenserDelay();
  }
  wifistatus = digitalRead(wifipin);
  cameraStatus = Serial1.parseInt();
  Serial.println("**");
  Serial.println(cameraStatus);
  Serial.println("**");
  if (cameraStatus == 1) {
    Serial.println("ps");
    PushShutter();
    delay(10000);
    PushShutter();
    cameraStatus = 0;
  }
  //load cell
  Serial.print("Reading: ");
  weight = abs(scale.get_units());
  Serial.print(weight, 1); //scale.get_units() returns a float
  Serial.println(" g"); //You can change this to kg but you'll need to refactor the calibration_factor
  if (Serial2.available() == 0) {
    //Serial.println("sent");
    Serial1.print(weight);
    Serial1.print("p");
  }
  delay(1000);

}


void dispenser() {
  Serial.println
    Servo1.write(1);
  disdelay = 1;
}

void dispenserDelay() {
  unsigned long int time1 = millis();
  unsigned long int time2;
  unsigned long int gap;
  disdelay = 0;
  while (1) {
    time2 = millis();
    gap = time2 - time1;
    if (gap >= 3000)
      break;
  }
  Servo1.write(180);
}

void PushShutter() {
  OnePulse();
  OnePulse();
  delay(2000); //give time for camera to process command
}

void OnePulse() { // |--|__ assumes output starts out low
  digitalWrite(RunCamControl, HIGH); //80 ms wide pulse
  delay(80);
  digitalWrite(RunCamControl, LOW);
  delay(80);//80 ms of low
}
