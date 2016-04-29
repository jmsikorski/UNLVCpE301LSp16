/*
 * CpE301L9E1.c
 *
 * Created: 4/15/2016 4:09:57 PM
 * Author : jmsikorski
 */ 
#define F_CPU 8000000UL
#include <avr/io.h> //standard AVR header
#include <util/delay.h> //delay header
#define LCD_DPRT PORTD //LCD DATA PORT
#define LCD_DDDR DDRD //LCD DATA DDR
#define LCD_DPIN PIND //LCD DATA PIN
#define LCD_CPRT PORTB //LCD COMMANDS PORT
#define LCD_CDDR DDRB //LCD COMMANDS DDR
#define LCD_CPIN PINB //LCD COMMANDS PIN
#define LCD_RS 0 //LCD RS
#define LCD_RW 1 //LCD RW
#define LCD_EN 2 //LCD EN

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

void set_key_prt(unsigned char p)
{
	PORTC |= (0x01 & p) | (0x02 & p) | (0x04 & p) | (0x08 & p) | (0x10 & p) | (0x20 & p);
	PORTB |= ((0x40 & p)>>3) | ((0x80 & p)>>3);
}

void set_key_ddr(unsigned char p)
{
	DDRC |= (0x01 & p) | (0x02 & p) | (0x04 & p) | (0x08 & p) | (0x10 & p) | (0x20 & p);
	DDRB |= ((0x40 & p)>>3) | ((0x80 & p)>>3);
}

void and_key_prt(unsigned char p)
{
	PORTC &= (0x01 & p) | (0x02 & p) | (0x04 & p) | (0x08 & p) | (0x10 & p) | (0x20 & p);
	PORTB &= ((0x40 & p)>>3) | ((0x80 & p)>>3);
}

int get_key_pin()
{
	int p = 0;
	p |= (0x01 & PINC) | (0x02 & PINC) | (0x04 & PINC) | (0x08 & PINC) | (0x10 & PINC) | (0x20 & PINC);
	p |= ((0x10 & PINB)<<3) | ((0x08 & PINB)<<3);
	return p;
}

//****************************************************************

//****************************************************************

int main(void)
{
	unsigned char rowloc;
	char data, colloc;
	set_key_ddr(0xF0);
	set_key_prt(0xFF);
	lcd_init();
	lcd_gotoxy(1,1);

	while(1)
	{
		do
		{
			and_key_prt(0x0F); //ground all rows at once
			colloc = (get_key_pin() & 0x0F); //read the columns
		} while (colloc != 0x0F); //check until all keys released
		do
		{
			do
			{
				_delay_ms(50); //call delay
				colloc = (get_key_pin() & 0x0F); //see if any key is pressed
			} while (colloc == 0x0F); //keep checking for key press
			_delay_ms(50); //call delay for debounce
			colloc = (get_key_pin() & 0x0F); //read columns
		} while (colloc == 0x0F); //wait for key press

		set_key_prt(0xEF);
		lcdData(get_key_pin() & 0x0F);
		set_key_prt(0xDF);
		lcdData(get_key_pin() & 0x0F);
		set_key_prt(0xBF);
		lcdData(get_key_pin() & 0x0F);
		set_key_prt(0x7F);
		lcdData(get_key_pin() & 0x0F);

		while(1)
		{
			lcdData('1');
			lcdData(get_key_pin());
			and_key_prt(0xEF); //ground row 1
			lcdData(get_key_pin());
			colloc = (get_key_pin() & 0x0F); //read the columns
			if (colloc != 0x0F) //column detected
			{
				rowloc = 0; //save row location
				break; //exit while loop
			}
			lcdData('2');
			lcdData(get_key_pin());
			set_key_prt(0xDF); //ground row 1
			lcdData(get_key_pin());
			colloc = (get_key_pin() & 0x0F); //read the columns
			if(colloc != 0x0F) //column detected
			{
				rowloc = 1; //save row location
				break; //exit while loop
			}
			lcdData('3');
			lcdData(get_key_pin());
			set_key_prt(0xBF); //ground row 1
			lcdData(get_key_pin());
			colloc = (get_key_pin() & 0x0F); //read the columns
			if(colloc != 0x0F) //column detected
			{
				rowloc = 2; //save row location
				break; //exit while loop
			}
			lcdData('4');
			lcdData(get_key_pin());
			set_key_prt(0x7F); //ground row 1
			lcdData(get_key_pin());
			colloc = (get_key_pin() & 0x0F); //read the columns
			if(colloc != 0x0F)
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