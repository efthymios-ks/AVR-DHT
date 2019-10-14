//Headers
#include <util/delay.h>
#include "DHT.h"

int main(void)
{
	//Variables
	double temperature[1];
	double humidity[1];

	//Setup
	DHT_Setup();
	
	//Loop
	while (1 == 1)
	{
		//Read from sensor
		DHT_Read(temperature, humidity);
		
		//Check status
		switch (DHT_Status())
		{
			case (DHT_Ok):
				//Print temperature
				//print(temperature[0]);
	
				//Print humidity
				//print(humidity[0]);	
				break;
			case (DHT_Error_Checksum):
				//Do something
				break;
			case (DHT_Error_Timeout):
				//Do something else
				break;
		}
				
		//Sensor needs 1-2s to stabilize its readings
		_delay_ms(1000);
	}
	
	return 0;
}
