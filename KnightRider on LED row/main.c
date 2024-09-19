/*
 * zad1lista1.c
 *
 * Created: 2017-10-14 16:34:50
 * Author : Radoslaw_Madzia
 */ 



#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(){
	DDRB = DDRB | 0x3F;

	while(1){
		PORTB = 0x01;
		_delay_ms(132);
		for(char b=0x03;b<0x20;b=b<<1){
			PORTB = b; 
			_delay_ms(132);
		}
		for(char b=0x60;b>0x02;b=b>>1){
			PORTB = b; 
			_delay_ms(132);
		}

	}
}


