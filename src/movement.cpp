#include "movement.h"

void teste(){

  Serial.println("TESTE");

}

void forward(){
  Serial.println("Forward");
  digitalWrite(AIN1,HIGH);    // AIN1 - 25 (BB)
  digitalWrite(AIN2,LOW);     // AIN2 - 24 (BB)
  
  //digitalWrite(BIN1,HIGH);  // BIN1 - 20 (BB)
  //digitalWrite(BIN2,LOW);   // BIN2 - 19 (BB)

  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);

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