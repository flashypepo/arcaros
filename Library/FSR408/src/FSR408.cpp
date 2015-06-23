/*
	FSR408.cpp - Library for FSR408 sensor.
	Version 1.0.0
	APica
	17/01/2015
*/

#include "FSR408.h"

#define FSR408_INTERNALVOLTAGE		(uint16_t)5000

//***************************************************************************//
//**                          Constructor                                  **//
//***************************************************************************//

FSR408::FSR408(void)
{
	//Store the internal parameters
	_pinSensor = 0;
	_vccSensor = 5000;
	_rPullDown = 1500;
}

FSR408::FSR408(int pinSensor, int vccSensor, int rPullDown)
{
	//Store the internal parameters
	_pinSensor = pinSensor;
	_vccSensor = vccSensor;
	_rPullDown = rPullDown;
}

//***************************************************************************//
//**                          Public functions                             **//
//***************************************************************************//

uint16_t FSR408::readVoltage(void)
{
	//Read the analog pin of the sensor
	uint16_t analogPin = analogRead(_pinSensor);
	//Convert the read value to voltage with the internal voltage of ADC
	return map(analogPin, 0, 1023, 0, FSR408_INTERNALVOLTAGE);
}

uint16_t FSR408::readResistance(void)
{
	//Read the voltage of the sensor
	uint16_t vSensor = readVoltage();
	
	if(vSensor == 0)
	{
		//There are no voltage! The sensor has "infinite" resistance! Return integer max value
		return 65535;
	}
	else
	{
		//vSensor = vccSensor * (rPullDown / (rPullDown + rSensor))
		//so rSensor = rPullDown * ((vccSensor - vSensor) / vSensor)
		return (uint16_t)((float)_rPullDown * ((float)(_vccSensor - vSensor) / (float)vSensor));
	}
}

//Resistence - Force
//     80000 -    15
//     30000 -    20
//     10000 -    50
//      6000 -   100
//      3300 -   240
//      2000 -   500
//      1100 -  1000
//       750 -  2000
//       450 -  4000
//       300 -  7000
//       230 - 10000
float FSR408::readForce(void)
{
	//Read the resistence of the sensor
	uint16_t rSensor = readResistance();
	
	//Aproximate the force of the sensor using Datasheet graph
	if(rSensor > 30000)
	{
		return (float)(((double)rSensor * (double)(-0.0001) + (double)(23)) * (double)0.00981);
	}
	else if(rSensor > 10000)
	{
		return (float)(((double)rSensor * (double)(-0.0015) + (double)(65)) * (double)0.00981);
	}
	else if(rSensor > 6000)
	{
		return (float)(((double)rSensor * (double)(-0.0125) + (double)(175)) * (double)0.00981);
	}
	else if(rSensor > 3300)
	{
		return (float)(((double)rSensor * (double)(-0.0519) + (double)(411.1111)) * (double)0.00981);
	}	
	else if(rSensor > 2000)
	{
		return (float)(((double)rSensor * (double)(-0.2000) + (double)(900)) * (double)0.00981);
	}	
	else if(rSensor > 1100)
	{
		return (float)(((double)rSensor * (double)(-0.5556) + (double)(1611.111)) * (double)0.00981);
	}
	else if(rSensor > 750)
	{
		return (float)(((double)rSensor * (double)(-2.8571) + (double)(4142.857)) * (double)0.00981);
	}
	else if(rSensor > 450)
	{
		return (float)(((double)rSensor * (double)(-6.6667) + (double)(7000)) * (double)0.00981);
	}
	else if(rSensor > 300)
	{
		return (float)(((double)rSensor * (double)(-20.0000) + (double)(13000)) * (double)0.00981);
	}
	else
	{
		return (float)(((double)rSensor * (double)(-42.8571) + (double)(19857.14)) * (double)0.00981);
	}
}

//***************************************************************************//
//**                          Private functions                            **//
//***************************************************************************//
