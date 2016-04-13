/*
 * CpE301L7E1.c
 *
 * Created: 3/30/2016 11:32:44 AM
 * Author : jmsikorski
 */ 

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>

void usart_init();
void usart_send(char);
volatile char a;
char recd = 0;

ISR(USART1_RX_vect)
{
	a = UDR1;
	if(a > 64 && a < 91)
		a += 32;
	else if (a > 96 & a < 123)
		a -= 32;
	usart_send(a);
}

int main(void)
{
    /* Replace with your application code */
	usart_init();
	sei();
	char a = 'A';
    while (1)
	{
		a++;
		usart_send(a);
	}
}

void usart_init()
{
	UCSR1B = (1<<TXEN1) | (1<<RXEN1) | (1<<RXCIE1); // TX Enable, RX Enable, RX Interrupt enabled
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10); // 8 bit data segment
	UBRR1 = 0x33; // Baud rate
}

void usart_send (char data)
{
	while (! (UCSR1A & (1<<UDRE1)));
	UDR1 = data;
	return;
}