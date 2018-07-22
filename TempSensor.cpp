
#include "TempSensor.h"

TempSensor::TempSensor()
{
	portx = &PORTC;
	pinx = 4;
	ddrx = &DDRC;
	inputPinx = &PINC;
}

//NAME: TempSensor (Parameterized constructor)
//PURPOSE: set data direction register, data port, data pin, and data read register to user defined values
//INPUT: the address of the DDRx, PORTx, PINx registers and the pin number 
//OUTPUT: none
TempSensor::TempSensor(volatile uint8_t *_port, volatile uint8_t *_ddr, volatile uint8_t *_input_pin, uint8_t _pin)
{
	portx = _port;
	pinx = _pin;
	ddrx = _ddr;
	inputPinx = _input_pin;
}



bool TempSensor::startCommSequence()
{
	*ddrx |= (1 << pinx);
	//pull the comm line high
	*portx |= (1 << pinx);
	//wait for a bit to start a new sequence
	_delay_ms(250);

	//begin MCU start sequence
	PORTC &= ~(1 << PC4);
	_delay_ms(20);
	PORTC |= (1 << PC4);
	_delay_us(40);

	//listen for DHT response
	//change comm pin to input
	DDRC &= ~(1 << PC4);

	//enable pull-up resistor
	PORTC |= (1 << PC4);

	//wait for dht to pull pin low
	_delay_us(10);

	if ((*inputPinx & (1 << PC4)) == 0x00)
	{
		_delay_us(80);

		if ((PINC & (1 << PC4)) == 0x10)
		{
			_delay_us(80);
			return true;
		}
	}
	//if the sequence failed return false
	return false;
}

uint8_t TempSensor::Read()
{
	//start the comm sequence
	bool startSequence = startCommSequence();

	//begin time critical code, disable interrupts
	cli();

	for (uint8_t i = 0; i < 40; i++)
	{
		
		//if the  comm line goes low wait for it to go high then record the lenght of the pulse
		if ((PINC & (1 << PC4)) == 0)
		{
			
			//delay for the low pulse between bits
			_delay_us(50);

			//a little extra delay for the dht to change the pulse
			_delay_us(10);

			//reset the timer
			TCNT0 = 0;
			
			while ((PINC & (1 << PC4)) != 0x00)
			{
				//let the timer run
				count++;
				
			}
			//record timer0 value in array
			rawData[i] = count;
			count = 0;
		}


	}
		//decifer the pulses
		DecipherPulses();

		//re-enable interrupts
		sei();
		return temperature;

	
}

void TempSensor::DecipherPulses()
{
	int arrayIndex = 7;
	float base10Num = 0;

	//loop through the raw data and change each value to 1 or 0
	for (int k = 0; k < 40; k++)
	{
		if (rawData[k] < 50)
		{
			rawData[k] = 0;
		}
		else if (rawData[k] > 50)
		{
			rawData[k] = 1;
		}
		//Serial.println(rawData[k]);
	}
	//Serial.println("end");
	//the raw data array has 4 sets of 8 bit integers, set a loop for each set
	for (int i = 0; i < 4; i++)
	{
		//  Serial.print(i);
		//  Serial.println(": ");
		//loop through each set of 8 bits, add to make a base 10 number
		uint8_t l = (i * 8) + 8;
		for (int j = (i * 8); j < l; j++)
		{
			base10Num = base10Num + (rawData[j] * (power(arrayIndex, 2)));
			
			arrayIndex--;
		}
		if (i == 0)
		{
			humidity = base10Num;
		}
		else if (i == 2)
		{

			temperature = base10Num;
		}
		base10Num = 0;
		arrayIndex = 7;
	}
	//if the value is less then 60us its a 0, otherwise its a 1

	//record equivelent value in the data array
}

uint8_t TempSensor::power(uint8_t _base, uint8_t _power)
{
	uint8_t multiplier = _base;
	for (uint8_t i = 0; i < _power - 1; i++)
	{
		_base = _base*multiplier;
	}

	return _base;
}

uint8_t TempSensor::GetTemp()
{
	return temperature;
}

uint8_t TempSensor::GetHumidity()
{
	return humidity;
}


TempSensor::~TempSensor()
{
}
