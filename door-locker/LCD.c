

#include "LCD.h"
#include <util/delay.h>


//*************************************************
void delay_us(int d)
{
	while(d--) 
	{
		_delay_us(1);
	}
}
//*************************************************
void delay_ms(int d)
{
	while(d--)
	{
		_delay_ms(1);
	}
}

void lcd_reset(void)
{
	LCD_DDR = 0xFF;					               
	LCD_PRT &= ~(1<<LCD_EN);			           
	delay_us(20000);				               
	lcdCommand(0x03);				               
	delay_us(10000);				               
    lcdCommand(0x03);								
	delay_us(1000);									
    lcdCommand(0x03);								
	delay_us(1000);                                
    lcdCommand(0x02);								
	delay_us(1000);                                
}

void lcd_init(void)
{       
    //lcd_reset();
	LCD_DDR = 0xFF;
	delay_us(2000);
    /* Function set */
    lcdCommand(0x33);							   
	delay_us(100);                                 
        
    /* Display ON/OFF Control */
	lcdCommand(0x32);                              
	delay_us(100);									
        
   
	lcdCommand(LCD_FUNCTION_4BIT_2LINES);
	delay_us(100);					               
	
	lcdCommand(LCD_DISP_ON);
	delay_us(100);
	
	lcdCommand(CLEAR_DISPLAY);
	delay_us(2000);									
        
    /* Entry mode set */
	lcdCommand(LCD_ENTRY_INC_);						
	delay_us(100);                                 
}

void lcdCommand(unsigned char cmd)
{
	
	LCD_PRT = (cmd & 0xF0);
	LCD_PRT &= ~(1<<LCD_RS);
	LCD_PRT &= ~(1<<LCD_RW);
	LCD_PRT |= (1<<LCD_EN);
	delay_ms(10);
	LCD_PRT &= ~(1<<LCD_EN);
	delay_ms(10);
	LCD_PRT = ((cmd<<4) & 0xF0);
	LCD_PRT &= ~(1<<LCD_RS);
	LCD_PRT &= ~(1<<LCD_RW);
	LCD_PRT |= (1<<LCD_EN);
	delay_ms(10);
	LCD_PRT &= ~(1<<LCD_EN);
	delay_ms(10);		               
}

void lcdData (unsigned char data)
{
	LCD_PRT = (data & 0xF0);
	LCD_PRT |= (1<<LCD_RS);
	LCD_PRT &= ~(1<<LCD_RW);
	LCD_PRT |= (1<<LCD_EN);
	delay_ms(10);
	LCD_PRT &= ~(1<<LCD_EN);
	delay_ms(10);
	LCD_PRT = ((data<<4) & 0xF0);
	LCD_PRT |= (1<<LCD_RS);
	LCD_PRT &= ~(1<<LCD_RW);
	LCD_PRT |= (1<<LCD_EN);
	delay_ms(10);
	LCD_PRT &= ~(1<<LCD_EN);
	delay_ms(10);				   
}

void lcd_gotoxy(unsigned char x, unsigned char y)
{
	unsigned char firstcharAdr[] = {0x80, 0xC0, 0x94, 0xD4};
	lcdCommand(firstcharAdr[y-1] + x - 1);
	delay_us(100);
}

void lcd_cursor_on(void)
{
	lcdCommand(LCD_DISP_ON_CURSOR_BLINK);
	delay_us(100);
}

void lcd_cursor_off(void)
{
	lcdCommand(LCD_DISP_ON);
	delay_us(100);
}
void lcd_move_cursor_left(void)
{
	lcdCommand(LCD_MOVE_CURSOR_LEFT);
	delay_us(100);
}

void lcd_move_cursor_right(void)
{
	lcdCommand(LCD_MOVE_CURSOR_RIGHT);
	delay_us(100);
}


void lcd_print(char *str)
{
	unsigned char i = 0;
	
	while(str[i] != 0)
	{
		lcdData(str[i]);
		i++;
	}
}

void lcd_clear(void)
{
	lcdCommand(CLEAR_DISPLAY);
	delay_us(2000);
	lcd_gotoxy(1,1);
}
 
void lcd_generate(unsigned char location, unsigned char *ptr)
{
	unsigned char i;
	if(location<8)
	{
		lcdCommand(0x40+(location*8));
		for(i=0;i<8;i++)
		{
			lcdData(ptr[i]);	
		}	
	}
}
