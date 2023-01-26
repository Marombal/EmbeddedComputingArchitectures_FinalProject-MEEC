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
  }
  else if(OperationMode.state == ON){
  }
  else if(OperationMode.state == OFF_LEDS_ON){
    LF = HIGH;
    LR = HIGH;
    LL = HIGH;
  }

}

void movementmode_calc_next_state(fsm_t& MovementMode, fsm_t& OperationMode, fsm_t& FindWallMode, int SF, int SR, int SL){
  if(OperationMode.state != ON){
    MovementMode.state_new = START;
  }
  else if((MovementMode.state == START) && (OperationMode.state == ON)){
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
  else if((MovementMode.state == MOVE) && (FindWallMode.state == FOLLOW_RIGHT)){
    MovementMode.state_new = MOVEMENT_RIGHT;
  }
  else if((MovementMode.state == MOVE) && (FindWallMode.state == FOLLOW_LEFT)){
    MovementMode.state_new = MOVEMENT_LEFT;
  }
  else if((MovementMode.state == MOVE) && (FindWallMode.state == MOVE_STRAIGHT)){
    MovementMode.state_new = MOVE;
  }
  else if((MovementMode.state == MOVEMENT_RIGHT) && (OperationMode.state != ON)){
    MovementMode.state_new = START;
  }
  else if((MovementMode.state == MOVEMENT_RIGHT) && SF <= 35){
    MovementMode.state_new = TURN_LEFT_CORNER;
  }
  else if((MovementMode.state == MOVEMENT_RIGHT) && SR >= 60){ //WARNING
    MovementMode.state_new = TURN_RIGHT;
  }
  else if((MovementMode.state == TURN_LEFT_CORNER) && SF > 35){
    MovementMode.state_new = MOVEMENT_RIGHT;
  }
  else if((MovementMode.state == TURN_RIGHT) && SR < 60){
    MovementMode.state_new = MOVEMENT_RIGHT;
  }
  else if((MovementMode.state == MOVEMENT_LEFT) && (OperationMode.state != ON)){
    MovementMode.state_new = START;
  }
  else if((MovementMode.state == MOVEMENT_LEFT) && SF <= 35){
    MovementMode.state_new = TURN_RIGHT_CORNER;
  }
  else if((MovementMode.state == TURN_RIGHT_CORNER) && SF > 35){
    MovementMode.state_new = MOVEMENT_LEFT;
  }
  else if((MovementMode.state == MOVEMENT_LEFT) && SL >= 60){
    MovementMode.state_new = TURN_LEFT;
  }
  else if((MovementMode.state == TURN_LEFT) && SL < 60){
    MovementMode.state_new = MOVEMENT_LEFT;
  }

}

void movementmode_calc_outputs(fsm_t& MovementMode, int SL, int SR){
  if(MovementMode.state == START){
    stop();
  }
  else if(MovementMode.state == STOP){
    stop();
    LF = HIGH;
    LR = HIGH;
    LL = HIGH;
  }
  else if(MovementMode.state == MOVE){
    forward();
    LF = HIGH;
    LR = LOW;
    LL = LOW;
  }
  else if(MovementMode.state == MOVEMENT_RIGHT){
    if(SR <= 18 && SR >= 15){
      forward();
      Serial.println(" FORWARD");
    }
    else if(SR > 18){
      ajust_right();
      Serial.println(" ajust_right");
    }
    else if(SR < 15){
      ajust_left();
      Serial.println(" ajust_left");
    }
  }
  else if(MovementMode.state == MOVEMENT_LEFT){
    if(SL <= 12 && SL >= 10){
      forward();
      Serial.println(" FORWARD");
    }
    else if(SL > 12){
      ajust_left();
      Serial.println(" ajust_left");
    }
    else if(SL < 10){
      ajust_right();
      Serial.println(" ajust_right");
    }
  }
  else if(MovementMode.state == TURN_LEFT_CORNER){
    turn_left();
    Serial.println(" turn_left");
  }
  else if(MovementMode.state == TURN_RIGHT_CORNER){
    turn_right();
    Serial.println(" turn_right");
  }
  else if(MovementMode.state == TURN_RIGHT){
    turn_right();
    Serial.println(" turn_right2");
  }
  else if(MovementMode.state == TURN_LEFT){
    turn_left();
    Serial.println(" turn_left2");
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
  else if((FindWallMode.state == MOVE_STRAIGHT) && (SL < SR) && (SL < 50)){
    FindWallMode.state_new = FOLLOW_LEFT;
  }
  else if((FindWallMode.state == MOVE_STRAIGHT) && (SL >= SR) && (SR < 50)){
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

