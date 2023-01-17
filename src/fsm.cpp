#include "fsm.h"
#include "movement.h"
#include "defines.h"

void set_state(fsm_t &fsm, int state_new){
  if(fsm.state != state_new){
    fsm.state = state_new;
    fsm.tis = 0;                  /* tis = time in state    */
    fsm.tes = millis();           /* tes = time enter state */
  } 
}

void operationmode_calc_next_state(fsm_t& OperationMode, int b, int prev_b){

  if((OperationMode.state == 0) && (b) && (!prev_b)){
    OperationMode.state_new = 1;
  }
  else if((OperationMode.state == 0) && OperationMode.tis > 500){
    OperationMode.state_new = 2;
  }
  else if((OperationMode.state == 1) && (b) && (!prev_b)){
    OperationMode.state_new = 0;
  }
  else if((OperationMode.state == 2) && (b) && (!prev_b)){
    OperationMode.state_new = 1;
  }
  else if((OperationMode.state == 2) && OperationMode.tis > 500){
    OperationMode.state_new = 0;
  }
}

void operationmode_calc_outputs(fsm_t& OperationMode){
  if(OperationMode.state == 0){
    LF = LOW;
    LR = LOW;
    LL = LOW;
    Serial.println("STATE 0 - Desligado");
  }
  else if(OperationMode.state == 1){
    Serial.println("STATE 1 - Ligado");
  }
  else if(OperationMode.state == 2){
    LF = HIGH;
    LR = HIGH;
    LL = HIGH;
    Serial.println("STATE 2 - Desligado");
  }

}

void movementmode_calc_next_state(fsm_t& MovementMode, fsm_t& OperationMode, int SF){
  if((MovementMode.state == 0) && (OperationMode.state == 1)){
    MovementMode.state_new = 1;
  }
  else if((MovementMode.state == 1) && (OperationMode.state != 1)){
    MovementMode.state_new = 0;
  }
  else if((MovementMode.state == 1) && SF > 25){
    MovementMode.state_new = 2;
  }
  else if((MovementMode.state == 2) && (OperationMode.state != 1)){
    MovementMode.state_new = 0;
  }
    else if((MovementMode.state == 2) && SF <= 25){
    MovementMode.state_new = 1;
  }
}

void movementmode_calc_outputs(fsm_t& MovementMode){
  if(MovementMode.state == 0){
    stop();
  }
  else if(MovementMode.state == 1){
    stop();
    LF = HIGH;
    LR = HIGH;
    LL = HIGH;
  }
  else if(MovementMode.state == 2){
    forward();
    LF = HIGH;
    LR = LOW;
    LL = LOW;
  }

}

void outputs(){
  digitalWrite(LED, LF);
  digitalWrite(LED_RIGHT, LR);
  digitalWrite(LED_LEFT, LL);
}

