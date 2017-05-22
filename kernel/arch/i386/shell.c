/*Shell code written by Ryken Thompson */

#include <stdio.h>
#include <kernel/tty.h>
#include <string.h>
//PROTOTYPES
int exit();
int help();
int settings();
int shell();
int color();
int set_username();
int invalid();
int clear();

//CONSTANTS
int (*commandaddrs[6])(void) = { shell, help, settings, exit, clear, invalid };
char* commands[] = {
	"cmd",
	"help",
	"settings",
	"exit",
	"clear",
};

//VARIABLES
char* username = "Ryken Thompson";
int shell_level = -1;

//Parse Command, Execute command


/*Reads through list of commands and see's if input matches any of them
increases int n until it finds a match or goes through all known 		commands*/
int parseCommand(char* ch) {
	int valid_command = 0;
	int n = 0;
	unsigned int i;
	for (i = 0; i < strlen(ch); i++) {
		if (strEql(ch, commands[i])) {
			valid_command = 1;
			
		}
		else {
			if(!valid_command) {
				n++;
			}
		}
		
	}
	return n;
}

//Executes command based on number returned from parseCommand
int executeCommand(char* ch1) {
	int n;
	n = parseCommand(ch1);
	int status = 1;
	status = (*commandaddrs[n])();
	return status;
}


//HELP COMMAND

int help() {
	printf("\ncmd		:Launches a new shell");
	printf("\nexit		:Exits the current shell");
	printf("\nsettings	:Allows you to change background and text color and username");
	printf("\nclear		:Clears the screen of all text");
	printf("\n\n");
	return 1;
}
// Exit current shell command
int exit() {
	shell_level = shell_level - 1;
	return 0;
}
// Clear Screen command
int clear() {
	terminal_clear();
	return 1;
}
//Invalid command
int invalid() {
	printf("Invalid Command. Type help for a list of valid commands\n");
	return 1;
}
//Set Username
int set_username() {
	printf("\nType in new username\n");
	username = (char*)readStr();
	printf("\nUsername set to ");
	printf(username);
	printf("\n");
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
	char* ch = (char*)malloc(200,0,0);
	int status;
	printf("\nEnter one of the below options");
	printf("\ncolor");
	printf("\nusername\n");
	ch = (char*)readStr();
	if(strEql(ch, "username")) {
		status = set_username();
	}
	else if (strEql(ch, "color")) {
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
	shell_level = shell_level + 1;
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
