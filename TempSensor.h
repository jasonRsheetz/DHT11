//#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "Wstring.h"

//#define dataRegister DDRC 
//#define dataPort PORTC 
//#define dataPin PC5 

class TempSensor
{
public:
	TempSensor();
	TempSensor(volatile uint8_t*, volatile uint8_t*, volatile uint8_t*, uint8_t);
	bool startCommSequence();
	uint8_t Read();
	void DecipherPulses();
	uint8_t power(uint8_t, uint8_t);
	uint8_t GetTemp();
	uint8_t GetHumidity();
	~TempSensor();

	//declare start sequence time variables
	uint8_t mcuStartHigh = 40;
	uint8_t mcuStartLow = 20;
	uint8_t dhtStartHigh = 80;
	uint8_t dhtStartLow = 80;
	uint8_t dhtBitDelay = 50;

private:

	uint8_t rawData[40];
	uint8_t temperature;
	int humidity;
	uint8_t count = 0;
	volatile uint8_t *portx;
	volatile uint8_t *ddrx;
	volatile uint8_t *inputPinx;
	uint8_t pinx;
	uint8_t mask;


};

