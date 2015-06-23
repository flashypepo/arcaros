/*
	Movements.ino - Movements to move the Bonny servos.
	Version 0.0.1  APica/PePo 12/05/2015
*/

#include "Project.h"

typedef enum movements_states_e
{
	MOVEMENTS_STATE_WAITING,
	MOVEMENTS_STATE_SMALLMOVEMENT,
	MOVEMENTS_STATE_WAITSMALLMOVEMENT,
	MOVEMENTS_STATE_FINISHED
}movements_states_t;

//***************************************************************************//
//**                          Private constants                            **//
//***************************************************************************//
#define MOVEMENTS_NUM_STATEMACHINES 10
#define MOVEMENTS_STATEMACHINE_LEFTARM 0
#define MOVEMENTS_STATEMACHINE_RIGHTARM 1
#define MOVEMENTS_STATEMACHINE_HEAD 2   // 2015_0609 PePo added head

//***************************************************************************//
//**                          Private variables                            **//
//***************************************************************************//

struct movements_S
{
	movements_states_t	motorState[MOVEMENTS_NUM_STATEMACHINES];
	uint16_t		currentAngle[NUM_MOTORS];
	uint16_t		finalAngle[NUM_MOTORS];
        boolean                 isDone[MOVEMENTS_NUM_STATEMACHINES];
}movements;
		
//***************************************************************************//
//**                          Constructor                                  **//
//***************************************************************************//
void Movements_Init()
{
    	Serial.println("\t\tMovements_Init");
	//Initialize lower layers
	ServosBMS620MG_Init();
	//Initialize the internal parameters
	for(int i = 0; i < MOVEMENTS_NUM_STATEMACHINES; i++)
	{
  		movements.motorState[i] = MOVEMENTS_STATE_WAITING;
	}
	movements.currentAngle[SERVOID_HEAD_VERTICAL] = servoAngles[SERVOID_HEAD_VERTICAL].defaultValue;
	movements.currentAngle[SERVOID_HEAD_HORIZONTAL] = servoAngles[SERVOID_HEAD_HORIZONTAL].defaultValue;
	movements.currentAngle[SERVOID_ARM_LEFT] = servoAngles[SERVOID_ARM_LEFT].defaultValue;
	movements.currentAngle[SERVOID_ARM_RIGHT] = servoAngles[SERVOID_ARM_RIGHT].defaultValue;
	movements.currentAngle[SERVOID_HAND_LEFT] = servoAngles[SERVOID_HAND_LEFT].defaultValue;
	movements.currentAngle[SERVOID_HAND_RIGHT] = servoAngles[SERVOID_HAND_RIGHT].defaultValue;
	//Move the servos to the default position
	ServosBMS620MG_MoveToAngle(SERVOID_HEAD_VERTICAL, servoAngles[SERVOID_HEAD_VERTICAL].defaultValue, MOVEMENT_TIME_FAST);
	ServosBMS620MG_MoveToAngle(SERVOID_HEAD_HORIZONTAL, servoAngles[SERVOID_HEAD_HORIZONTAL].defaultValue, MOVEMENT_TIME_FAST);
	ServosBMS620MG_MoveToAngle(SERVOID_ARM_LEFT, servoAngles[SERVOID_ARM_LEFT].defaultValue, MOVEMENT_TIME_FAST);
	ServosBMS620MG_MoveToAngle(SERVOID_ARM_RIGHT, servoAngles[SERVOID_ARM_RIGHT].defaultValue, MOVEMENT_TIME_FAST);
	ServosBMS620MG_MoveToAngle(SERVOID_HAND_LEFT, servoAngles[SERVOID_HAND_LEFT].defaultValue, MOVEMENT_TIME_FAST);
	ServosBMS620MG_MoveToAngle(SERVOID_HAND_RIGHT, servoAngles[SERVOID_HAND_RIGHT].defaultValue, MOVEMENT_TIME_FAST);
}

//***************************************************************************//
//**                          Statemachine functions                       **//
//***************************************************************************//
// convenient/helper function to call all movement statemachines in one function
void Movements_Motor(void)
{
  	Movements_MotorLeftArm();
	Movements_MotorRightArm();
	Movements_MotorHeadUp();
}

void Movements_MotorLeftArm(void)
{
	switch(movements.motorState[MOVEMENTS_STATEMACHINE_LEFTARM])
	{
  		case MOVEMENTS_STATE_WAITING:
			break;

		case MOVEMENTS_STATE_SMALLMOVEMENT:
 			///Serial.println("\t\tMOVEMENTS_STATE_SMALLMOVEMENT of leftarm");
                        if(movements.currentAngle[SERVOID_ARM_LEFT] < movements.finalAngle[SERVOID_ARM_LEFT])
			{
			       movements.currentAngle[SERVOID_ARM_LEFT]++;
			}
			else
			{
			       movements.currentAngle[SERVOID_ARM_LEFT]--;
			}
			ServosBMS620MG_MoveToAngle(SERVOID_ARM_LEFT, movements.currentAngle[SERVOID_ARM_LEFT], MOVEMENT_TIME_SLOW);
			movements.motorState[MOVEMENTS_STATEMACHINE_LEFTARM] = MOVEMENTS_STATE_WAITSMALLMOVEMENT;
			break;

		case MOVEMENTS_STATE_WAITSMALLMOVEMENT:
 			//Serial.println("\t\tMOVEMENTS_STATE_WAITSMALLMOVEMENT of leftarm");
			if(ServosBMS620MG_IsMovementDone(SERVOID_ARM_LEFT))
			{
				if(movements.currentAngle[SERVOID_ARM_LEFT] != movements.finalAngle[SERVOID_ARM_LEFT])// check if there are more small movements
				{
					//More movements remaining
					movements.motorState[MOVEMENTS_STATEMACHINE_LEFTARM] = MOVEMENTS_STATE_SMALLMOVEMENT;
				}
				else
				{
					//Movement in one way done
					movements.motorState[MOVEMENTS_STATEMACHINE_LEFTARM] = MOVEMENTS_STATE_FINISHED;
				}
			}
			break;
			
		case MOVEMENTS_STATE_FINISHED:
			//Serial.println("\t\tMOVEMENTS_STATE_FINISHED of leftarm");
                        // movement of leftArm servo is done
                        movements.isDone[MOVEMENTS_STATEMACHINE_LEFTARM] = true;
			//Go to the default state
			movements.motorState[MOVEMENTS_STATEMACHINE_LEFTARM] = MOVEMENTS_STATE_WAITING;
			break;
	} // endof switch
}
 
void Movements_MotorRightArm(void)
{
	switch(movements.motorState[MOVEMENTS_STATEMACHINE_RIGHTARM])
	{
  		case MOVEMENTS_STATE_WAITING:
			break;

		case MOVEMENTS_STATE_SMALLMOVEMENT:
 			//Serial.println("\t\tMOVEMENTS_STATE_SMALLMOVEMENT of rightarm");
                        if(movements.currentAngle[SERVOID_ARM_RIGHT] < movements.finalAngle[SERVOID_ARM_RIGHT])
			{
			       movements.currentAngle[SERVOID_ARM_RIGHT]++;
			}
			else
			{
			       movements.currentAngle[SERVOID_ARM_RIGHT]--;
			}
			ServosBMS620MG_MoveToAngle(SERVOID_ARM_RIGHT, movements.currentAngle[SERVOID_ARM_RIGHT], MOVEMENT_TIME_SLOW);
			movements.motorState[MOVEMENTS_STATEMACHINE_RIGHTARM] = MOVEMENTS_STATE_WAITSMALLMOVEMENT;
			break;

		case MOVEMENTS_STATE_WAITSMALLMOVEMENT:
 			//Serial.println("\t\tMOVEMENTS_STATE_WAITSMALLMOVEMENT of rightarm");
			if(ServosBMS620MG_IsMovementDone(SERVOID_ARM_RIGHT))
			{
				if(movements.currentAngle[SERVOID_ARM_RIGHT] != movements.finalAngle[SERVOID_ARM_RIGHT])// check if there are more small movements
				{
					//More movements remaining
					movements.motorState[MOVEMENTS_STATEMACHINE_RIGHTARM] = MOVEMENTS_STATE_SMALLMOVEMENT;
				}
				else
				{
					//Movement in one way done
					movements.motorState[MOVEMENTS_STATEMACHINE_RIGHTARM] = MOVEMENTS_STATE_FINISHED;
				}
			}
			break;
			
		case MOVEMENTS_STATE_FINISHED:
			//Serial.println("\t\tMOVEMENTS_STATE_FINISHED of rightarm");
                        // movement of leftArm servo is done
                        movements.isDone[MOVEMENTS_STATEMACHINE_RIGHTARM] = true;
			//Go to the default state
			movements.motorState[MOVEMENTS_STATEMACHINE_RIGHTARM] = MOVEMENTS_STATE_WAITING;
			break;
	} // endof switch
}
  
// 2015_0609 PEPo/Pica: statemachine similar to leftarm, but then for head SERVOID_Head_VERTICAL
void Movements_MotorHeadUp(void)
{
	switch(movements.motorState[MOVEMENTS_STATEMACHINE_HEAD])
	{
  		case MOVEMENTS_STATE_WAITING:
			break;

		case MOVEMENTS_STATE_SMALLMOVEMENT:
 			Serial.println("\t\tMOVEMENTS_STATE_SMALLMOVEMENT of the head");
                        if(movements.currentAngle[SERVOID_HEAD_VERTICAL] < movements.finalAngle[SERVOID_HEAD_VERTICAL])
			{
			       movements.currentAngle[SERVOID_HEAD_VERTICAL]++;
			}
			else
			{
			       movements.currentAngle[SERVOID_HEAD_VERTICAL]--;
			}
			ServosBMS620MG_MoveToAngle(SERVOID_HEAD_VERTICAL, movements.currentAngle[SERVOID_HEAD_VERTICAL], MOVEMENT_TIME_SLOW);
			movements.motorState[MOVEMENTS_STATEMACHINE_HEAD] = MOVEMENTS_STATE_WAITSMALLMOVEMENT;
			break;

		case MOVEMENTS_STATE_WAITSMALLMOVEMENT:
 			Serial.println("\t\tMOVEMENTS_STATE_WAITSMALLMOVEMENT of the head");
			if(ServosBMS620MG_IsMovementDone(SERVOID_HEAD_VERTICAL))
			{
				if(movements.currentAngle[SERVOID_HEAD_VERTICAL] != movements.finalAngle[SERVOID_HEAD_VERTICAL])// check if there are more small movements
				{
					//More movements remaining
					movements.motorState[MOVEMENTS_STATEMACHINE_HEAD] = MOVEMENTS_STATE_SMALLMOVEMENT;
				}
				else
				{
					//Movement in one way done
					movements.motorState[MOVEMENTS_STATEMACHINE_HEAD] = MOVEMENTS_STATE_FINISHED;
				}
			}
			break;
			
		case MOVEMENTS_STATE_FINISHED:
			Serial.println("\t\tMOVEMENTS_STATE_FINISHED - head done");
                        // movement of head vertical servo is done
                        movements.isDone[MOVEMENTS_STATEMACHINE_HEAD] = true;
			//Go to the default state
			movements.motorState[MOVEMENTS_STATEMACHINE_HEAD] = MOVEMENTS_STATE_WAITING;
			break;
	} // endof switch
}

//***************************************************************************//
//**                          Public functions                             **//
//***************************************************************************//
// direction = 0: head up
// direction = 1: head down
void Movements_Head(int direction)  {
  movements.isDone[MOVEMENTS_STATEMACHINE_HEAD] = false; // 2015_0609
  if (direction == 1) {
    movements.finalAngle[SERVOID_HEAD_VERTICAL] = servoAngles[SERVOID_HEAD_VERTICAL].minimumValue;
  } else {
    movements.finalAngle[SERVOID_HEAD_VERTICAL] = servoAngles[SERVOID_HEAD_VERTICAL].defaultValue;
  }
  /* 2015_0609 
  Serial.print("Movements_Head(");
    Serial.print(direction);
    Serial.print("), movenements.finalAngle[]=");
    Serial.println(movements.finalAngle[SERVOID_HEAD_VERTICAL]); */
  movements.motorState[MOVEMENTS_STATEMACHINE_HEAD] = MOVEMENTS_STATE_SMALLMOVEMENT;
}

// direction = 0: open
// direction = 1: close
void Movements_LeftArm(int direction)  {
  movements.isDone[MOVEMENTS_STATEMACHINE_LEFTARM] = false; // 2015_0609
  if (direction == 1) {
    movements.finalAngle[SERVOID_ARM_LEFT] = servoAngles[SERVOID_ARM_LEFT].defaultValue; // 2015_0609
  } else {
    movements.finalAngle[SERVOID_ARM_LEFT] = servoAngles[SERVOID_ARM_LEFT].maximumValue; // 2015_0609
  }
  movements.motorState[MOVEMENTS_STATEMACHINE_LEFTARM] = MOVEMENTS_STATE_SMALLMOVEMENT;
}

void Movements_RightArm(int direction)  { 
  movements.isDone[MOVEMENTS_STATEMACHINE_RIGHTARM] = false; // 2015_0609
  if (direction == 1) {
    movements.finalAngle[SERVOID_ARM_RIGHT] = servoAngles[SERVOID_ARM_RIGHT].defaultValue; // 2015_0609
  } else {
    movements.finalAngle[SERVOID_ARM_RIGHT] = servoAngles[SERVOID_ARM_RIGHT].minimumValue; // 2015_0609
  }
  movements.motorState[MOVEMENTS_STATEMACHINE_RIGHTARM] = MOVEMENTS_STATE_SMALLMOVEMENT;
}

boolean Movements_IsLeftArmDone(void)  {
  return movements.isDone[MOVEMENTS_STATEMACHINE_LEFTARM];
}
boolean Movements_IsRightArmDone(void)  {
  return movements.isDone[MOVEMENTS_STATEMACHINE_RIGHTARM];
}
// 2015_0609 PePo added...
boolean Movements_IsHeadVerticalDone(void)  {
  return movements.isDone[MOVEMENTS_STATEMACHINE_HEAD];
}

//***************************************************************************//
//**                          Private functions                            **//
//***************************************************************************//


