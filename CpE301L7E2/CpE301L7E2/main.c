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

ISR(USART_RX_vect)
{
	a = UDR0;
	usart_send(a);
}

int main(void)
{
    /* Replace with your application code */
	usart_init();
	sei();
    while (1);
}

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0) | (1<<UMSEL00); // TX Enable, RX Enable, RX Interrupt enabled
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8 bit data segment
	UBRR0 = 0x33; // Baud rate
}

void usart_send (char data)
{
	while (! (UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	return;
}