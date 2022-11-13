
#define  F_CPU 1600000UL

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

#include "LCD.h"
#include "Keypad.h"
#include "Door.h"
#include "User.h"

int pinLength = 5;
int storePinCount;
unsigned char passwordroot[5]="12345";


void init (void);
void welcome_message (void);
void choose_list (void);
void log_in (void);
unsigned char get_userInput(unsigned char* pass,int isPassword);
unsigned char check_password(unsigned char len, unsigned char* password);
void correct_msg(void);
void try_again (void);
void try_again_userId (void);
void printsome(User	val);
void choose_list2 (void);
void add_user(void);
unsigned char compare(unsigned char* pass1, unsigned char* pass2, unsigned char len1, unsigned char len2);
unsigned char check_rootUser();
void root_user_login();
void initInterrupt(void);
int userAvailble(User *loginUser,char userId[]);
int checkUserPassword(char userPassword[],char enterdPassword[]);
int compareString(char val1[],char val2[]);

int main(void)
{
	init ();
	unsigned char choice = 0;
	
	uint16_t word = eeprom_read_word((uint16_t*)4000);
	eeprom_update_word((uint16_t*)4000,0);
	
	if(word	<= 0 || word == NULL){
		eeprom_update_word((uint16_t*)4000,0);
	}
	
    while (1) 
    {
		choice=0;
		welcome_message();
		get_key();
		do
		{
			choose_list();
			choice = get_key();
		} while (choice != 'A' && choice != 'B' );
		
		if(choice == 'A')
		{
			choice = 0;
			do
			{
				choose_list2();
				choice = get_key();
			} while (choice != 'A' && choice != 'B' );
			
			if(choice == 'A'){
				choice = 0;
				root_user_login();
			}else if (choice == 'B')
			{
				choice = 0;
				log_in();
			}
			
		}
		else if(choice == 'B')
		{
			choice = 0;
			add_user();
		}
    }
}

void init (void)
{
	lcd_init();
	init_door();
	
}

void initInterrupt(void){
	EICRA |= 0x03;
	EIMSK |= 0x01;
	sei();
}

void welcome_message (void)
{
	lcd_clear();
	lcd_print("Password-lock door");
	lcd_gotoxy(1,2);
	lcd_print("Welcome!");
}

void choose_list (void)
{
	lcd_clear();
	lcd_print("(A)Log-in");
	lcd_gotoxy(1,2);
	lcd_print("(B)Add User");
	
}

void choose_list2 (void)
{
	lcd_clear();
	lcd_print("(A)Root User");
	lcd_gotoxy(1,2);
	lcd_print("(B)Normal User");
	
}

void printsome(User val){
	lcd_clear();
	lcd_print(val.password);
}


unsigned char get_userInput(unsigned char* pass,int isPassword)
{
	int cnt = 0;
	unsigned char key = 0;
	
	lcd_cursor_on();
	
	for(cnt = 0; cnt <= 5 ; )
	{
		key = get_key();
		
		if(key == 'D')
		{
			break;
		}
		
		else if(key == 'C' && cnt > 0)
		{
			lcd_move_cursor_left();
			lcdData(' ');
			lcd_move_cursor_left();
			cnt --;
		}
		else if(key == 'C' && cnt <= 0)
		{
			cnt = 0;
		}
		else if(cnt != 5)
		{
			pass[cnt++] = key;
			if(isPassword){
				lcdData('*');	
			}else{
				lcdData(key);
			}
			
		}
	}

	lcd_cursor_off();
	
	return cnt;
}

void log_in (void)
{
	unsigned char entered_password[5] = {0};
	unsigned char entered_userId[5] = {0};				
	unsigned char length = 0;	
	User *user;							
	lcd_clear();
	lcd_print("Enter your userId: ");
	lcd_gotoxy(1,2);
	length = get_userInput(entered_userId,0);
	int result =  userAvailble(user,entered_userId);
	if(result == 0){
		try_again_userId();
		
	}else{
		lcd_clear();
		lcd_print("Enter Password");
		lcd_gotoxy(1,2);
		length = get_userInput(entered_password,1);
		
		if(checkUserPassword(user->password, entered_password))
		{
			open_door();
			correct_msg();
			_delay_ms(10000);
			close_door();
		}
		else
		{
			try_again();
		}
	}
	
	
}

int userAvailble(User *loginUser,char userId[]){
	
	int size = sizeof(User);
	User tempUser;
	uint16_t word = eeprom_read_word((uint16_t*)4000);
	for(unsigned char i = 0 ; i < word; i++){
		eeprom_read_block(&tempUser,size*i,size);
		int result = compareString(tempUser.userId,userId);
		if(result == 1){
			strcpy(loginUser->userId,tempUser.userId);
			strcpy(loginUser->password,tempUser.password);
			return 1 ;
		}
	}
	
	return 0;
	
}


int compareString(char val1[],char val2[]){
	
	for(int i = 0; i < 5; i++)
	{
		if(val1[i] != val2[i])
		{
			return 0 ;
		}
	}
	return 1 ;
	
}

int checkUserPassword(char userPassword[],char enterdPassword[]){
	int result = compareString(enterdPassword,userPassword);
	
	return result;
}

void correct_msg(void){
	lcd_clear();
	lcd_print("Door Unlocked");
	_delay_ms(20000);
}

void try_again (void){
	lcd_clear();
	lcd_print("Password is");
	lcd_gotoxy(1,2);
	lcd_print("Wrong");
	_delay_ms(8000);
}

void try_again_userId (void){
	lcd_clear();
	lcd_print("UserId is");
	lcd_gotoxy(1,2);
	lcd_print("Wrong");
	_delay_ms(8000);
}

unsigned char check_password(unsigned char len, unsigned char* password)
{
	unsigned char i = 0;				//Just a counter variable
	for(i = 0; i < len; i++)
	{
		if(password[i] != passwordroot[i])		//Password is stored in EEPROM from location 2 to 12
		{
			lcd_clear();
			lcd_print("break");
			_delay_ms(1000);
			return 0;
		}
	}
	return 1;
}

unsigned char check_rootUser(){
	unsigned char root_user_password[5] = {0};
	unsigned char length_1 = 0;
		
	lcd_clear();
	lcd_gotoxy(1,1);
	lcd_print("Root Password: ");
	lcd_gotoxy(1,2);
	length_1 = get_userInput(root_user_password,1);
	
	for(int i = 0;i<pinLength;i++){
		if(root_user_password[i] != passwordroot[i]){
			return 0;
		}
	}
	return 1;
	
}

void root_user_login(){
	unsigned char val = check_rootUser();
	if(val)
	{
		open_door();
		correct_msg();
		close_door();
	}
	else
	{
		try_again();
	}
}

void add_user(void){
	if(check_rootUser()){
		unsigned char user_id[5] = {0};
		unsigned char password_1[5] = {0};
		unsigned char password_2[5] = {0};
		
		unsigned char length_1 = 0;
		unsigned char length_2 = 0;
		unsigned char length_3 = 0;
		
		
		lcd_clear();
		lcd_gotoxy(1,1);
		lcd_print("Your User ID: ");
		lcd_gotoxy(1,2);
		length_1 = get_userInput(user_id,0);
		
		lcd_clear();
		lcd_gotoxy(1,1);
		lcd_print("Your Password: ");
		lcd_gotoxy(1,2);
		length_2 = get_userInput(password_1,1);
		
		lcd_clear();
		lcd_gotoxy(1,1);
		lcd_print("Re-enter Password: ");
		lcd_gotoxy(1,2);
		length_3 = get_userInput(password_2,1);
		
		if(compare(password_1,password_2,length_1,length_2))
		{
			uint16_t word = eeprom_read_word((uint16_t*)4000);
			User us = {{0},{0}};
			strcpy(us.userId,(char*)user_id);
			strcpy(us.password,(char*)password_1);
			eeprom_write_block(&us,sizeof(User)*word,sizeof(User));
			word++;
			eeprom_update_word((uint16_t*)4000,word);
			
			lcd_clear();
			lcd_print("User Added");
			lcd_gotoxy(1,2);
			lcd_print("Successfully");
			_delay_ms(20000);
		}
	}
}

unsigned char compare(unsigned char* pass1, unsigned char* pass2, unsigned char len1, unsigned char len2)
{
	unsigned char i = 0;		//Just a counter variable

	if(len1 != len2)
	{
		return 0;
	}
	
	else
	{
		for(i = 0; i < len1; i++)
		{
			if(pass1[i] != pass2[i])
			{
				return 0;
			}
			else
			{
				continue;			//No need for it, just to make the code readable
			}
		}
		return 1;
	}
	
}

void open_from_inside(void){
	open_door();
	_delay_ms(18000);
	close_door();
}



ISR (INT0_vect)
{
	open_from_inside();
}