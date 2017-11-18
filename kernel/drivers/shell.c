#include <stdio.h>
#include <kernel/tty.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/keyboard.h>
#include <kernel/shell.h>
#include <kernel/mm.h>
extern multiboot_info_t *mbi;
extern uint32_t total_mem;
extern uint32_t useable_mem;

/*The three virtual terminals available to the shell*/
virtual_terminal_t terminals[3];

/*Pointers to the functions*/
int (*commandaddrs[11])() = { shell, help, shutdown, clear, sysInfo, list, color, setusername, reboot, calculator, invalid};

/*Names of the functions*/
char* commands[] = {
	"shell",
	"help",
	"shutdown",
	"clear",
	"info",
	"ls",
	"color",
	"username",
	"reboot",
};
char* username = "Ryken Thompson";
int shell_level = 0;
int start = 1;
int termNum = 0;
int currentSpace = 0;
int shell_init = 0;

char *previousCommands[15];
int commands_entered = 0;

char *args[5][15];
int commandNum;


/*Parse Command, Execute command
Reads through list of commands and see's if input matches any of them
increases int n until it finds a match or goes through all known commands*/
void parseCommand(char* ch) {
	commandNum = 10;
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
		commandNum = 9;
	}
	else {
		for (int x = 0; x < 9; x++) {
			if (strcmp(args[0], commands[x]) == 0) {
				commandNum = x;
			}
		}
	}

}

/*Splits command with parseCommand then executes command based on the commandNum.*/
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
	return 1;
}

int sysInfo() {
	detectCpu();
	printf("\nTotal Memory: %uMiB", total_mem / 1024 / 1024);
	printf("\nUseable Memory: %uMiB\n", useable_mem / 1024 / 1024);
	return 1;
}

/*HELP COMMAND
Returns a list of commands to the user*/

int help() {
	printf("\n-----------------------------Help-----------------------------\n");
	printf("\nCommand         Arguments                 Purpose\n");
	printf("\nshell        	[number 0 - 2]          :Switches to a different shell");
	printf("\nclear                                 :Clears the screen of all text ");
	printf("\nshutdown                              :Shuts down the computer");
	printf("\nreboot                                :Reboots computer");
	printf("\nusername     	[new_username]          :Switches username");
	printf("\ninfo                                  :CPU and Memory info");
	printf("\ncolor         [num 1] [num 2]         :Switches the bg and fg colors");
	printf("\nYou can also  use the shell as a calculator");
	printf("\n\n");
	return 1;
}
/* Shutdown computer by sending shutdown signal */
int shutdown() {
	outw(0xB004, 0x00002000);
	return 1;
}

/* Clear Screen command */
int clear() {
	terminal_clear();
	return 1;
}

/*Invalid command 
Executed if none of the other commands match the first arg*/
int invalid() {
	
	printf("\nInvalid Command. Type help for a list of valid commands\n");
	return 1;
}

/*Reboot system by sending reboot signal*/
int reboot() {
	outb(0x64, 0xFE);
	return 1;
}

/*Set Username
Changes the username displayed for the current session*/
int setusername() {
	username = args[1];
	printf("\nUsername set to ");
	printf(username);
	printf("\n");
	return 1;
}

/*Basic calculator
can do +, -, *, /, ^, and %
If the first character of the input
is a number calculator will be executed*/
int calculator() {
	printf("\n");
	if (args[0] == "sqrt") {
		return 1;
	}
	int num1 = atoi(args[0]);
	int num2 = atoi(args[2]);
	if (strcmp(args[1], "+") == 0) {
		printf("%d", (num1 + num2));
	}
	else if (strcmp(args[1], "-") == 0) {
		printf("%d", (num1 - num2));

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
		if (num1 % num2 != 0) {
			printf(".");
			int num3 = num1;
			for (int i = 0; i < 5; i++) {
				num3 = (num3 % num2) * 10;
				printf("%d", num3 / num2);
				if (num3 % num2 == 0) {
					break;
				}
			}
		}
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

/*Can list the grub memory map
Heap, and Color code sheet*/
int list() {
	int status = 1;
	if(strcmp(args[1], "mmap") == 0) {
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
	}
	else if (strcmp(args[1], "colors") == 0) {
		printf("\nColor code sheet:\n");
		printf("BLACK = 0,\nBLUE = 1,\nGREEN = 2,\nCYAN = 3,\nRED = 4,\nMAGENTA = 5,\nBROWN = 6,\nLIGHT_GREY = 7,\nDARK_GREY = 8,\nLIGHT_BLUE = 9,\nLIGHT_GREEN = 10,\nLIGHT_CYAN = 11,\nLIGHT_RED = 12,\nLIGHT_MAGENTA = 13,\nLIGHT_BROWN = 14,\nWHITE = 15\n");
	}
	else if (strcmp(args[1], "heap") == 0) {
		printheap();
	}
	else {
		status = invalid();
	}
	return status;
}

//Set background and foreground color
int color() {
	uint8_t fg;
	uint8_t bg;
	bg = atoi(args[1]);
	fg = atoi(args[2]);
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
	for (int i = 0; i < 3; i++) {
		if (i == atoi(args[1])) {
			break;
		}
		else if (atoi(args[1]) > 2) {
			printf("\nToo high of a shell number\n");
			return 1;
		}
		else if (i == 3) {
			printf("\nYou did not enter a number!!!\n");
			return 1;
		}
		
	}
	if (start) {
		for (int i = 0; i < 3; i++) {
			terminals[i].buffer = kmalloc(80 * 25 * 2);
			terminals[i].x = 0;
			terminals[i].y = 0;
			terminals[i].color = terminal_color;
		}
		shell_init = 1;
		start = 0;
	}
	else {
		termNum = atoi(args[1]);
		set_buffer(terminals[termNum].buffer);
		terminal_setcolor(terminals[termNum].color);
		unsigned short pos = (terminals[termNum].y * 80) + terminals[termNum].x;
		move_cursor(pos + 1);
		shell_level = atoi(args[1]);
	}
	
	do {
		char* ch = (char*)kmalloc(200);
		printf("\nPLP OS: %s (%d)>>", username, shell_level);
		ch = (char*)readStr();
		status = executeCommand(ch);
		kfree(ch);
		
	} while (status);
	for(int i = 0; i < 3; i++) {
		kfree(terminals[i].buffer);
	}
	printf("How did this happen");
	return 1;
}
