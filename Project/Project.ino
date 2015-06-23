/*
	Test for FSR408_Acquisition library.
	ApplicationHug added.
	Version 0.0.4 APica/PePo 03/06/2015 - moved FSR408-callback to ApplicationBonny, where it belongs!
	Version 0.0.3 PePo 18/05/2015 -still one compiler error left!
              Project.ino:14:1: error: expected ',' or ';' before 'void'
              Error compiling.
	Version 0.0.2 APica/PePo 12/05/2015 
	Version 0.0.1 APica 02/03/2015
*/

#include "Project.h"
#include <Wire.h>

void setup(void)
{
	//Serial output at 9600 baud
	Serial.begin(115200);// (9600);

        //TODO: DEBUG, waits for a serial input char
        //while(!Serial.available() > 0); Serial.read();
	
	//Initialize the main application
        ApplicationBonny_Init();
	
}

void loop(void)
{        
	//Application motor
	ApplicationBonny_Motor();
	
	//Sensor motors
	FSR408Acquisition_Motor();
	Actions_Motor();
	Movements_Motor();
	//TODO: ...
}
