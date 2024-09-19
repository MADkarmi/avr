/*
 * sciemniacz.c
 *
 * Created: 2017-11-15 20:26:35
 * Author : Radoslaw Madzia
 * Wykorzystano fragmenty przyk³adowego programu ADC Pañskiego autorstwa
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>


#define BAUD 9600                          
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   

// inicjalizacja UART
void uart_init()
{
	UBRR0 = UBRR_VALUE;
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	UCSR0A = 0;
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
	while(!(UCSR0A & _BV(UDRE0)));
	UDR0 = data;
	return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
	while (!(UCSR0A & _BV(RXC0)));
	return UDR0;
}

// inicjalizacja ADC
void adc_init()
{
	ADMUX   = _BV(REFS0); // referencja AVcc, wejœcie ADC0
	DIDR0   = _BV(ADC0D); // wy³¹cz wejœcie cyfrowe na ADC0
	// czêstotliwoœæ zegara ADC 125 kHz (16 MHz / 128)
	ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
	ADCSRA |= _BV(ADEN); // w³¹cz ADC
}

FILE uart_file;

int main(void)
{	
	DDRD = 0x40;    
    TCCR0A |= (1<<WGM00)|(1<<WGM01)|(1<<COM0A1)|_BV(COM0A0)|(1<<COM0B1);
    TCCR0B =_BV(CS00);

	uart_init();
	fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
	stdin = stdout = stderr = &uart_file;
	adc_init();

	uint16_t adcArr[10];
	uint16_t avADC =0;
	while(1) {
		//uœredniamy pomiar
		for(uint8_t j = 0;j< 10;++j)
		{
			uint16_t adctmp = 0;
			uint16_t tmp[10];
			for(uint8_t i = 0; i< 10;++i)
			{
				ADCSRA |= _BV(ADSC); // wykonaj konwersjê
				while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
				ADCSRA |= _BV(ADIF); // wyczyœæ bit ADIF (pisz¹c 1!)
				tmp[i]= ADC;
				_delay_ms(1);
			}
			for(uint8_t i =0; i< 10;++i)
				adctmp +=tmp[i];
			//printf("tmp: %"PRIu16"\n", adctmp);
			adcArr[j] = adctmp/10;

		}
		for(uint8_t i =0; i< 10;++i)
			avADC += adcArr[i];
		avADC/=10;

		
		float vIn = 5 - ((avADC*5)/1024);
		float resistance = (5*10)/vIn - 10;
		int res = resistance*10;
		printf("Odczytano: %"PRIu16"\n", avADC*10/32);
		#if 1
				OCR0A = res;
		#endif
		#if 0 //lepszy efekt ale bez bezpoœredniej referencji do rezystancji (chocia¿...?) 
				OCR0A = avADC*10/32;
		#endif

		
	}
	
}

