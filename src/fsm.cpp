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

  if((OperationMode.state == OFF_LEDS_OFF) && (b) && (!prev_b)){
    OperationMode.state_new = ON;
  }
  else if((OperationMode.state == OFF_LEDS_OFF) && OperationMode.tis > 500){
    OperationMode.state_new = OFF_LEDS_ON;
  }
  else if((OperationMode.state == ON) && (b) && (!prev_b)){
    OperationMode.state_new = OFF_LEDS_OFF;
  }
  else if((OperationMode.state == OFF_LEDS_ON) && (b) && (!prev_b)){
    OperationMode.state_new = ON;
  }
  else if((OperationMode.state == OFF_LEDS_ON) && OperationMode.tis > 500){
    OperationMode.state_new = OFF_LEDS_OFF;
  }
}

void operationmode_calc_outputs(fsm_t& OperationMode){
  if(OperationMode.state == OFF_LEDS_OFF){
    LF = LOW;
    LR = LOW;
    LL = LOW;
    Serial.println("STATE 0 - Desligado");
  }
  else if(OperationMode.state == ON){
    Serial.println("STATE 1 - Ligado");
  }
  else if(OperationMode.state == OFF_LEDS_ON){
    LF = HIGH;
    LR = HIGH;
    LL = HIGH;
    Serial.println("STATE 2 - Desligado");
  }

}

void movementmode_calc_next_state(fsm_t& MovementMode, fsm_t& OperationMode, int SF){
  if((MovementMode.state == START) && (OperationMode.state == ON)){
    MovementMode.state_new = STOP;
  }
  else if((MovementMode.state == STOP) && (OperationMode.state != ON)){
    MovementMode.state_new = START;
  }
  else if((MovementMode.state == STOP) && SF > 25){
    MovementMode.state_new = MOVE;
  }
  else if((MovementMode.state == MOVE) && (OperationMode.state != ON)){
    MovementMode.state_new = START;
  }
  else if((MovementMode.state == MOVE) && SF <= 25){
    MovementMode.state_new = STOP;
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
    // teste forward();
    LF = HIGH;
    LR = LOW;
    LL = LOW;
  }

}

void findWallMode_calc_next_state(fsm_t& FindWallMode, fsm_t& OperationMode, int SF, int SL, int SR){
  if(OperationMode.state != 1){
    FindWallMode.state_new = START;
  }
  else if((FindWallMode.state == START) && SF > 25 && SR > 35 && SL <= 35){
    FindWallMode.state_new = FOLLOW_LEFT;
  }
  else if((FindWallMode.state == START) && SF > 25 && SL > 35 && SR <= 35){
    FindWallMode.state_new = FOLLOW_RIGHT;
  }
  else if((FindWallMode.state == START) && SF > 25 && SL > 35 && SR > 35){
    FindWallMode.state_new = MOVE_STRAIGHT;
  }
  else if((FindWallMode.state == MOVE_STRAIGHT) && SL < SR){
    FindWallMode.state_new = FOLLOW_LEFT;
  }
  else if((FindWallMode.state == MOVE_STRAIGHT) && SL >= SR){
    FindWallMode.state_new = FOLLOW_RIGHT;
  }
}

void findWallMode_calc_outputs(fsm_t& FindWallMode){
  // No outputs
}

void outputs(){
  digitalWrite(LED, LF);
  digitalWrite(LED_RIGHT, LR);
  digitalWrite(LED_LEFT, LL);
}

