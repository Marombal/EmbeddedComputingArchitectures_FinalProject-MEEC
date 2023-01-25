#include "movement.h"
#include "defines.h"


void teste(){

  Serial.println("TESTE");

}

void forward(){
  Serial.println("Forward");
  /*
  digitalWrite(AIN1,HIGH);    // AIN1 - 25 (BB)
  digitalWrite(AIN2,LOW);     // AIN2 - 24 (BB)
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);
  */

  analogWrite(AIN1, MAX_SPEED);
  analogWrite(AIN2, MIN_SPEED);
  analogWrite(BIN1, MIN_SPEED);
  analogWrite(BIN2, MAX_SPEED);

}

void stop(){
  Serial.println("Stop");
  analogWrite(AIN1,MIN_SPEED); 
  analogWrite(AIN2,MIN_SPEED);
  analogWrite(BIN1,MIN_SPEED); 
  analogWrite(BIN2,MIN_SPEED);
}

/*void backwards(){
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
}*/

void ajust_right(){
  analogWrite(AIN1, MAX_SPEED - 50);
  analogWrite(AIN2, MIN_SPEED);
  analogWrite(BIN1, MIN_SPEED);
  analogWrite(BIN2, MAX_SPEED);
}

void ajust_left(){
  analogWrite(AIN1, MAX_SPEED);
  analogWrite(AIN2, MIN_SPEED);
  analogWrite(BIN1, MIN_SPEED);
  analogWrite(BIN2, MAX_SPEED - 50);
}

void turn_left(){
  analogWrite(AIN1, MAX_SPEED);
  analogWrite(AIN2, MIN_SPEED);
  analogWrite(BIN1, MIN_SPEED);
  analogWrite(BIN2, MIN_SPEED);
}

void turn_right(){
  analogWrite(AIN1, MIN_SPEED);
  analogWrite(AIN2, MIN_SPEED);
  analogWrite(BIN1, MIN_SPEED);
  analogWrite(BIN2, MAX_SPEED);
}