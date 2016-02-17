/*
 * CpE301Lab3E4.c
 *
 * Created: 2/12/2016 3:55:02 PM
 * Author : jmsikorski
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		DDRD = 0xFF; //set all of portD to output
		while (1)
			PORTD = 0x54; //set PD2, PD4 and PD6 high
		return 0;
    }
}

