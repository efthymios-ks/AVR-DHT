#include "DHT.h"

//----- Auxiliary data ----------//
enum DHT_Status_t __DHT_STATUS;

#if (DHT_Type == DHT11)
	#define __DHT_Temperature_Min	0
	#define __DHT_Temperature_Max	50
	#define __DHT_Humidity_Min		20
	#define __DHT_Humidity_Max		90
	#define __DHT_Delay_Read		50
#elif (DHT_Type == DHT22)
	#define __DHT_Temperature_Min	-40
	#define __DHT_Temperature_Max	80
	#define __DHT_Humidity_Min		0
	#define __DHT_Humidity_Max		100
	#define __DHT_Delay_Read		20
#endif
//-------------------------------//

//----- Prototypes ----------------------------//
static double ExtractTemperature(uint8_t Data3, uint8_t Data4);
static double ExtractHumidity(uint8_t Data1, uint8_t Data2);
//---------------------------------------------//

//----- Functions -----------------------------//
//Setup sensor.
void DHT_Setup()
{
	_delay_ms(__DHT_Delay_Setup);
	__DHT_STATUS = DHT_Ok;
}

//Get sensor status.
enum DHT_Status_t DHT_GetStatus()
{
	return (__DHT_STATUS);
}

//Read raw buffer from sensor.
enum DHT_Status_t DHT_ReadRaw(uint8_t Data[4])
{
	uint8_t buffer[5] = {0, 0, 0, 0, 0};
	uint8_t retries, i;
	int8_t j;
	__DHT_STATUS = DHT_Ok;
	retries = i = j = 0;

	//----- Step 1 - Start communication -----
	if (__DHT_STATUS == DHT_Ok)
	{
		//Request data
		DigitalWrite(DHT_Pin, Low);			//DHT_PIN = 0
		PinMode(DHT_Pin, Output);			//DHT_PIN = Output
		_delay_ms(__DHT_Delay_Read);

		//Setup DHT_PIN as input with pull-up resistor so as to read data
		DigitalWrite(DHT_Pin, High);		//DHT_PIN = 1 (Pull-up resistor)
		PinMode(DHT_Pin, Input);			//DHT_PIN = Input

		//Wait for response for 20-40us
		retries = 0;
		while (DigitalRead(DHT_Pin))
		{
			_delay_us(2);
			retries += 2;
			if (retries > 60)
			{
				__DHT_STATUS = DHT_Error_Timeout;	//Timeout error
				break;
			}
		}
	}
	//----------------------------------------

	//----- Step 2 - Wait for response -----
	if (__DHT_STATUS == DHT_Ok)
	{
		//Response sequence began
		//Wait for the first response to finish (low for ~80us)
		retries = 0;
		while (!DigitalRead(DHT_Pin))
		{
			_delay_us(2);
			retries += 2;
			if (retries > 100)
			{
				__DHT_STATUS = DHT_Error_Timeout;	//Timeout error
				break;
			}
		}
		//Wait for the last response to finish (high for ~80us)
		retries = 0;
		while(DigitalRead(DHT_Pin))
		{
			_delay_us(2);
			retries += 2;
			if (retries > 100)
			{
				__DHT_STATUS = DHT_Error_Timeout;	//Timeout error
				break;
			}
		}
	}
	//--------------------------------------

	//----- Step 3 - Data transmission -----
	if (__DHT_STATUS == DHT_Ok)
	{
		//Reading 5 bytes, bit by bit
		for (i = 0 ; i < 5 ; i++)
			for (j = 7 ; j >= 0 ; j--)
			{
				//There is always a leading low level of 50 us
				retries = 0;
				while(!DigitalRead(DHT_Pin))
				{
					_delay_us(2);
					retries += 2;
					if (retries > 70)
					{
						__DHT_STATUS = DHT_Error_Timeout;	//Timeout error
						j = -1;								//Break inner for-loop
						i = 5;								//Break outer for-loop
						break;								//Break while loop
					}
				}

				if (__DHT_STATUS == DHT_Ok)
				{
					//We read data bit || 26-28us means '0' || 70us means '1'
					_delay_us(35);							//Wait for more than 28us
					if (DigitalRead(DHT_Pin))				//If HIGH
						BitSet(buffer[i], j);				//bit = '1'

					retries = 0;
					while(DigitalRead(DHT_Pin))
					{
						_delay_us(2);
						retries += 2;
						if (retries > 100)
						{
							__DHT_STATUS = DHT_Error_Timeout;	//Timeout error
							break;
						}
					}
				}
			}
	}
	//--------------------------------------


	//----- Step 4 - Check checksum and return data -----
	if (__DHT_STATUS == DHT_Ok)
	{
		if (((uint8_t)(buffer[0] + buffer[1] + buffer[2] + buffer[3])) != buffer[4])
		{
			__DHT_STATUS = DHT_Error_Checksum;	//Checksum error
		}
		else
		{
			//Build returning array
			//data[0] = Humidity		(int)
			//data[1] = Humidity		(dec)
			//data[2] = Temperature		(int)
			//data[3] = Temperature		(dec)
			//data[4] = Checksum
			for (i = 0 ; i < 4 ; i++)
				Data[i] = buffer[i];
		}
	}
	//---------------------------------------------------

	return DHT_GetStatus();
}

//Read temperature in Celsius.
enum DHT_Status_t DHT_GetTemperature(double *Temperature)
{
	double *waste = 0;
	return DHT_Read(Temperature, waste);
}

//Read humidity percentage.
enum DHT_Status_t DHT_GetHumidity(double *Humidity)
{
	double *waste = 0;
	return DHT_Read(waste, Humidity);
}

//Read temperature and humidity.
enum DHT_Status_t DHT_Read(double *Temperature, double *Humidity)
{
	uint8_t data[4] = { 0, 0, 0, 0 };

	//Read data
	enum DHT_Status_t status =  DHT_ReadRaw(data);

	//If read successfully
	if (status == DHT_Ok)
	{
		//Calculate values
		*Temperature = ExtractTemperature(data[2], data[3]);
		*Humidity = ExtractHumidity(data[0], data[1]);

		//Check values
		if ((*Temperature < __DHT_Temperature_Min) || (*Temperature > __DHT_Temperature_Max))
			__DHT_STATUS = DHT_Error_Temperature;
		else if ((*Humidity < __DHT_Humidity_Min) || (*Humidity > __DHT_Humidity_Max))
			__DHT_STATUS = DHT_Error_Humidity;
	}

	return DHT_GetStatus();
}

//Convert temperature from Celsius to Fahrenheit.
double DHT_CelsiusToFahrenheit(double Temperature)
{
	return (Temperature * 1.8 + 32);
}

//Convert temperature from Celsius to Kelvin.
double DHT_CelsiusToKelvin(double Temperature)
{
	return (Temperature + 273.15);
}

//Convert temperature data to double temperature.
static double ExtractTemperature(uint8_t Data2, uint8_t Data3)
{
	double temp = 0.0;

	#if (DHT_Type == DHT11)
		temp = Data2;
	#elif (DHT_Type == DHT22)
		//(Integral<<8 + Decimal) / 10
		temp = (BitCheck(Data2, 7) ? ((((Data2 & 0x7F) << 8) | Data3) / (-10.0)) : (((Data2 << 8) | Data3) / 10.0));
	#endif

	return temp;
}

static double ExtractHumidity(uint8_t Data0, uint8_t Data1)
{
	double hum = 0.0;

	#if (DHT_Type == DHT11)
		hum = Data0;
	#elif (DHT_Type == DHT22)
		//(Integral<<8 + Decimal) / 10
		hum = ((Data0<<8) | Data1) / 10.0;
	#endif

	return hum;
}
//---------------------------------------------//
