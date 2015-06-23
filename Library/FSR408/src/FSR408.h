/*
	FSR408.h - Library for FSR408 sensor.
	Version 1.0.0
	APica
	17/01/2015
*/
#ifndef FSR408_h
#define FSR408_h

#include "Arduino.h"

class FSR408
{
	public:
		//***************************************************************************//
		//**                          Constructor                                  **//
		//***************************************************************************//
		//FSR408
		//	Uses AN0 pin, 5000mv as VCC and 1500ohm as pullDown.
		FSR408(void);
		//FSR408
		//	Parameters:
		//		-pinSensor: Physical pin where the output of the sensor is connected.
		//		-vccSensor: Voltage of the sensor (in mV).
		//		-rPullDown: Resistance of the pull down resistor (in ohms).
		FSR408(int pinSensor, int vccSensor, int rPullDown);
		//***************************************************************************//
		//**                          Public functions                             **//
		//***************************************************************************//
		//readVoltage
		//	Return: Voltage readed on pinSensor (in mV).
		uint16_t readVoltage(void);
		//readResistance
		//	Return: Resistance of the sensor (in Ohms).
		uint16_t readResistance(void);
		//readForce
		//	Return: Force applied to the sensor (in Newtons).
		float readForce(void);
		//***************************************************************************//
		//**                          Public variables                             **//
		//***************************************************************************//
	private:
		//***************************************************************************//
		//**                          Private functions                            **//
		//***************************************************************************//
		//***************************************************************************//
		//**                          Private variables                            **//
		//***************************************************************************//
		int _pinSensor;
		int _vccSensor;
		int _rPullDown;
};

#endif