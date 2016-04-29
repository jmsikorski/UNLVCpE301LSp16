/*
 * CpE301L9E1.c
 *
 * Created: 4/15/2016 4:09:57 PM
 * Author : jmsikorski
 */ 
#define F_CPU 8000000UL
#include <avr/io.h> //standard AVR header
#include <util/delay.h> //delay header
#define LCD_DPRT PORTH //LCD DATA PORT
#define LCD_DDDR DDRH //LCD DATA DDR
#define LCD_DPIN PINH //LCD DATA PIN
#define LCD_CPRT PORTB //LCD COMMANDS PORT
#define LCD_CDDR DDRB //LCD COMMANDS DDR
#define LCD_CPIN PINB //LCD COMMANDS PIN
#define LCD_RS 0 //LCD RS
#define LCD_RW 1 //LCD RW
#define LCD_EN 2 //LCD EN


//****************************************************************

//****************************************************************
void lcdCommanda (unsigned char cmnd)
{
	LCD_DPRT = cmnd; //send cmnd to data port
	LCD_CPRT &= ~(1<<LCD_RS); //RS = 0 for command
	LCD_CPRT &= ~(1<<LCD_RW); //RW = 0 for write
	LCD_CPRT |= (1<<LCD_EN); //EN = 1 for H-to-L pulse
	_delay_us(1); //wait to make enable wide
	LCD_CPRT &= ~(1<<LCD_EN); //EN = 0 for H-to_L pulse
	_delay_us(100); //wait to make enable wide
}
void lcdData(unsigned char data)
{
	LCD_DPRT = data; //send data to data port
	LCD_CPRT |= (1<<LCD_RS); //RS = 1 for data
	LCD_CPRT &= ~(1<<LCD_RW); //RW = 0 for write
	LCD_CPRT |= (1<<LCD_EN); //EN = 1 for H-to-L pulse
	_delay_us(1); //wait to make enable wide
	LCD_CPRT &= ~(1<<LCD_EN); //EN = 0 for H-to_L pulse
	_delay_us(100); //wait to make enable wide
}
void lcd_init()
{
	LCD_DDDR = 0xFF;
	LCD_CDDR |= (1<<LCD_RS) | (1<<LCD_RW) | (1<<LCD_EN);
	LCD_CPRT &=~(1<<LCD_EN); //LCD_EN = 0
	_delay_us(2000); //wait for init
	lcdCommanda(0x38); //inti. LCD 2 line, 5x7
	lcdCommanda(0x0E); //display on, cursor on
	lcdCommanda(0x01); //clear LCD
	_delay_us(2000); //wait
	lcdCommanda(0x06); //shift cursor right
}
void lcd_gotoxy(unsigned char x, unsigned char y)
{
	unsigned char firstCharAdr[] = {0x80, 0xC0};
	lcdCommanda(firstCharAdr[y-1] + x -1);
	_delay_us(100);
}
void lcd_print(char * str)
{
	unsigned char i = 0;
	while (str[i]!=0)
	{
		lcdData(str[i]);
		i++;
	}
}

int main(void)
{		
	lcd_init();
	lcd_gotoxy(1,1);
	lcd_print("Sikorski, Jason");
	lcd_gotoxy(1,2);
	lcd_print(" CpE301: Lab 9  ");
	return 0;
}
