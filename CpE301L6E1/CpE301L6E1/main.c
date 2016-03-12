/*
 * CpE301L6E1.c
 *
 * Created: 3/11/2016 4:47:18 PM
 * Author : jmsikorski
 */ 

#define F_CPU 8000000UL	//XTAL = 4 MHz
#include <avr/io.h>
#include <util/delay.h>
int main ()
{
	unsigned char i;
	DDRD = 0x60;		//PD6 as output (OC0A)
	i = 1;
	OCR0A = 1;			//Duty cycle = 100%
	TCCR0A = 0xC1;		//Phase Correct PWM, inverted, N64
	TCCR0B = 0x0B;
	while (i != 0 )
	{
		OCR0A = i;
		_delay_ms(25);	//use AVR studio library delay
		i++;
	}
	while (1)
		DDRD = 0x60;			//wait here forever after dimming is done
	return 0;
}
