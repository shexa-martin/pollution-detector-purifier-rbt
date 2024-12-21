// Bluetooth and motor driving declarations starts
#include<SoftwareSerial.h>
#include "TinyGPS++.h"
SoftwareSerial mySerial(0, 1); //rx|TX
SoftwareSerial gpsSerialCommunication(51, 50);
TinyGPSPlus gps;
int command;
#define MQ7_PIN A0
#define MQ135_PIN A1
int suckingFan = 47;
/***************************************************
  the ena and enb Pins of the L298N motor driver
  are connected with the Arduino’s Pin Number 5
  and Pin number 6
****************************************************/
int ena = 5;
int enb = 6;
/***************************************************
  the ena and enb Pins of the L298N motor driver
  are connected with the Arduino’s Pin Number 5
  and Pin number 6
****************************************************/


/********************************************************
   The In1, In2, In3 and In4 Pins of the L298N
   Motor driver are connected with Pins 8, 9, 10,
   and 11 of the Arduino.
 * ******************************************************/
int in1 = 8;
int in2 = 9;
int in3 = 10;
int in4 = 11;
/********************************************************
   The In1, In2, In3 and In4 Pins of the L298N
   Motor driver are connected with Pins 8, 9, 10,
   and 11 of the Arduino.
 * ******************************************************/
// Bluetooth and motor driving declarations ends
const int alarm = 52;
// defines pins numbers
const int rearLeftTrig = 12;
const int rearLeftEcho = 13;
const int rearMidTrig = 2;
const int rearMidEcho = 3;
const int rearRightTrig = 4;
const int rearRightEcho = 7;
// defines pins numbers

const int fwdLeftTrig = 26;
const int fwdLeftEcho = 27;
const int fwdMidTrig = 24;
const int fwdMidEcho = 25;
const int fwdRightTrig = 22;
const int fwdRightEcho = 23;

// defines variables
long fwdRightDuration;
long fwdMidDuration;
long fwdLeftDuration;
long rearRightDuration;
long rearMidDuration;
long rearLeftDuration;

int fwdRightDistance;
int fwdMidDistance;
int fwdLeftDistance;
int rearRightDistance;
int rearMidDistance;
int rearLeftDistance;

// Servo motor controlled by ultrasonics declarations Ends
void setup() {

  gpsSerialCommunication.begin(9600);
  
  pinMode(MQ135_PIN, INPUT);
  pinMode(MQ7_PIN, INPUT);
  pinMode(suckingFan, OUTPUT);

  // Bluetooth motor driving setup starts
  mySerial.begin(9600);
  Serial.println("You are connected via Bluetooth");
  //Arduino sending OUTPUT to L298N Motor driver ENA
  pinMode(ena, OUTPUT);
  //Arduino sending OUTPUT to L298N Motor driver ENB
  pinMode(enb, OUTPUT);


  //Arduino sending OUTPUT to L298N Motor driver IN1
  pinMode(in1, OUTPUT);
  //Arduino sending OUTPUT to L298N Motor driver IN2
  pinMode(in2, OUTPUT);
  //Arduino sending OUTPUT to L298N Motor driver IN3
  pinMode(in3, OUTPUT);
  //Arduino sending OUTPUT to L298N Motor driver IN4
  pinMode(in4, OUTPUT);

  /******************
     255 means that the motors are set at
     their maximum speed.
  */
  analogWrite(ena, 122);
  analogWrite(enb, 122);
  delay(1000);
  // Bluetooth motor driving setup ends
  Serial.begin(9600);

  pinMode(alarm, OUTPUT);

  pinMode(fwdLeftTrig, OUTPUT); // Sets the trigPin as an Output
  pinMode(fwdLeftEcho, INPUT); // Sets the echoPin as an Input

  pinMode(fwdMidTrig, OUTPUT); // Sets the trigPin as an Output
  pinMode(fwdMidEcho, INPUT); // Sets the echoPin as an Input

  pinMode(fwdRightTrig, OUTPUT); // Sets the trigPin as an Output
  pinMode(fwdRightEcho, INPUT); // Sets the echoPin as an Input

  pinMode(rearLeftTrig, OUTPUT); // Sets the trigPin as an Output
  pinMode(rearLeftEcho, INPUT); // Sets the echoPin as an Input

  pinMode(rearMidTrig, OUTPUT); // Sets the trigPin as an Output
  pinMode(rearMidEcho, INPUT); // Sets the echoPin as an Input

  pinMode(rearRightTrig, OUTPUT); // Sets the trigPin as an Output
  pinMode(rearRightEcho, INPUT); // Sets the echoPin as an Input
}

void loop() {

  while(gpsSerialCommunication.available()) {
    gps.encode(gpsSerialCommunication.read());
  }
  
  int MQ135_gas_value = analogRead(MQ135_PIN);
  int MQ7_gas_value = analogRead(MQ7_PIN);
  Serial.print("Methane: ");
  Serial.println(MQ135_gas_value);
  Serial.print("CO: ");
  Serial.println(MQ7_gas_value);
  ////  Testing ultrasonic Sensors
  //  if(myFwdLeftDistance()<=10){
  //      Serial.println("Object detected on the left");
  //      }
  //      delay(300);
  //    if(myFwdMidDistance()<=10){
  //      Serial.println("Object detected on the front");
  //      }
  //      delay(300);
  //    if(myFwdRightDistance()<=10){
  //      Serial.println("Object detected on the right");
  //      }
  //      delay(300);
  //      if(myRearLeftDistance()<=10){
  //      Serial.println("Object detected on the left rear");
  //      }
  //      delay(300);
  //    if(myRearMidDistance()<=10){
  //      Serial.println("Object detected on the front rear");
  //      }
  //      delay(300);
  //    if(myRearRightDistance()<=10){
  //      Serial.println("Object detected on the right rear");
  //      }
  //      delay(300);
  //Testing ultrasonic sensors ends here
  if (MQ135_gas_value > 230 || MQ7_gas_value > 30) {
  raisePollutionAlarm();
  STOP();
  digitalWrite(suckingFan, HIGH);
  if (gps.location.isUpdated()) {
    Serial.print("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.print("Latitude:");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude:");
    Serial.println(gps.location.lng(), 6);

    Serial.print("Altitude Feet:");
    Serial.println(gps.altitude.feet());
    Serial.print("");

    Serial.print("Date: ");
    if (gps.date.isValid()) {
      if (gps.time.hour() < 10) Serial.print(F("0"));
      Serial.print(gps.time.hour()+2);
      Serial.print(":");
      if (gps.time.minute() < 10) Serial.print(F("0"));
      Serial.print(gps.time.minute());
      Serial.print(":");
      if (gps.time.second() < 10) Serial.print(F("0"));
      Serial.print(gps.time.second());
      Serial.print(".GMT");
    }
    Serial.println("");
    delay(2000);
  }
  delay(20000);
  digitalWrite(suckingFan, LOW);
  delay(10);
  } else {
    FORWARD();
    delay(200);
    STOP();
  }
}
///////Motor motion functions starts
void STOP() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void FORWARD() {
  //if there are no objects in front go forward
  if (myFwdLeftDistance() > 10 && myFwdMidDistance() > 10 && myFwdRightDistance() > 10 && myRearLeftDistance() > 10 && myRearMidDistance() > 10 && myRearRightDistance() > 10) {
    forward();
  }
  //if there is an object on the front left
  else if (myFwdLeftDistance() <= 10 && myFwdMidDistance() > 10 && myFwdRightDistance() > 10 && myRearLeftDistance() > 10 && myRearMidDistance() > 10 && myRearRightDistance() > 10) {
    //reverse for few seconds,turn right and move forward
    REVERSE();
    delay(500);
    STOP();
    RIGHT();
    delay(1000);
    STOP();
    FORWARD();
    STOP();
  }
  //when there exist an object in front right while clear rear
  else if ((myFwdMidDistance() <= 10 || myFwdRightDistance() <= 10) && (myRearLeftDistance() > 10 && myRearMidDistance() > 10 && myRearRightDistance() > 10)) {
    REVERSE();
    delay(500);
    STOP();
    LEFT();
    delay(1000);
    FORWARD();
    STOP();
  }
  ////      When there is an object on the right
  //     else if(myFwdLeftDistance()>10 && myFwdMidDistance()>10 && myFwdRightDistance()<=10 && myRearLeftDistance()>10 && myRearMidDistance()>10 && myRearRightDistance()>10){
  //      //reverse for few seconds,turn left and move forward
  //      REVERSE();
  //      delay(300);
  //      STOP();
  //      LEFT();
  //      delay(300);
  //      STOP();
  //      FORWARD();
  //      STOP();
  //      }
  //if there are objects in Front and Rear, raise an alarm
  else if ((myFwdLeftDistance() < 10 || myFwdMidDistance() < 10 || myFwdRightDistance() < 10) && (myRearLeftDistance() < 10 || myRearMidDistance() < 10 || myRearRightDistance() < 10)) {
    //STOP & Ring the Bell
    STOP();
    raiseObstacleAlarm();
  } else {

  }
}
void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void RIGHT() {
  //    if(myFwdRightDistance>10 && myFwdMidDistance>10){
  right();
  //      }else{
  //        STOP();
  //        raiseObstacleAlarm();
  //      }
}

void right() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void LEFT() {
  //  if(myFwdLeftDistance>10 && myFwdMidDistance>10){
  left();
  //      }else{
  //        STOP();
  //        raiseObstacleAlarm();
  //      }
}
void left() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void REVERSE() {
  //  if(myRearRightDistance>10 && myRearMidDistance>10 && myRearLeftDistance>10){
  reverse();
  //      }else{
  //        STOP();
  //        raiseObstacleAlarm();
  //      }
}
void reverse() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void raiseObstacleAlarm() {
  digitalWrite(alarm, HIGH);
  delay(200);
  digitalWrite(alarm, LOW);
  delay(100);
  digitalWrite(alarm, HIGH);
  delay(200);
  digitalWrite(alarm, LOW);
  delay(100);
}
void raisePollutionAlarm() {
  digitalWrite(alarm, HIGH);
  delay(100);
  digitalWrite(alarm, LOW);
  delay(50);
  digitalWrite(alarm, HIGH);
  delay(100);
  digitalWrite(alarm, LOW);
  delay(50);
  digitalWrite(alarm, HIGH);
  delay(100);
  digitalWrite(alarm, LOW);
  delay(50);
  digitalWrite(alarm, HIGH);
  delay(100);
  digitalWrite(alarm, LOW);
  delay(50);
}
////////////////motor motion functions ends

int myFwdLeftDistance() {
  // Clears the trigPin
  digitalWrite(fwdLeftTrig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(fwdLeftTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(fwdLeftTrig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  fwdLeftDuration = pulseIn(fwdLeftEcho, HIGH);
  // Calculating the distance
  fwdLeftDistance  = fwdLeftDuration  * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  Serial.println(fwdLeftDistance);
  return fwdLeftDistance;
}

int myFwdMidDistance() {
  // Clears the trigPin
  digitalWrite(fwdMidTrig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(fwdMidTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(fwdMidTrig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  fwdMidDuration = pulseIn(fwdMidEcho, HIGH);
  // Calculating the distance
  fwdMidDistance  = fwdMidDuration  * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  Serial.println(fwdMidDistance);
  return fwdMidDistance;
}

int myFwdRightDistance() {
  // Clears the trigPin
  digitalWrite(fwdRightTrig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(fwdRightTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(fwdRightTrig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  fwdRightDuration = pulseIn(fwdRightEcho, HIGH);
  // Calculating the distance
  fwdRightDistance  = fwdRightDuration  * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  Serial.println(fwdRightDistance);
  return fwdRightDistance;
}

////////////////
//Rear

int myRearLeftDistance() {
  // Clears the trigPin
  digitalWrite(rearLeftTrig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(rearLeftTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(rearLeftTrig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  rearLeftDuration = pulseIn(rearLeftEcho, HIGH);
  // Calculating the distance
  rearLeftDistance  = rearLeftDuration  * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  Serial.println(rearLeftDistance);
  return rearLeftDistance;
}

int myRearMidDistance() {
  // Clears the trigPin
  digitalWrite(rearMidTrig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(rearMidTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(rearMidTrig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  rearMidDuration = pulseIn(rearMidEcho, HIGH);
  // Calculating the distance
  rearMidDistance  = rearMidDuration  * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  Serial.println(rearMidDistance);
  return rearMidDistance;
}

int myRearRightDistance() {
  // Clears the trigPin
  digitalWrite(rearRightTrig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(rearRightTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(rearRightTrig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  rearRightDuration = pulseIn(rearRightEcho, HIGH);
  // Calculating the distance
  rearRightDistance  = rearRightDuration  * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  Serial.println(rearRightDistance);
  return rearRightDistance;
}
