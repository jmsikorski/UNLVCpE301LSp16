/*
 * CpE301L3E2.c
 *
 * Created: 2/12/2016 2:58:04 PM
 * Author : jmsikorski
 */ 

#include <avr/io.h>


int main(void)
{
	DDRD = 0xFF; //set all of portB to output
	while (1)
		PORTD = 0x88; //set PB7 and PB3 high
	return 0;
}

