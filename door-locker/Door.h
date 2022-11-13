
#ifndef DOOR_H_
#define DOOR_H_

#include <avr/io.h>

#define DOOR_PRT PORTB
#define DOOR_DDR DDRB
#define DOOR_PIN PINB

void init_door(void);
void open_door(void);
void close_door(void);



#endif /* DOOR_H_ */