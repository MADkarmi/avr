/*
 * Motor.c
 *
 * Created: 2018-01-15 07:44:51
 * Author : RadoslawMadzia
 */ 

#define F_CPU 16000000L 
#include <avr/io.h>
#include <util/delay.h>

uint8_t k_lock;
uint8_t rotationL = 1, rotationR = 0;

void readButton(uint8_t * lock, volatile uint8_t * PIN, unsigned key, void (*funkcja)(void) ) {
	
	if( !*lock && !(*PIN & (1<<key) )) {
		*lock=1;
		if(funkcja) funkcja(); // reakcja na wciniêcie przycisku
	} else if( *lock && (*PIN & (1<<key))) (*lock)+=15;
}


void change_rotation(void)
{
	rotationL=rotationL^rotationR;
	rotationR=rotationL^rotationR;
	rotationL=rotationR^rotationL;
	
}

void adc_init()
{
	ADMUX   = _BV(REFS0);
	DIDR0   = _BV(ADC0D); 
	ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); 
	ADCSRA |= _BV(ADEN); 
}

int readADC()
{
	ADCSRA |= _BV(ADSC);
	while (!(ADCSRA & _BV(ADIF)));
	ADCSRA |= _BV(ADIF);
	
	return ADC;
}
void pwm_init()
{
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) |_BV(WGM11);
	TCCR1B = _BV(WGM12) |_BV(WGM13) | _BV(CS11) | _BV(CS10);
	ICR1 = 249;
}

int main(void)
{
	adc_init();
	pwm_init();
	
	PORTB |= _BV(PB4) | _BV(PB3);
	DDRB = 0x0E;
	
    while (1) 
    {
		readButton(&k_lock, &PINB, PB4,change_rotation);
		
		if(rotationL == 1) 
		{
			OCR1A = (readADC()>>2); 
			OCR1B = 0x00;
		}
		else
			if(rotationR == 1)
			{
				OCR1B = (readADC()>>2); 
				OCR1A = 0x00;
			}
	
    }
}

