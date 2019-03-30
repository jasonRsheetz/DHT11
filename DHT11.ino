//declare start sequence time variables
uint8_t mcuStartHigh = 40;
uint8_t mcuStartLow = 20;
uint8_t dhtStartHigh = 80;
uint8_t dhtStartLow = 80;

//declare arrays
uint8_t rawData[40];
uint8_t decipheredData[40]

////////////////ENTER MAIN FUNCTION////////////////
int main(void)
{
//init timer 0
//use PD7 for the comm line, set to output initially high
DDRD |= (1 << PD7);
PORTD |= (1 << PD7);

////////////////ENTER MAIN LOOP////////////////
  while(1)
  {
    
  }
}

bool StartComm()
{
  //pull the comm line high
PORTD |= (1 << PD7);
  //wait for a bit to start a new sequence
_delay_ms(100);

  //start MCU start sequence
PORTD &= ~(1 << PD7);
_delay_us(mcuStartLow);
PORTD |= (1 << PD7);
_delay_us(mcuStartHigh);

  //listen for DHT response
  //change comm pin to input
  DDRD &= ~(1 << PD7)

  //wait for dht to pull pin low
  _delay_us(10);
  
  if((PIND & (1 << PD7) == 0)
  {
    delay_us(dhtStartLow);

    if((PIND & (1 << PD7) == 1)
    {
      _delay_us(dhtStartHigh);
      return true;
    }
  }

  //if the sequence failed return false
  return false;
}

uint8_t Read()
{
  //start the comm sequence
bool startSequence = StartComm();
  //add code to detect failed start sequence!!!!!!!!!!!!!!!!!!!!!!!!

  //begin time critical code, disable interrupts
  cli();
  
  //if the  comm line goes low wait for it to go high then record the lenght of the pulse

  //set timer0 to 0

  //wait for the DHT to pull comm line low again

  //record timer0 value in array

  //decipher pulses
}

void DecipherPulses(void)
{
  //look at the ith element of the array

  //if the value is less then 60us its a 0, otherwise its a 1

  //record equivelent value in the data array
}

