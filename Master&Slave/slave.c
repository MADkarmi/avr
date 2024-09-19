#define F_CPU 16000000L
#include<avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>


void spi_init()
{
	DDRB = ( 1 << PB4 );        //Miso as out
	SPCR = ( 1 << SPE );        //SPI on
	
}

unsigned char spi_slaver_transfer(unsigned char data)
{
	SPDR = data;
	while( ! bit_is_set( SPSR, SPIF ) );
	return(SPDR );
}



int main()
{
	
	spi_init();

	srand(time(0));

	int randomnum = rand()%99;
	
	unsigned char response = 0;
	unsigned char masterdata;
	while(1)
	{
		masterdata = spi_slaver_transfer(0); //gettin question
		if(masterdata > 0)
		{
			if(masterdata != randomnum)   //calculating answ
			{
				if(masterdata < randomnum) masterdata = spi_slaver_transfer(1);
				if(masterdata > randomnum) masterdata = spi_slaver_transfer(2);
			}
			else
			{
				masterdata = spi_slaver_transfer(3);
				randomnum=rand()%99;
			}
		}
		
		
		
	}                        
	return 0;
}