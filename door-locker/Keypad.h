/*
 * Keypad.h
 *
 * Created: 11/6/2022 10:55:20 PM
 *  Author: premi
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define     KEY_PRT     PORTL
#define     KEY_DDR     DDRL
#define     KEY_PIN     PINL

extern unsigned char get_key(void);

#endif /* KEYPAD_H_ */