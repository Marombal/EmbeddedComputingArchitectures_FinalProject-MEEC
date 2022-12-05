// 2022 Paulo Costa
// Pico W LED access

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

#define XSHUT 16

VL53L0X tof;
VL53L0X tof_direita;
VL53L0X tof_esquerda;

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
  interval = 40 * 1000;

  Serial.begin(115200);

  Wire.setSDA(12);
  Wire.setSCL(13);  

  Wire.begin();

  tof.setTimeout(500);
  while (!tof.init()) {
    Serial.println(F("Failed to detect and initialize VL53L0X!"));
    delay(100);
  }  


  // Reduce timing budget to 20 ms (default is about 33 ms)
  //tof.setMeasurementTimingBudget(20000);

  // Start new distance measure
  tof.startReadRangeMillimeters();  

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
 
    // Start new distance measure
    tof.startReadRangeMillimeters(); 

    Serial.print(" Dist: ");
    Serial.print(distance*100, 3);
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
