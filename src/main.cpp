#include <Arduino.h>
#include <WiFi.h>
#include "pico/cyw43_arch.h"
#include <Wire.h>
#include <VL53L0X.h>

#include "movement.h"
#include "fsm.h"
#include "defines.h"


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


/* Encoder Variables */
// LEFT Motor
//const byte encoder0pinA = 7;//A pin -> the interrupt pin 0 left
//const byte encoder0pinB = 6;//B pin -> the digital pin 3 left
byte encoder0PinALast;
int durationLeft;//the number of the pulses
boolean DirectionLeft;//the rotation direction
//RIGHT Motor
//const byte encoder1pinA = 2;//A pin -> the interrupt pin 0 left
//const byte encoder1pinB = 3;//B pin -> the digital pin 3 left
byte encoder1PinALast;
int durationRight;//the number of the pulses
boolean DirectionRight;//the rotation direction

void forward();
void stop();
void backwards();
void serialprints();

fsm_t OperationMode;
fsm_t MovementMode;
fsm_t FindWallMode;
fsm_t LED_DIRECTION;
int stateOperationMode = 0, stateLED_DIRECTION = 0;
int ButtonInit = 0, prev_ButtonInit = 0;

void wheelSpeedLeft();
void wheelSpeedRight();
void EncoderInit()
{
  DirectionLeft = true;//default -> Forward
  pinMode(encoder0pinB,INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0pinA), wheelSpeedLeft, CHANGE);

  DirectionRight = true;
  pinMode(encoder1pinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder1pinA), wheelSpeedRight, CHANGE);
}
void wheelSpeedLeft()
{
  int Lstate = digitalRead(encoder0pinA);
  if((encoder0PinALast == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoder0pinB);
    if(val == LOW && DirectionLeft)
    {
      DirectionLeft = false; //Reverse
    }
    else if(val == HIGH && !DirectionLeft)
    {
      DirectionLeft = true;  //Forward
    }
  }
  encoder0PinALast = Lstate;

  if(!DirectionLeft)  durationLeft++;
  else  durationLeft--;
}
void wheelSpeedRight()
{
  int Lstate = digitalRead(encoder1pinA);
  if((encoder1PinALast == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoder1pinB);
    if(val == LOW && DirectionRight)
    {
      DirectionRight = false; //Reverse
    }
    else if(val == HIGH && !DirectionRight)
    {
      DirectionRight = true;  //Forward
    }
  }
  encoder1PinALast = Lstate;

  if(!DirectionRight)  durationRight++;
  else  durationRight--;
}


void setup() 
{

  EncoderInit();

  pinMode(XSHUT_Front, OUTPUT);
  pinMode(XSHUT_Right, OUTPUT);
  pinMode(XSHUT_Left, OUTPUT);

  pinMode(Button_Init, INPUT);

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
    Serial.println(F("Failed to detect and initialize VL53L0X! LEFT"));
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
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_LEFT, OUTPUT);

  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);



  set_state(OperationMode, START);
  set_state(MovementMode, START);
  set_state(FindWallMode, START);
  set_state(LED_DIRECTION, START);
}

#define CYW43_WL_GPIO_LED_PIN 0

void loop() 
{
  currentMicros = micros();
  // THE Control Loop
  if (currentMicros - previousMicros >= interval) {
    previousMicros = currentMicros;

    unsigned long cur_time = millis();
    OperationMode.tis = cur_time - OperationMode.tes;

    /* READ INPUTS */

    // Read Button
    prev_ButtonInit = ButtonInit;    
    ButtonInit = !digitalRead(Button_Init);

    // Read Sensors (ToF)
    if (tof.readRangeAvailable()) {
      prev_distance = distance;
      distance = tof.readRangeMillimeters() * 1e-3;
    }
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
    tof_right.startReadRangeMillimeters();
    tof_left.startReadRangeMillimeters();
    

    //calculate next state
    operationmode_calc_next_state(OperationMode, ButtonInit, prev_ButtonInit);
    movementmode_calc_next_state(MovementMode, OperationMode, FindWallMode, distance*100, distance_right*100, distance_left*100);
    findWallMode_calc_next_state(FindWallMode, OperationMode, distance*100, distance_left*100, distance_right*100);

    //update state
    set_state(OperationMode, OperationMode.state_new);
    set_state(MovementMode, MovementMode.state_new);
    set_state(FindWallMode, FindWallMode.state_new);

    // Actions set by the current state
    operationmode_calc_outputs(OperationMode);
    movementmode_calc_outputs(MovementMode, distance_left*100, distance_right*100);
    findWallMode_calc_outputs(FindWallMode);

    // Update the outputs
    outputs();

    /* Left ENCODER */
    Serial.print("Left Speed:");    Serial.println(durationLeft);
    durationLeft = 0;
    /* Right ENCODER */
    Serial.print("Right Speed:");    Serial.println(durationRight);
    durationRight = 0;

    // Serial Prints (For debug and control prupose)
    serialprints();
    
  }
}


void serialprints(){
    // Serial Prints (For debug and control prupose)


    Serial.print(" Dist FRONT: ");    Serial.println(distance*100, 3);
    Serial.print(" Dist RIGHT : ");    Serial.println(distance_right*100, 3);
    Serial.print(" Dist LEFT : ");    Serial.println(distance_left*100, 3);

    Serial.print("State OperationMode: "); Serial.println(OperationMode.state);
    Serial.print("State MovementMode: "); Serial.print(MovementMode.state);
    if(MovementMode.state == STOP) Serial.println(" STOP");
    else if(MovementMode.state == START) Serial.println(" START");
    else if(MovementMode.state == MOVE) Serial.println(" MOVE");
    else if(MovementMode.state == MOVEMENT_RIGHT) Serial.println(" MOVEMENT_RIGHT");
    else if(MovementMode.state == MOVEMENT_LEFT) Serial.println(" MOVEMENT_LEFT");
    Serial.print("State FindWallMode: "); Serial.println(FindWallMode.state);
}

