/*
 * KodyGrey'a.c
 *
 * Created: 2017-10-18 22:59:45
 * Author : Radoslaw_Madzia
 */ 
#define F_CPU 9000UL 
#include <avr/io.h>
#include <util/delay.h>

uint8_t k7_lock,k6_lock,k5_lock;
uint8_t global_counter = 0;

void readButton(uint8_t * lock, volatile uint8_t * PIN, unsigned key, void (*funkcja)(void) ) {
	
	if( !*lock && !(*PIN & (1<<key) )) {
		*lock=1;
		if(funkcja) funkcja(); // reakcja na wciniêcie przycisku
	} else if( *lock && (*PIN & (1<<key))) (*lock)++;
}



void zeruj(void){
PORTB = 0x00;
global_counter = 0;
}

void next(void){
	if(global_counter == 15){ 
		PORTB = 0x00;	
		global_counter=0;
	}
	else{
		++global_counter;
		PORTB = (global_counter ^ (global_counter >> 1));
	}
}

void prev(void){
	if(global_counter == 0 ){ 
		PORTB = 0x08; 
		global_counter = 15;
	}
	else{
		--global_counter;
		PORTB = (global_counter ^ (global_counter >> 1));  
	}
	
}

int main(void)
{
    DDRD = 0x00;
	PORTD =0xF0; //enable pull_ups
	
	DDRB = 0X0F;
	PORTB = 0x0F; //confirm_blink
	_delay_ms(250);
	PORTB = 0x00;
	 _delay_ms(10);

    while (1) 
	{
		readButton(&k7_lock, &PIND, 0x07,zeruj);
		readButton(&k6_lock,&PIND,0x06,prev);
		readButton(&k5_lock,&PIND,0x05,next);	
    }
}
