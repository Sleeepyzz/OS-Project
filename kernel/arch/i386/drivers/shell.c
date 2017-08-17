/*Shell code written by Ryken Thompson */

#include <stdio.h>
#include <kernel/tty.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
//PROTOTYPES
int exit();
int help();
int shell();
int color();
int setusername();
int invalid();
int clear();
int add();
int multiply();
int divide();
int subtract();
int settings();

//POINTERS
int (*commandaddrs[10])() = { shell, help, settings, exit, clear, add, multiply, divide, subtract, invalid };

//VARIABLES
char* commands[] = {
	"cmd",
	"help",
	"settings",
	"exit",
	"clear",
	"add",
	"multiply",
	"divide",
	"subtract",
};
char* username = "Ryken Thompson";
int shell_level = 0;

char* terminal0 = malloc(sizeof(char*) * 25 * 100);
char* terminal1 = malloc(sizeof(char*) * 25 * 100);
char* terminal2 = malloc(sizeof(char*) * 25 * 100);
char* terminal3 = malloc(sizeof(char*) * 25 * 100);
char* terminal4 = malloc(sizeof(char*) * 25 * 100);


char *previousCommands[15];
int commands_entered = 0;

//


char *args[5][15];
char *arg1;
char *arg2;
int commandNum;


//Parse Command, Execute command


/*Reads through list of commands and see's if input matches any of them
increases int n until it finds a match or goes through all known commands*/
void parseCommand(char* ch) {
	commandNum = 9;
	char buf[100];
	strcpy(buf, ch);	
	char *p = strtok (buf, " ");
	int i = 0;

    	while (p != NULL)
    	{
		strcpy(args[i], p);
		i++;
   		p = strtok (NULL, " ");
    	}
	for (int x = 0; x < 9; x++) {
		if (strcmp(args[0], commands[x]) == 0) {
			commandNum = x;
		}
	}

}

//Executes command based on number returned from parseCommand
int executeCommand(char* ch1) {
	parseCommand(ch1);
	int status = 1;
	status = (*commandaddrs[commandNum])();
	if (commands_entered >= 15) {
		commands_entered = 0;
		for (int i = 0; i < commands_entered; i++) {
			previousCommands[i] = "";
		}
	}
	previousCommands[commands_entered] = commands[commandNum];
	commands_entered++;
	return status;
}


//HELP COMMAND

int help() {
	printf("\n-----------------------------Help-----------------------------\n");
	printf("\nCommand         Arguments                 Purpose\n");
	printf("\nshell        [number 0 - 4]        :Launches a new shell");
	printf("\nexit                                :Exits the current shell");
	printf("\nclear        [lineNum/all]          :Clears the screen of all text");
	printf("\nadd          [num1] [num2]          :Adds two numbers together");
	printf("\nmultiply     [num1] [num2]          :Multiplies two numbers together");
	printf("\ndivide       [num1] [num2]          :Divides a number by another number");
	printf("\nsubtract     [num1] [num2]          :Subtracts a number from the other");
	printf("\nsettings     [username/color]       :Lets you switch the username and color");
	printf("\n\n");
	return 1;
}
// Exit current shell command
int exit() {
	printf("Halting system.");
	asm("hlt");
}
// Clear Screen command
int clear() {
	terminal_clear();
	return 1;
}
//Invalid command
int invalid() {
	
	printf("\nInvalid Command. Type help for a list of valid commands\n");
	return 1;
}
//Set Username
int setusername() {
	printf("Enter new username: ");
	username = (char*)readStr();
	printf("\nUsername set to ");
	printf(username);
	printf("\n");
	return 1;
}
int num1 = 0;
int num2 = 0;

int add(){
	int num1 = 0;
	int num2 = 0;
	if (sizeof(args) < 2) {
		printf("ERROR. \n Please enter exactly two numbers");
	}
	else {
		num1 = str_to_int(args[1]);
		num2 = str_to_int(args[2]);
		printf("\n");
		printf(int_to_string(num1 + num2));
	}
	return 1;
}
int multiply(){
	int num1 = 0;
	int num2 = 0;
	if (sizeof(args) < 2) {
		printf("ERROR. \n Please enter exactly two numbers");
	}
	else {
		num1 = str_to_int(args[1]);
		num2 = str_to_int(args[2]);
		printf("\n");
		printf(int_to_string(num1 * num2));
	}
	return 1;
}
int divide(){
	int num1 = 0;
	int num2 = 0;
	if (sizeof(args) < 2) {
		printf("ERROR. \n Please enter exactly two numbers");
	}
	else {
		if (num1 == 0 || num2 == 0) {
			printf("\nDONT DIVIDE WITH ZERO\n");
			return 1;
		}
		num1 = str_to_int(args[1]);
		num2 = str_to_int(args[2]);
		printf("\n");
		printf(int_to_string(num1 / num2));
	}
	return 1;
}
int subtract() {
	int num1 = 0;
	int num2 = 0;
	if (sizeof(args) < 2) {
		printf("ERROR. \n Please enter exactly two numbers");
	}
	else {
		num1 = str_to_int(args[1]);
		num2 = str_to_int(args[2]);
		printf("\n");
		printf(int_to_string(num1 - num2));
	}
	return 1;
}


//Set background and foreground color
int color() {
	char* ch = (char*)malloc(200,0,0);
	uint8_t fg;
	uint8_t bg;
	printf("\nColor code sheet:\n");
	printf("BLACK = 0,\nBLUE = 1,\nGREEN = 2,\nCYAN = 3,\nRED = 4,\nMAGENTA = 5,\nBROWN = 6,\nLIGHT_GREY = 7,\nDARK_GREY = 8,\nLIGHT_BLUE = 9,\nLIGHT_GREEN = 10,\nLIGHT_CYAN = 11,\nLIGHT_RED = 12,\nLIGHT_MAGENTA = 13,\nLIGHT_BROWN = 14,\nWHITE = 15\n");
	printf("Type in the corresponding number to the color you want for the background\n");
	ch = (char*)readStr();
	bg = str_to_int(ch);
	printf("\nNow type in the number of the color you want for the text\n");
	ch = (char*)readStr();
	fg = str_to_int(ch);
	printf("\nSetting new colors...");
	terminal_setcolor(fg | bg << 4);
	terminal_clear();
	return 1;
	
}

//Settings
int settings() {
	printf("\n---------------------------Settings----------------------------\n");
	int status;
	if(strcmp(args[1], "username") == 0) {
		status = setusername();
	}
	else if (strcmp(args[1], "color") == 0) {
		status = color();
	}
	else {
		status = invalid();
	}
	printf("\n");
	return status;	
}
//Main shell loop
int shell() {
	int status;
	if (str_to_int(args[1]) > 2) {
		printf("NOTE: Only using the first argument");
	}
	for (int i = 0; i == 5; i++) {
		if (i == str_to_int(args[1])) {
			break;
		}
		else if (str_to_int(args[1]) > 5) {
			printf("\nToo high of a shell number\n");
			invalid("");
		}
		else if (i == 4) {
			printf("\nYou did not enter a number!!!\n");
			invalid("");
		}
		
	}
	shell_level = str_to_int(args[1]);
	do {
		char* ch = (char*)malloc(200, 0, 0);
		printf("\nNAME OF OS: ");
		printf(username);
		printf(" (");
		printf(int_to_string(shell_level));
		printf(")>> ");
		ch = (char*)readStr();
		status = executeCommand(ch);
		
	} while (status);
	return 1;
}
