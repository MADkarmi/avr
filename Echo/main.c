/*
 * task3.c
 *
 * Created: 2017-12-07 21:29:04
 * Author : Radoslaw Madzia
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


int main (void)
{
	//Usart init
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	UCSR0A = 0;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); 
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
	UCSR0B |= (1 << RXCIE0 ); // Enable the USART Recieve Complete interrupt 
	sei(); 
	
	
	
	power_adc_disable();
	power_timer0_disable();
	power_timer1_disable();
	power_timer2_disable();
	power_twi_disable();
	power_spi_disable();
	
	while(1){
		sleep_cpu();
	}


}
ISR(USART_RX_vect)
{
	volatile char ReceivedByte;
	ReceivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	UDR0 = ReceivedByte; // Echo back the received byte back to the computer	
}
