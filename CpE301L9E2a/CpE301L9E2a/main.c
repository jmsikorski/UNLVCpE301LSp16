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

#define KEY_PRT PORTA //keyboard PORT
#define KEY_DDR DDRA //keyboard DDR
#define KEY_PIN PINA //keyboard PIN
unsigned char keypad[ 4][ 4] ={ {'0','1','2','3'},
{'4','5','6','7'},
{'8','9','A','B'},
{'C','D','E','F'},
};

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


//****************************************************************

//****************************************************************

int main(void)
{
	unsigned char rowloc, colloc;
	char data;
	lcd_init();
	lcd_gotoxy(1,1);
	KEY_DDR = 0x0F;
	KEY_PRT = 0xFF;

	while(1)
	{
		do
		{
			KEY_PRT &= 0xF0; //ground all rows at once
			colloc = (KEY_PIN & 0xF0); //read the columns
		} while (colloc != 0xF0); //check until all keys released
		do
		{
			do
			{
				_delay_ms(50); //call delay
				colloc = (KEY_PIN & 0xF0); //see if any key is pressed
			} while (colloc == 0xF0); //keep checking for key press
			_delay_ms(50); //call delay for debounce
			colloc = (KEY_PIN & 0xF0); //read columns
		} while (colloc == 0xF0); //wait for key press

		while(1)
		{
			KEY_PRT = 0xFE; //ground row 1
			KEY_PIN;
			colloc = (KEY_PIN & 0xF0); //read the columns
			if (colloc != 0xF0) //column detected
			{
				rowloc = 0; //save row location
				break; //exit while loop
			}
			KEY_PRT = 0xFD; //ground row 1
			KEY_PIN;
			colloc = (KEY_PIN & 0xF0); //read the columns
			if(colloc != 0xF0) //column detected
			{
				rowloc = 1; //save row location
				break; //exit while loop
			}
			KEY_PRT = 0xFB; //ground row 1
			KEY_PIN;
			colloc = (KEY_PIN & 0xF0); //read the columns
			if(colloc != 0xF0) //column detected
			{
				rowloc = 2; //save row location
				break; //exit while loop
			}
			KEY_PRT = 0xF7; //ground row 1
			KEY_PIN;
			colloc = (KEY_PIN & 0xF0); //read the columns
			if(colloc != 0xF0)
			{
				rowloc = 3; //save row location
				break; //exit while loop
			}
			break;
		}
		//check column and send result to Port B
		if(colloc == 0xE0)
		data = (keypad[rowloc][0]);
		else if(colloc == 0xD0)
		data = (keypad[rowloc][1]);
		else if(colloc == 0xB0)
		data = (keypad[rowloc][2]);
		else
		data = (keypad[rowloc][3]);
		if(data != 0)
		{
			lcd_gotoxy(1,2);
			lcd_print("  ");
			lcdData(data);
			lcd_print(" was pressed  ");
			lcdCommanda(0x02);
		}
	}
	return 0;
}