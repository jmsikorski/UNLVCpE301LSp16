/*
 * CpE301L8E3a.c
 *
 * Created: 4/12/2016 5:17:57 PM
 * Author : jmsikorski
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
int main()
{
	//PORTD pins as input
	DDRD = 0x00;
	//Enable internal pull ups
	PORTD = 0xFF;
	//Set PORTB1 pin as output
	DDRB = 0xFF;
	//TOP = ICR1;
	//output compare OC1A 8 bit non inverted PWM
	//Clear OC1A on Compare Match, set OC1A at TOP
	//Fast PWM
	//ICR1 = 20000 defines 50Hz pwm
	ICR1 = 20000;
		
	TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10); //TCCR1A = 0x82
	TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
	//start timer with prescaler 8
	OCR1A = 2140;
	// 2330 upper limit
	// 630 lower limit
	while (1)
	{
		OCR1A=2140;  //reset
		_delay_ms(1000); // 0 to 90
		OCR1A=1280;
		_delay_ms(2000); //delay 2 seconds		
		OCR1A=2140;  //reset
		_delay_ms(1000);
		OCR1A=1827; //move to 35degrees
		_delay_ms(1000);
		OCR1A=997; //move to 125degrees
		_delay_ms(2000); // delay 2 seconds		
		OCR1A=2140;  //reset
		_delay_ms(1000);
		OCR1A=490; //move to 180 degrees
		_delay_ms(1000);		
		OCR1A=2140; //move to 0 degrees
		_delay_ms(2000); // delay 2 seconds	
	}
}

