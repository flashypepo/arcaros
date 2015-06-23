/*
	ApplicationBonny.ino - Main application.
	Version 0.0.2 APica/PePo 03/06/2015
	Version 0.0.1 APica/PePo 12/05/2015
	Version 0.0.0 APica 02/03/2015
*/

#include "Project.h"

typedef enum applicationBonny_states_e
{
	APPLICATIONBONNY_STATE_WAITSTARTHUG,
	APPLICATIONBONNY_STATE_STARTHUG,
	APPLICATIONBONNY_STATE_WAITSTARTHUGDONE,
	APPLICATIONBONNY_STATE_WAITHUGEND,
	APPLICATIONBONNY_STATE_HUGEND,
	APPLICATIONBONNY_STATE_WAITHUGENDDONE,
        APPLICATIONBONNY_STATE_NULL,  //TODO TEST
}applicationBonny_states_t;

//***************************************************************************//
//**                          Private variables                            **//
//***************************************************************************//
struct applicationBonny_s
{
	applicationBonny_states_t      motorState; // state in which Bonny can be
	uint8_t			       pressionChange;	
}applicationBonny;

//***************************************************************************//
//**                          Constructor                                  **//
//***************************************************************************//
void ApplicationBonny_Init(void)
{
  	Serial.println("ApplicationBonny_Init");
	//Initialize the internal parameters
	applicationBonny.motorState = APPLICATIONBONNY_STATE_WAITSTARTHUG;
	applicationBonny.pressionChange = 0;

	Actions_Init();

        //Initialize the acquisition module (sample each 100ms). Notify the changes to the application
        FSR408Acquisition_Init(100, ApplicationBonny_CallBackPressionChange);
}

//***************************************************************************//
//**                          Public functions                             **//
//***************************************************************************//

void ApplicationBonny_CallBackPressionChange(void)
{
	//Pression detected
	applicationBonny.pressionChange = 1;
	//TODO: FOR TEST ONLY!
	Serial.println("Change detected!");

        // future: data logging starts here
        // Database_StoreData("FSR408 change detected", millis()):
}

// 2015_0609 should I add Actions_DoSayYes() on pressureChange in ApplicationBonny_Motor: Yes
// remove the word HUG from states!!
void ApplicationBonny_Motor(void)
{
	switch(applicationBonny.motorState)
	{
		case APPLICATIONBONNY_STATE_WAITSTARTHUG:
			//Wait until a pression change is detected
			if(applicationBonny.pressionChange)
			{
				applicationBonny.motorState = APPLICATIONBONNY_STATE_STARTHUG;
			}
			break;

		case APPLICATIONBONNY_STATE_STARTHUG:
			Serial.println("APPLICATIONBONNY_STATE_STARTHUG");
			//Action: make a hug
                        Actions_DoHug();
                        Actions_SayYes(); // 2015_0609 added PePo
			applicationBonny.motorState = APPLICATIONBONNY_STATE_WAITSTARTHUGDONE;
			break;

		case APPLICATIONBONNY_STATE_WAITSTARTHUGDONE:
			//Wait until the do-hug and sayyes is finished
			if( Actions_IsDoHugDone() && Actions_IsSayYesDone() )
			{
  				Serial.println("APPLICATIONBONNY_STATE_WAITSTARTHUGDONE OK");
				applicationBonny.pressionChange = 0;
				applicationBonny.motorState = APPLICATIONBONNY_STATE_WAITHUGEND;
			}
			break;
			
		case APPLICATIONBONNY_STATE_WAITHUGEND:
			//Wait the end of the hug
			if(applicationBonny.pressionChange)
			{
    				Serial.println("APPLICATIONBONNY_STATE_WAITHUGEND OK");
				applicationBonny.motorState = APPLICATIONBONNY_STATE_HUGEND;
			}
			break;
			
		case APPLICATIONBONNY_STATE_HUGEND:
    			Serial.println("APPLICATIONBONNY_STATE_HUGEND");
			//release the hug
			Actions_ReleaseHug();  //TODO...
                        Actions_SayYes(); // 2015_0609 added PePo
			applicationBonny.motorState = APPLICATIONBONNY_STATE_WAITHUGENDDONE;
			break;
			
		case APPLICATIONBONNY_STATE_WAITHUGENDDONE:
                        // ait until the hug and sayyes is finished
			if(Actions_IsReleaseHugDone() && Actions_IsSayYesDone() )
			{
      				Serial.println("APPLICATIONBONNY_STATE_WAITHUGENDDONE OK");
				applicationBonny.pressionChange = 0;
				applicationBonny.motorState = APPLICATIONBONNY_STATE_WAITSTARTHUG;
			}
			break;
	}
}

//***************************************************************************//
//**                          Private functions                            **//
//***************************************************************************//
