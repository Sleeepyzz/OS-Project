#include <stdio.h>
#include <kernel/tty.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <kernel/keyboard.h>
#include <kernel/multiboot.h>
extern multiboot_info_t *mbi;
extern uint32_t total_mem;

//PROTOTYPES
int exit();
int help();
int shell();
int color();
int setusername();
int invalid();
int clear();
int settings();
int calculator();
int sysInfo();

//POINTERS
int (*commandaddrs[8])() = { shell, help, settings, exit, clear, sysInfo, calculator, invalid};

//VARIABLES
char* commands[] = {
	"shell",
	"help",
	"settings",
	"exit",
	"clear",
	"info",
};
char* username = "Ryken Thompson";
int shell_level = 0;
int start = 1;

char *previousCommands[15];
int commands_entered = 0;

char *args[5][15];
int commandNum;


//Parse Command, Execute command


/*Reads through list of commands and see's if input matches any of them
increases int n until it finds a match or goes through all known commands*/
void parseCommand(char* ch) {
	commandNum = 7;
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
	if (isNumber(args[0]) == 1) {
		commandNum = 6;
	}
	else {
		for (int x = 0; x < 6; x++) {
			if (strcmp(args[0], commands[x]) == 0) {
				commandNum = x;
			}
		}
	}

}

/*Splits command with parseCommand then executes command based on the commandNum.
This command can be used */
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


int sysInfo() {
	detectCpu();
	multiboot_memory_map_t *mmap;
     
        printf ("\nMemory Map:\nmmap_addr = 0x%x, mmap_length = 0x%x\n",
        (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
        for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
        (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
        mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
        + mmap->size + sizeof (mmap->size))) {

             	printf (" size = 0x%x, base_addr = 0x%x%x\b,"
                " length_hex = 0x%x%x\b, type = 0x%x\n",
                (unsigned) mmap->size,
                mmap->addr,
                mmap->len,
                (unsigned) mmap->type);
         	}
	printf("\nTotal Memory: %uMiB\n", total_mem / 1024 / 1024);
	return 1;
}

//HELP COMMAND

int help() {
	printf("\n-----------------------------Help-----------------------------\n");
	printf("\nCommand         Arguments                 Purpose\n");
	printf("\nshell        [number 0 - 4]         :Launches a new shell");
	printf("\nexit                                :Exits the current shell");
	printf("\nclear                               :Clears the screen of all text ");
	printf("\nsettings     [username/color]       :Lets you switch the username and color");
	printf("\ninfo                                :Shows you CPU and Memory information");
	printf("\nYou can also  use the shell as a calculator");
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

int calculator() {
	printf("\n");
	if (sizeof(args < 3)) {
		int num1 = atoi(args[0]);
		int num2 = atoi(args[2]);
		if (strcmp(args[1], "+") == 0) {
			printf("%d", (num1 + num2));
		}
		else if (strcmp(args[1], "-") == 0) {
			if (num1 < num2) {
				printf("-");
				printf("%d", (num1 - num2));
			}
			else {
				printf("%d", (num1 - num2));
			}
		}
		else if (strcmp(args[1], "*") == 0) {
			printf("%d", (num1 * num2));
		}
		else if (strcmp(args[1], "/") == 0) {
			if (args[0] == 0 || args[2] == 0) {
				printf("Don't Divide with Zero");
				return 1;
			}
			printf("%d", (num1 / num2));
		}
		else if (strcmp(args[1], "^") == 0) {
			int args0 = atoi(args[0]);
			int temp = 1;
			
			for (int i = 0; i < atoi(args[2]); i++) {
				temp = temp * args0;
			}
			printf("%d", temp);
		}
		else {
			printf("Only enter like [number1] [+, -, /, *, ^] [number2]");
			return 1;
		}
		return 1;
	}
	printf("Only enter like [number] [+,-,/,*, ^, %] [number2]");
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
	for (int i = 0; i < 4; i++) {
		if (i == atoi(args[1])) {
			break;
		}
		else if (atoi(args[1]) > 3) {
			printf("\nToo high of a shell number\n");
			return 1;
		}
		else if (i == 4) {
			printf("\nYou did not enter a number!!!\n");
			return 1;
		}
		
	}
	if (start) {
		start = 0;
	}
	else {
		terminal_clear();
		shell_level = atoi(args[1]);
	}
	
	do {
		char* ch = (char*)malloc(200, 0, 0);
		printf("\nPLP OS: ");
		printf(username);
		printf(" (");
		printf(int_to_string(shell_level));
		printf(")>> ");
		ch = (char*)readStr();
		status = executeCommand(ch);
		
	} while (status);
	return 1;
}
