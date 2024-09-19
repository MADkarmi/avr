/*
 * servo_tester.c
 *
 * Created: 2018-01-04 04:31:44
 * Author : Rodoslaw_Madzia
 */ 

#define F_CPU 16000000L 
#include <avr/io.h>
#include <util/delay.h>

#define ocr_min 1800
#define ocr_max 4000
#define ocr_mid (ocr_max+ocr_min)/2



void adc_init()
{
	ADMUX   = _BV(REFS0); 
	DIDR0   = _BV(ADC0D); 
	ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
	ADCSRA |= _BV(ADEN); 
}

void pwm_init()
{
    TCCR1A = _BV(WGM11) | _BV(COM1A1);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);
    ICR1 = 39999;
	TCNT1 = 0; 
}

int main(void)
{
	adc_init();
	pwm_init();
	DDRB |= _BV(PB1);

	OCR1A = ocr_max;
	_delay_ms(1000);
	OCR1A = ocr_mid;
	_delay_ms(1000);
	OCR1A = ocr_min;
	_delay_ms(1000);

	while(1) 
	{
		ADCSRA |= _BV(ADSC);
		while (!(ADCSRA & _BV(ADIF)));
		ADCSRA |= _BV(ADIF);
		uint16_t v = ADC;
		OCR1A = ocr_min + (v<<1);
	}

}

