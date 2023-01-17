#include "fsm.h"
#include "movement.h"

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
  else if((OperationMode.state == 1) && (b) && (!prev_b)){
    OperationMode.state_new = 0;
  }
}

void operationmode_calc_outputs(fsm_t& OperationMode){
  if(OperationMode.state == 0){
    Serial.println("Desligado");
  }
  else if(OperationMode.state == 1){
    Serial.println("Ligado");
  }
}

