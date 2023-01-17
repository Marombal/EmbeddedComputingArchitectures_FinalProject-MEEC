#ifndef DEFINES
#define DEFINES

/* LED's PIN */
#define LED 16
#define LED_RIGHT 21
#define LED_LEFT 22

/* DRIVER MOTOR CONTROL PINS */
#define AIN1 4
#define AIN2 5
#define BIN1 8
#define BIN2 9

/* SENSOR CONTROL PINS */
#define XSHUT_Front 17
#define XSHUT_Right 18
#define XSHUT_Left 19

/* SENSORS ADDRESS */
#define Address_Right 0x31
#define Address_Left 0x33

/* BUTTON PIN */
#define Button_Init 20

/* OUTPUTS LED CONTROL*/
static volatile boolean LF, LR, LL;

#endif