/*
	Project.h - Project header.
	Version 0.0.0
	APica
	07/02/2015
*/

#ifndef _Project_h
#define _Project_h

//***************************************************************************//
//**                          Common includes                              **//
//***************************************************************************//
#include <Arduino.h>

//***************************************************************************//
//**                          Common type definitions                      **//
//***************************************************************************//
typedef void (*CallBackType)(void);

typedef struct servoAngles_s
{
	uint16_t minimumValue;
	uint16_t maximumValue;
	uint16_t defaultValue;
}servoAngles_t;

//***************************************************************************//
//**                          Common definitions                           **//
//***************************************************************************//
#define NUM_MOTORS		(uint8_t)6
#define PWM_MIN_PULSE		(uint16_t)180
#define PWM_MAX_PULSE		(uint16_t)550

#define SERVOID_HEAD_VERTICAL	(uint8_t)0	//Say yes movement
#define SERVOID_HEAD_HORIZONTAL	(uint8_t)1	//Say no movement
#define SERVOID_ARM_RIGHT	(uint8_t)2	//Left arm
#define SERVOID_ARM_LEFT	(uint8_t)3	//Right arm
#define SERVOID_HAND_RIGHT	(uint8_t)4	//Left hand
#define SERVOID_HAND_LEFT	(uint8_t)5	//Right hand

#define MOVEMENT_TIME_SLOW	(uint8_t)8	//Slow speed movement
#define MOVEMENT_TIME_MEDIUM	(uint8_t)4	//Medium speed movement
#define MOVEMENT_TIME_FAST	(uint8_t)1	//Fast speed movement

//***************************************************************************//
//**                          Private constants                            **//
//***************************************************************************//
servoAngles_t servoAngles[NUM_MOTORS] = { 
	//Servo 0 - SERVOID_HEAD_VERTICAL (MIN = Down, MAX = Up)
	{7, 320, 180},
	//Servo 1 - SERVOID_HEAD_HORIZONTAL (MIN = Left, MAX = Right)
	{5, 316, 130},
	//Servo 2 - SERVOID_ARM_RIGHT (MIN = Closed, MAX = Opened)
	{80, 310, 220},
	//Servo 3 - SERVOID_ARM_LEFT (MIN = Opened, MAX = Closed)
	{7, 324, 220},
	//Servo 4 - SERVOID_HAND_RIGHT (MIN = Opened, MAX = Closed)
	{20, 65, 20},  //TODO...
	//Servo 5 - SERVOID_HAND_LEFT (TODO...)
	{30, 60, 30}  //TODO...
};

//Phisical boundary values
	//Servo 0 - 6, 320
	//Servo 1 - 4, 316
	//Servo 2 - 8, 321
	//Servo 3 - 7, 324
	//Servo 4 - 260, 322
	//Servo 5 - 40, 130

#endif

