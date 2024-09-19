/*
 * task2.c
 *
 * Created: 2017-12-08 01:38:18
 * Author : Radoslaw Madzia
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


int stan[100];
int count = 0;

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	TCNT1 = 64911;   	
	if(PINB & 0x01) stan[(count)] = 1;
	else stan[(count)] = 0;
	
	if(stan[(count + 55 )%100]) PORTB = 0x05;
	else PORTB = 0x01;
	if(count == 99) count = 0;
	else count +=1;

}

int main(void)
{
	DDRB = 0x04;
	PORTB = 0x01;
	for (int i = 0; i < 100; i++){stan[i] = 0;}
	TCNT1 = 64911;   // for 1/100 sec at 16 MHz

	TCCR1B =  (1<<CS12);// Timer mode CTC with 256 prescler
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
    while (1) 
    {
		sleep_cpu();
    }
}



