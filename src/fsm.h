#ifndef FSM
#define FSM

#include <Arduino.h>
#include <WiFi.h>
#include "pico/cyw43_arch.h"
#include <Wire.h>
#include <VL53L0X.h>

typedef struct{
  int state, state_new;
  unsigned long tes, tis;
} fsm_t;


void set_state(fsm_t &fsm, int state_new);
void operationmode_calc_next_state(fsm_t& OperationMode, int b, int prev_b);
void operationmode_calc_outputs(fsm_t& OperationMode);
void outputs();

#endif