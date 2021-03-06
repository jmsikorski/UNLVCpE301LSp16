/*
 * CpE301L6E1.c
 *
 * Created: 3/11/2016 4:47:18 PM
 * Author : jmsikorski
 */ 

#define F_CPU 8000000UL	//XTAL = 8 MHz
#include <avr/io.h>
#include <util/delay.h>
int main ()
{
	unsigned char i;
	DDRB = 0x02;		//PD6 as output (OC0A)
	i = 26;
	OCR1A = 26;			//Duty cycle = 100%
	TCCR1A = 0xC1;		//Phase Correct PWM, inverted, N64
	TCCR1B = 0x02;
	while (1)
	{
		OCR1A = i;
		_delay_ms(25);	//use AVR studio library delay
		i++;
	}
	while (1);			//wait here forever after dimming is done
	return 0;
}
