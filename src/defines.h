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
#define encoder0pinA 7
#define encoder0pinB 6
#define encoder1pinA 2
#define encoder1pinB 3

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

/* Speed Modes */
#define MAX_SPEED 255
#define MIN_SPEED 0

/* STATES GERAL */
#define START 0

/* STATES 'operationMode' */
#define OFF_LEDS_OFF 0
#define OFF_LEDS_ON 2
#define ON 1

/* STATES 'movementMode' */
#define STOP 1
#define MOVE 2
#define MOVEMENT_RIGHT 3
#define MOVEMENT_LEFT 4
#define TURN_LEFT_CORNER 5
#define TURN_RIGHT_CORNER 6
#define TURN_RIGHT 7
#define TURN_LEFT 8

/* STATES 'findWallMode' */
#define FOLLOW_LEFT 1
#define FOLLOW_RIGHT 2
#define MOVE_STRAIGHT 3

#endif