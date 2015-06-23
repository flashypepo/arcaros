/*
	ServosBMS620MG.ino - Controller of the servos.
	Version 1.0.0
	APica
	21/04/2015
*/

#include "Project.h"
#include <Adafruit_PWMServoDriver.h>

typedef struct servosBMS620MG_servo_e
{
	boolean isInMovement;
	unsigned long timeMovementFinished;
}servosBMS620MG_servo_t;

//***************************************************************************//
//**                          Private variables                            **//
//***************************************************************************//
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

struct servosBMS620MG_s
{
	servosBMS620MG_servo_t servos[NUM_MOTORS];
}servosBMS620MG;
		
//***************************************************************************//
//**                          Initialization function                      **//
//***************************************************************************//
void ServosBMS620MG_Init(void)
{
      	Serial.println("\t\t\tServosBMS620MG_Init");
	uint8_t i = 0;
	//Initialize the internal parameters
	for(i = 0; i < NUM_MOTORS; i++)
	{
  		servosBMS620MG.servos[i].isInMovement = false;
	}
	//Start generation of PWM 
	pwm.begin();
	pwm.setPWMFreq(60);	//Analog servos run at ~60 Hz updates
}

//***************************************************************************//
//**                          Statemachine functions                       **//
//***************************************************************************//

//***************************************************************************//
//**                          Public functions                             **//
//***************************************************************************//

void ServosBMS620MG_MoveToAngle(uint8_t servoId, uint16_t angle, unsigned long movementTime)
{
      	int pwmValue;
	
        //Set the new values for the motor
        servosBMS620MG.servos[servoId].timeMovementFinished = millis() + movementTime;
        servosBMS620MG.servos[servoId].isInMovement = true;

	//Move the motor to the selected position
	//pwmValue = (MIN_ANGLE_MOTOR + float(angle / 180) * (MAX_ANGLE_MOTOR - MIN_ANGLE_MOTOR));
        pwmValue =  map(angle, 0, 360, PWM_MIN_PULSE, PWM_MAX_PULSE);
        pwm.setPWM(servoId, 0, pwmValue);
        Serial.print("\t\t\tServosBMS620MG_MoveToAngle: Id=");
        Serial.print(servoId);
        Serial.print("; Angle=");
        Serial.println(angle);
}


boolean ServosBMS620MG_IsMovementDone(uint8_t servoId)
{
	if(servosBMS620MG.servos[servoId].isInMovement == true)
	{
  		//Servo in movement
		if(millis() >= servosBMS620MG.servos[servoId].timeMovementFinished)
		{
			//Movement done!
			servosBMS620MG.servos[servoId].isInMovement = false;
			return true;
		}
		else
		{
  			//Movement not done yet!
  			return false;
  		}
  	}
  	else
  	{
		//The motor is not in movement!
		Serial.println("[ServoBMS620MG]Servo is not in movement!");
    		return false;
    	}
}

//***************************************************************************//
//**                          Private functions                            **//
//***************************************************************************//

