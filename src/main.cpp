#include <Arduino.h>
#include <WiFi.h>
#include "pico/cyw43_arch.h"
#include <Wire.h>
#include <VL53L0X.h>

#define LED 16
#define AIN1 4
#define AIN2 5
#define BIN1 8
#define BIN2 9

#define XSHUT_Front 17
#define XSHUT_Right 18
#define XSHUT_Left 19

#define Address_Right 0x31
#define Address_Left 0x33

VL53L0X tof;
VL53L0X tof_right;
VL53L0X tof_left;

float distance, prev_distance;

float distance_right, prev_distance_right;

float distance_left, prev_distance_left;


int LED_state;
unsigned long interval;
unsigned long currentMicros, previousMicros;
int loop_count;

void forward();
void stop();
void backwards();

void setup() 
{
  pinMode(XSHUT_Front, OUTPUT);
  pinMode(XSHUT_Right, OUTPUT);
  pinMode(XSHUT_Left, OUTPUT);

  digitalWrite(XSHUT_Front, LOW);
  digitalWrite(XSHUT_Right, LOW);
  digitalWrite(XSHUT_Left, LOW);

  interval = 40 * 1000;

  Serial.begin(115200);

  Wire.setSDA(12);
  Wire.setSCL(13);  

  Wire.begin();




  digitalWrite(XSHUT_Front, HIGH); 

  tof.setTimeout(500);
  while (!tof.init()) {
    Serial.println(F("Failed to detect and initialize VL53L0X!"));
    delay(100);
  }  
  Serial.println(F("FRONT. Success initialize VL53L0X!"));
  Serial.println(F("Changing address to 0x31"));


  tof.setAddress(Address_Right);

  digitalWrite(XSHUT_Right, HIGH);

  tof_right.setTimeout(500);
  while (!tof_right.init()){
    Serial.println(F("Failed to detect and initialize VL53L0X! RIGHT"));
    delay(100);
  }

  tof_right.setAddress(Address_Left);

  digitalWrite(XSHUT_Left, HIGH);

  tof_left.setTimeout(500);
  while (!tof_left.init()){
    Serial.println(F("Failed to detect and initialize VL53L0X! RIGHT"));
    delay(100);
  }



  // Reduce timing budget to 20 ms (default is about 33 ms)
  //tof.setMeasurementTimingBudget(20000);

  // Start new distance measure
  tof.startReadRangeMillimeters();  
  tof_right.startReadRangeMillimeters();
  tof_left.startReadRangeMillimeters();

  //tof2.startReadRangeMillimeters(); 
  //WiFi.begin

  pinMode(LED, OUTPUT);

  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
}

#define CYW43_WL_GPIO_LED_PIN 0

void loop() 
{
  currentMicros = micros();

  // THE Control Loop
  if (currentMicros - previousMicros >= interval) {
    previousMicros = currentMicros;

    if (tof.readRangeAvailable()) {
      prev_distance = distance;
      distance = tof.readRangeMillimeters() * 1e-3;
    }

    //new

    
    if (tof_right.readRangeAvailable()){
      prev_distance_right = distance_right;
      distance_right = tof_right.readRangeMillimeters() * 1e-3;
    }

    if (tof_left.readRangeAvailable()){
      prev_distance_left = distance_left;
      distance_left = tof_left.readRangeMillimeters() * 1e-3;
    }
    
 
    // Start new distance measure
    tof.startReadRangeMillimeters(); 

    //new
    tof_right.startReadRangeMillimeters();
    tof_left.startReadRangeMillimeters();
    

    Serial.print(" Dist:: ");
    Serial.print(distance*100, 3);
    Serial.println();

    Serial.print(" Dist RIGHT : ");
    Serial.print(distance_right*100 - (8.5-5), 3);
    Serial.println();

    Serial.print(" Dist LEFT : ");
    Serial.print(distance_left*100 - (8-5), 3);
    Serial.println();

    
    if(distance*100 > 10){
      digitalWrite(LED, 1);
      forward();
    }
    else{  
      digitalWrite(LED, 0);
      stop();
    }

  }
}



void forward(){
  Serial.println("Forward");
  digitalWrite(AIN1,HIGH); 
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);
}

void stop(){
  Serial.println("Stop");
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,LOW);
}

void backwards(){
  Serial.println("Backwards");
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);
}

void right(){
  Serial.println("Right");
  digitalWrite(AIN1,HIGH); 
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);
}

void left(){
  Serial.println("Left");
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);
}
