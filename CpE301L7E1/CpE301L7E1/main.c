/*
 * CpE301L7E1.c
 *
 * Created: 3/30/2016 11:32:44 AM
 * Author : jmsikorski
 */ 

#include <avr/io.h>
#include <string.h>

void usart_init();
void usart_send(char*);

int main(void)
{
    /* Replace with your application code */
	usart_init();
	char str[38] = "CpE301-Microcontroller System Design\n\r";

    while (1) 
    {
		usart_send(str);		
    }
}

void usart_init()
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0) | (1<<UMSEL00); // TX Enable, RX Enable, RX Interrupt enabled
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8 bit data segment
	UBRR0 = 0x33; // Baud rate
}

void usart_send (char* data)
{
	for(int i = 0; i < strlen(data); i++)
	{
		while (! (UCSR0A & (1<<UDRE0)));
		UDR0 = data[i];
	}
	return;
}