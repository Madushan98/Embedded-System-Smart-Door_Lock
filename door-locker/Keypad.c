/*
 * Keypad.c
 *
 * Created: 11/6/2022 10:56:31 PM
 *  Author: premi
 */ 

#define F_CPU 1600000UL
#include <avr/io.h>
#include <util/delay.h>
#include "Keypad.h"

unsigned char keypad[4][4]={{'1','*','4','7'},
							{'2','0','5','8'},
							{'3','#','6','9'},
							{'A','D','B','C'}};


extern unsigned char get_key (void)
{
	unsigned char colloc, rowloc;
	
	KEY_DDR = 0xF0;
	KEY_PRT = 0xFF;
	KEY_PIN = 0x0F;
	
	do{
		KEY_PRT &= 0x0F;                                                  //ground all rows at once
		colloc = (KEY_PIN & 0x0F);                                        //read the columns
	} while(colloc != 0x0F);											    //check untill all keys are released
	do{
		do{
			_delay_ms(20);											   //Delay to allow the user to enter the required Key
			colloc = (KEY_PIN & 0x0F);                                    //read the columns
		} while(colloc != 0x0F);                                          //wait for key press
		
		_delay_ms(20);													     //Delay for debounce
		colloc = (KEY_PIN & 0x0F);                                          //read the columns
	} while(colloc != 0x0F);                                                //wait for key press
	
	while(1)
	{
		KEY_PRT = 0xEF;
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F)
		{
			rowloc = 0;
			break;
		}
		KEY_PRT = 0xDF;
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F)
		{
			rowloc = 1;
			break;
		}
		KEY_PRT = 0xBF;
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F)
		{
			rowloc = 2;
			break;
		}
		KEY_PRT = 0x7F;
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F)
		{
			rowloc = 3;
			break;
		}
	}
	
	if(colloc == 0x0E)
	return (keypad[rowloc][0]);
	else if(colloc == 0x0D)
	return (keypad[rowloc][1]);
	else if(colloc == 0x0B)
	return (keypad[rowloc][2]);
	else
	return (keypad[rowloc][3]);

}
