/*
	Actions.ino - Actions to make Movements.
	Version 0.0.2  APica/PePo 12/05/2015
	Version 0.0.1  APica 02/03/2015
*/

#include "Project.h"

typedef enum actions_states_e
{
	ACTIONS_STATE_WAITING,
	ACTIONS_STATE_STARTMOVEMENT,
	ACTIONS_STATE_WAITMOVEMENTDONE,
	ACTIONS_STATE_GETFINISHED
}actions_states_t;

//***************************************************************************//
//**                          Private constants                            **//
//***************************************************************************//
#define ACTIONS_NUM_STATEMACHINES 10     //TODO

#define ACTIONS_STATEMACHINE_DOHUG 0
#define ACTIONS_STATEMACHINE_SAYYES 1
#define ACTIONS_STATEMACHINE_DO_RELEASE_HUG 2

//***************************************************************************//
//**                          Private variables                            **//
//***************************************************************************//

struct actions_s
{
	actions_states_t	motorState[ACTIONS_NUM_STATEMACHINES];
        boolean                 isDone[ACTIONS_NUM_STATEMACHINES];
}actions;
		
//***************************************************************************//
//**                          Constructor                                  **//
//***************************************************************************//
void Actions_Init()
{
	Serial.println("\tActions_Init");
	//Initialize lower layers
	Movements_Init();

        //Initialize the internal parameters
	for(int i = 0; i < ACTIONS_NUM_STATEMACHINES; i++)
	{
  		actions.motorState[i] = ACTIONS_STATE_WAITING;
                actions.isDone[i] = false;
	}

}

//***************************************************************************//
//**                          Statemachine functions                       **//
//***************************************************************************//
// convenient/helper function to call all action statemachines in one function
void Actions_Motor(void)
{
  	Actions_MotorDoHug();
	Actions_MotorSayYes();
        Actions_MotorDoReleaseHug();
        // and so on...
}

void Actions_MotorDoHug(void)
{
        // 1= doHug state..
	switch(actions.motorState[ACTIONS_STATEMACHINE_DOHUG])
	{
  		case ACTIONS_STATE_WAITING:
			break;

		case ACTIONS_STATE_STARTMOVEMENT:
			Serial.println("\tACTIONS_STATE_STARTMOVEMENT");
                        Movements_LeftArm(0); 
                        Movements_RightArm(0); 
			actions.motorState[ACTIONS_STATEMACHINE_DOHUG] = ACTIONS_STATE_WAITMOVEMENTDONE;
			break;

		case ACTIONS_STATE_WAITMOVEMENTDONE:
                        //Wait until left and right arm movements are done
			if(Movements_IsLeftArmDone() && Movements_IsRightArmDone() )
			{
  				Serial.println("\tACTIONS_STATE_WAITMOVEMENTDONE OK");
                                // action of doHug is done
                                actions.isDone[ACTIONS_STATEMACHINE_DOHUG] = true;
				//Start the movement in the other way
				actions.motorState[ACTIONS_STATEMACHINE_DOHUG] = ACTIONS_STATE_WAITING;
			}
			break;
         } // endof switch
}
// 2015_0609 added do release hug
void Actions_MotorDoReleaseHug(void)
{
        // 1= doHug state..
	switch(actions.motorState[ACTIONS_STATEMACHINE_DO_RELEASE_HUG])
	{
  		case ACTIONS_STATE_WAITING:
			break;

		case ACTIONS_STATE_STARTMOVEMENT:
			Serial.println("\tACTIONS_STATE_STARTMOVEMENT Do_Release_Hug");
                        Movements_LeftArm(1); 
                        Movements_RightArm(1); 
			actions.motorState[ACTIONS_STATEMACHINE_DO_RELEASE_HUG] = ACTIONS_STATE_WAITMOVEMENTDONE;
			break;

		case ACTIONS_STATE_WAITMOVEMENTDONE:
                        //Wait until left and right arm movements are done
			if(Movements_IsLeftArmDone() && Movements_IsRightArmDone() )
			{
  				Serial.println("\tACTIONS_STATE_WAITMOVEMENTDONE OK");
                                // action of doReleaseHug is done
                                actions.isDone[ACTIONS_STATEMACHINE_DO_RELEASE_HUG] = true;
				//Start the movement in the other way
				actions.motorState[ACTIONS_STATEMACHINE_DO_RELEASE_HUG] = ACTIONS_STATE_WAITING;
			}
			break;
         } // endof switch
}
	
// TODO
// 2015_0609 PePo first version
void Actions_MotorSayYes(void)
{
        // 1= doHug state..
	switch(actions.motorState[ACTIONS_STATEMACHINE_SAYYES])
	{
  		case ACTIONS_STATE_WAITING:
			break;

		case ACTIONS_STATE_STARTMOVEMENT:
			Serial.println("\tACTIONS_STATE_STARTMOVEMENT: head down");
                        Movements_Head(1); 
			actions.motorState[ACTIONS_STATEMACHINE_SAYYES] = ACTIONS_STATE_WAITMOVEMENTDONE;
			break;

		case ACTIONS_STATE_WAITMOVEMENTDONE:
                        //Wait until headvertical movement is done
			if(Movements_IsHeadVerticalDone() )
			{
                                Movements_Head(0); 
  				Serial.println("\tACTIONS_STATE_WAITMOVEMENTDONE:head up");
				//Start the movement in idle...
				actions.motorState[ACTIONS_STATEMACHINE_SAYYES] = ACTIONS_STATE_GETFINISHED;
			}
			break;

		case ACTIONS_STATE_GETFINISHED:
                        //Wait until headvertical movement is done
			if(Movements_IsHeadVerticalDone() )
			{
  				Serial.println("\tACTIONS_STATE_GETFINISHED: SayYes is finised");
                                // action of sayYes is done
                                actions.isDone[ACTIONS_STATEMACHINE_SAYYES] = true;
				//Start the movement in idle...
				actions.motorState[ACTIONS_STATEMACHINE_SAYYES] = ACTIONS_STATE_WAITING;
			}
			break;

         } // endof switch
}

//***************************************************************************//
//**                          Public functions                             **//
//***************************************************************************//

// 2015_0609 SayYes - PePo - entered into the loop =======
void Actions_SayYes(void)
{
	//Start statemachine
        actions.isDone[ACTIONS_STATEMACHINE_SAYYES] = false;  // 2015_0609
	actions.motorState[ACTIONS_STATEMACHINE_SAYYES] = ACTIONS_STATE_STARTMOVEMENT;
	//The "say yes" motor should be in the default position!
}
boolean Actions_IsSayYesDone(void) {
  return actions.isDone[ACTIONS_STATEMACHINE_SAYYES];
}
// 2015_0609 PePo endof added code for SayYes =============

// start a hug
void Actions_DoHug(void)
{
	//Start statemachine
        actions.isDone[ACTIONS_STATEMACHINE_DOHUG] = false; // 2015_0609
	actions.motorState[ACTIONS_STATEMACHINE_DOHUG] = ACTIONS_STATE_STARTMOVEMENT;
	//The "say yes" motor should be in the default position!
}
boolean Actions_IsDoHugDone(void) {
  return actions.isDone[ACTIONS_STATEMACHINE_DOHUG];
}

// end a hug
// 2015_0609 added 
void Actions_ReleaseHug(void)
{
	//Start statemachine
        actions.isDone[ACTIONS_STATEMACHINE_DO_RELEASE_HUG] = false;
	actions.motorState[ACTIONS_STATEMACHINE_DO_RELEASE_HUG] = ACTIONS_STATE_STARTMOVEMENT;
	//The "say yes" motor should be in the default position!
}
boolean Actions_IsReleaseHugDone(void) {
  return actions.isDone[ACTIONS_STATEMACHINE_DO_RELEASE_HUG];
}

//***************************************************************************//
//**                          Private functions                            **//
//***************************************************************************//


