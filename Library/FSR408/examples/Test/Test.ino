/*
	Test for FSR408 library.
	
	Connect the FSR408 sensor to the FSR408_PIN analog pin.
	Set the Vcc of the sensor to FSR408_VCC.
	Set the Pull Down resistor to FSR408_RPULLDOWN ohms.
*/

#include <FSR408.h>

//***************************************************************************//
//**                          Defines                                      **//
//***************************************************************************//
#define FSR408_PIN			(int)0			//Analog 0
#define FSR408_VCC			(int)5000		//5v
#define FSR408_RPULLDOWN		(int)1500		//1k5 ohms

//***************************************************************************//
//**                          Global variables                             **//
//***************************************************************************//
FSR408 fsr408(FSR408_PIN, FSR408_VCC, FSR408_RPULLDOWN);

//***************************************************************************//
//**                          Initial Setup                                **//
//***************************************************************************//
void setup()
{
	//Serial output at 9600 baud
	Serial.begin(9600);
}

//***************************************************************************//
//**                          Main Loop                                    **//
//***************************************************************************//
void loop()
{
	//Read the current voltage
	Serial.print("Voltage FSR408: ");
	Serial.print(fsr408.readVoltage());
	Serial.println(" (mV)");

	//Read the current resistance
	Serial.print("Resistance FSR408: ");
	Serial.print(fsr408.readResistance());
	Serial.println(" (ohms)");
	
	//Read the current force
	Serial.print("Force FSR408: ");
	Serial.print(fsr408.readForce());
	Serial.println(" (N)");
	
	Serial.println("--------------------");
	delay(250);
}

