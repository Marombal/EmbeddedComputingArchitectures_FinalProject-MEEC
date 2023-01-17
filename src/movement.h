#ifndef MOVEMENT
#define MOVEMENT

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

/*
* TESTE
*/
void teste();

void forward();
void stop();
void backwards();
void right();
void left();

#endif