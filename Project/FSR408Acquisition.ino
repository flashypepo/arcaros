/*
	FSR408Acquisition.ino - Acquire data from FSR408 sensor.
	Version 1.0.0
	APica
	02/03/2015
*/

#include "Project.h"
#include "FSR408.h"

#define FSR408ACQUISITION_NUMSAMPLES	(int)4
#define FSR408ACQUISITION_HISTERESIS	(float)10.0
#define FSR408ACQUISITION_PIN		(uint16_t)0	//Analog 0
#define FSR408ACQUISITION_VCC		(uint16_t)5000	//5v
#define FSR408ACQUISITION_RPULLDOWN	(uint16_t)1500	//1k5 ohms

typedef enum fsr408Acquisition_states_e
{
	FSR408ACQUISITION_STATE_GETSAMPLES,
	FSR408ACQUISITION_STATE_PROCESSSAMPLES,
}fsr408Acquisition_states_t;

void processSamples(void);

//***************************************************************************//
//**                          Private variables                            **//
//***************************************************************************//
FSR408 _fsr408(FSR408ACQUISITION_PIN, FSR408ACQUISITION_VCC, FSR408ACQUISITION_RPULLDOWN);

struct fsr408Acquisition_s
{
	fsr408Acquisition_states_t	motorState;
	int				sampleTime;
	uint8_t				indexBuffer;
	uint8_t				firstSample;
	unsigned long			timeNextSample;
	float				bufferSamples[FSR408ACQUISITION_NUMSAMPLES];
	float				averageSamples;
	CallBackType			CallBackChange;
}fsr408Acquisition;
		
//***************************************************************************//
//**                          Constructor                                  **//
//***************************************************************************//
void FSR408Acquisition_Init(int sampleTime, CallBackType CallBackChange)
{
  	Serial.println("FSR408Acquisition_Init");
	//Store the internal parameters
	fsr408Acquisition.sampleTime = sampleTime;
	fsr408Acquisition.CallBackChange = CallBackChange;
	//Initialize the internal parameters
	fsr408Acquisition.motorState = FSR408ACQUISITION_STATE_GETSAMPLES;
	fsr408Acquisition.indexBuffer = 0;
	fsr408Acquisition.averageSamples = 0.0;
	fsr408Acquisition.firstSample = 1;
	fsr408Acquisition.timeNextSample = millis() + fsr408Acquisition.sampleTime;
}

//***************************************************************************//
//**                          Public functions                             **//
//***************************************************************************//

void FSR408Acquisition_Motor(void)
{
	switch(fsr408Acquisition.motorState)
	{
		case FSR408ACQUISITION_STATE_GETSAMPLES:
			if(millis() >= fsr408Acquisition.timeNextSample)
			{
				//Get one sample
				fsr408Acquisition.bufferSamples[fsr408Acquisition.indexBuffer++] = _fsr408.readForce();
				
				//TODO: FOR TEST ONLY!!
				//Serial.println(fsr408Acquisition.bufferSamples[fsr408Acquisition.indexBuffer-1]);
				
				if(fsr408Acquisition.indexBuffer == FSR408ACQUISITION_NUMSAMPLES)
				{
					//The buffer is full. Get the average
					fsr408Acquisition.motorState = FSR408ACQUISITION_STATE_PROCESSSAMPLES;
				}
				else
				{
					//Buffer not full. Set the next sample time
					fsr408Acquisition.timeNextSample = millis() + fsr408Acquisition.sampleTime;
				}
			}
			break;
			
		case FSR408ACQUISITION_STATE_PROCESSSAMPLES:
			processSamples();
			//Restart the samples buffer
			fsr408Acquisition.indexBuffer = 0;
			fsr408Acquisition.motorState = FSR408ACQUISITION_STATE_GETSAMPLES;
			fsr408Acquisition.timeNextSample = millis() + fsr408Acquisition.sampleTime;
			break;
	}
}

//***************************************************************************//
//**                          Private functions                            **//
//***************************************************************************//

void processSamples(void)
{
	int i;
	float sum = 0.0;
	float average = 0.0;
	
	for(i = 0; i < FSR408ACQUISITION_NUMSAMPLES; i++)
	{
		sum += (float)fsr408Acquisition.bufferSamples[i];
	}
	
	average = sum/(float)FSR408ACQUISITION_NUMSAMPLES;

	if(fsr408Acquisition.firstSample == 0)
	{
  		//Normal sample
		if((average > fsr408Acquisition.averageSamples + FSR408ACQUISITION_HISTERESIS) ||
		   (average < fsr408Acquisition.averageSamples - FSR408ACQUISITION_HISTERESIS))
		{
			//New change. Notify it
			fsr408Acquisition.CallBackChange(/*average*/); // send average, as example 2015_0603
		}
	}
	else
	{
  		//First sample. Don't notify it!
  		fsr408Acquisition.firstSample = 0;	
	}
  
	fsr408Acquisition.averageSamples = average;
}
