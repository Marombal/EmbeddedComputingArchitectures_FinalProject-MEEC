#include "movement.h"
#include "defines.h"


void teste(){

  Serial.println("TESTE");

}

void forward(){
  Serial.println("Forward");
  analogWrite(AIN1, MAX_SPEED - 13);
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