#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#define BAUD 9600                          
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   


void uart_init()
{
	UBRR0 = UBRR_VALUE;
	UCSR0A = 0;
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
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

FILE uart_file;

void spi_init()
{
	DDRB = ( 1 << PB5 ) | ( 1 << PB3 ) | ( 1 << PB2 );              //MOSI, SCK, SS' jako wyjœcia
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR1 ) | ( 1 << SPR0 );   //W³¹czamy SPI,
}                                                                               //uk³ad Master, najmniejsza czêstotliwoœæ SCK fosc / 128

void WinBlink()
{
	PORTD = 0xE0;
	_delay_ms(10);
	PORTD = 0x00;
}
void ToMuchBlink()
{
	PORTD = 0xC0;
	_delay_ms(10);
	PORTD = 0x00;
}
void NotEnoughBlink()
{
	
	PORTD = 0x60;
	_delay_ms(10);
	PORTD = 0x00;
}
unsigned char spi_master_transfer(unsigned char data)
{
	SPDR = data;                   
	while( ! bit_is_set( SPSR, SPIF ) ); 
	return(SPDR );      
}

int main()
{
	DDRD = 0xE0;
	char query = 40;
	char responce;

	uart_init();
	fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
	stdin = stdout = stderr = &uart_file;

	spi_init();

	while(1)
	{

		//data =scanf

		responce = spi_master_transfer(query);
		_delay_ms(500);
		responce = spi_master_transfer(0);
		switch(responce) //switch on slave response
		{
			case 2: printf(" dane które wysy³am %d , odpowiedŸ od slave %d za du¿o\n", query, responce); query--;break; //czytam dane
			case 1: printf("dane które wysy³am %d , odpowiedŸ od slave %d za ma³o\n",query, responce); query++;break; //czytam dane
			case 3: printf("zgad³es, nowa gra\n"); query = 1; WinBlink(); break;//czytam dane
		}
		_delay_ms(500);
	}
	return 0;
}

