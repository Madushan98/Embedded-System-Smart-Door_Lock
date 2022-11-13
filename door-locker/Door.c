
#include "Door.h"

void init_door(){
	DOOR_DDR = 0x01;
	DOOR_PRT = 0x01;
}

void open_door(){
	DOOR_PRT = 0x00;
}

void close_door(){
	DOOR_PRT = 0x01;
}
