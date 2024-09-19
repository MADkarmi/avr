/*
 * Grzalka.c
 *
 * Created: 2017-11-20 21:38:07
 * Author : Radoslaw Madzia
 * Wykorzystano fragmenty kodu do komunikacji Uart, oraz fragment przyk³adowej inicjalizacji ADC
 */ 


#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   

void uart_init()
{
	UBRR0 = UBRR_VALUE;
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	
	UCSR0A = 0;
}

int uart_transmit(char data, FILE *stream)
{
	while(!(UCSR0A & _BV(UDRE0)));
	UDR0 = data;
	return 0;
}

int uart_receive(FILE *stream)
{
	while (!(UCSR0A & _BV(RXC0)));
	return UDR0;
}

void adc_init()
{
	ADMUX   = _BV(REFS1) | _BV(REFS0); // referencja AVcc, wejœcie ADC0
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

   while(1) {	
		ADCSRA |= _BV(ADSC); 
		while (!(ADCSRA & _BV(ADIF)));
		ADCSRA |= _BV(ADIF); 
		uint16_t v = ADC; 
		printf("Odczytano temp: %"PRIu16" stopni C \n", (v-500)/10);
		if(v < 795)
			OCR0A = 0;
		if(v>800)
			OCR0A = 255;
	   
   }
}

