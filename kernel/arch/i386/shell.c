#include <stdio.h>
#include <string.h>
//VARIABLES
int exit();
char* username = "Ryken Thompson";
int help();
int settings();
int shell();
int shell_level = 0;


char* commands[] = {
	"exit",
	"cmd",
	"help",
	"settings",
};

//Parse Command, Execute command

int parseCommand(char* ch) {
	int valid_command = 0;
	int n = 0;
	int i;
	for (i = 0; i < 4; i++) {
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

int executeCommand(char* ch1) {
	int n;
	n = parseCommand(ch1);
	int status;
	printf("\n");
	switch (n) {
		case 0:
			status = exit();
			break;
		case 1:
			shell_level = shell_level + 1;
			status = shell();
			break;
		case 2:
			status = help();
			break;
		case 3:
			status = settings();
			break;
		default:
			printf("Invalid Command ");
			printf(ch1);
			printf("\n");
			status = 1;
	}
	return status;
}


//HELP COMMAND

int help() {
	printf("\ncmd		:Launches a new shell");
	printf("\nexit		:Exits the current shell");
	printf("\nsettings	:Allows you to change background color and username");
	printf("\n\n");
	return 1;
}
int exit() {
	shell_level = shell_level - 1;
	return 0;
}

int settings() {
	printf("\nEnter one of the below options");
	printf("\ncolor");
	printf("\nusername");
	printf("\n\n");
	return 1;	
}

//SHELL

int shell() {
	int counter = 0;
	int status;
	do {
		char* ch = (char*) malloc(200);
		printf("NAME OF OS: ");
		printf(username);
		printf(" (");
		printf(int_to_string(shell_level));
		printf(")>> ");
		ch = readStr();
		status = executeCommand(ch);
		
	} while (status);
	return 1;
}
