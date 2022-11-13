
#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#define F_CPU 1600000UL

#define	LCD_PRT	PORTC		
#define	LCD_DDR	DDRC			
#define	LCD_PIN	PINC			
#define	LCD_RS	0				
#define	LCD_RW  1				
#define	LCD_EN	2				

/* function set: set interface data length and number of display lines */
#define LCD_FUNCTION_4BIT_1LINE  0x20                                           
#define LCD_FUNCTION_4BIT_2LINES 0x28                                           
#define LCD_FUNCTION_8BIT_1LINE  0x30                                           
#define LCD_FUNCTION_8BIT_2LINES 0x38                                           

/* display on/off, cursor on/off, blinking char at cursor position */
#define LCD_DISP_OFF             0x08                                           
#define LCD_DISP_ON              0x0C                                           
#define LCD_DISP_ON_BLINK        0x0D                                           
#define LCD_DISP_ON_CURSOR       0x0E                                           
#define LCD_DISP_ON_CURSOR_BLINK 0x0F                                           

/* Some useful commands (Execution time of this commands = 2ms) */
#define CLEAR_DISPLAY            0x01
#define RETURN_HOME              0x02

/* set entry mode: display shift on/off, dec/inc cursor move direction */
#define LCD_ENTRY_DEC            0x04                                           
#define LCD_ENTRY_DEC_SHIFT      0x05                                           
#define LCD_ENTRY_INC_           0x06                                           
#define LCD_ENTRY_INC_SHIFT      0x07                                           

/* move cursor/shift display */
#define LCD_MOVE_CURSOR_LEFT     0x10                                           
#define LCD_MOVE_CURSOR_RIGHT    0x14                                           
#define LCD_MOVE_DISP_LEFT       0x18                                           
#define LCD_MOVE_DISP_RIGHT      0x1C                                           

//******************************************************************************

void lcdCommand(unsigned char cmnd);
void lcdData (unsigned char data);
void lcd_init();
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_print(char *str);
void lcd_clear(void);
void lcd_generate(unsigned char location, unsigned char *ptr);
void lcd_cursor_on(void);
void lcd_cursor_off(void);
void lcd_move_cursor_left(void);
void lcd_move_cursor_right(void);

#endif /* LCD_H_ */