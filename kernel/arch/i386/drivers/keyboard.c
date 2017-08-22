#include <stdint.h>
#include <kernel/irq.h>
#include <kernel/keyboard.h>
#include <kernel/port.h>
#include <kernel/pit.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


extern char *previousCommands[15];
extern int commands_entered;

char* readStr()
{
    char* buffstr = malloc(200, 0, 0);
    int count = commands_entered - 1;
    uint8_t i = 0;
    int thing = 0;
    uint8_t reading = 1;
    uint8_t shift_on = 0;
    uint8_t caps_lock = 0;
    for (int n = 0; n < strlen(buffstr); n++) {
	buffstr[n] = 0;
    }
    while(reading)
    {
        if(inb(0x64) & 0x1)
        {
	    
            switch(inb(0x60))
            { 
      /*case 1:
                putchar('(char)27);           Escape button
                buffstr[i] = (char)27;
                i++;
                break;*/
        case 2:
		if (shift_on == 1) {
			putchar('!');
			buffstr[i] = '!';
		}
		else {
                	putchar('1');
                	buffstr[i] = '1';
		}
                i++;
		shift_on = 0;
                break;
        case 3:
                if (shift_on == 1) {
			putchar('@');
			buffstr[i] = '@';
		}
		else {
                	putchar('2');
                	buffstr[i] = '2';
		}
                i++;
		shift_on = 0;
                break;
        case 4:
                if (shift_on == 1) {
			putchar('#');
			buffstr[i] = '#';
		}
		else {
                	putchar('3');
                	buffstr[i] = '3';
		}
                i++;
		shift_on = 0;
                break;
        case 5:
                if (shift_on == 1) {
			putchar('$');
			buffstr[i] = '$';
		}
		else {
                	putchar('4');
                	buffstr[i] = '4';
		}
                i++;
		shift_on = 0;
                break;
        case 6:
                if (shift_on == 1) {
			putchar('%');
			buffstr[i] = '%';
		}
		else {
                	putchar('5');
                	buffstr[i] = '5';
		}
                i++;
		shift_on = 0;
                break;
        case 7:
                if (shift_on == 1) {
			putchar('^');
			buffstr[i] = '^';
		}
		else {
                	putchar('6');
                	buffstr[i] = '6';
		}
                i++;
		shift_on = 0;
                break;
        case 8:
                if (shift_on == 1) {
			putchar('&');
			buffstr[i] = '&';
		}
		else {
                	putchar('7');
                	buffstr[i] = '7';
		}
                i++;
		shift_on = 0;
                break;
        case 9:
                if (shift_on == 1) {
			putchar('*');
			buffstr[i] = '*';
		}
		else {
                	putchar('8');
                	buffstr[i] = '8';
		}
                i++;
		shift_on = 0;
                break;
        case 10:
                if (shift_on == 1) {
			putchar('(');
			buffstr[i] = '(';
		}
		else {
                	putchar('9');
                	buffstr[i] = '9';
		}
                i++;
		shift_on = 0;
                break;
        case 11:
                if (shift_on == 1) {
			putchar(')');
			buffstr[i] = ')';
		}
		else {
                	putchar('0');
                	buffstr[i] = '0';
		}
                i++;
		shift_on = 0;
                break;
        case 12:
                if (shift_on == 1) {
			putchar('_');
			buffstr[i] = '_';
		}
		else {
                	putchar('-');
                	buffstr[i] = '-';
		}
                i++;
		shift_on = 0;
                break;
        case 13:
                if (shift_on == 1) {
			putchar('+');
			buffstr[i] = '+';
		}
		else {
                	putchar('=');
                	buffstr[i] = '=';
		}
                i++;
		shift_on = 0;
                break;
        case 14:
                putchar('\b');
                i--;
               	buffstr[i] = 0;
		shift_on = 0;
                break;
       /* case 15:
                putchar('\t');          Tab button
                buffstr[i] = '\t';
                i++;
                break;*/
        case 16:
                putchar('q');
                buffstr[i] = 'q';
                i++;
		shift_on = 0;
                break;
        case 17:
                putchar('w');
                buffstr[i] = 'w';
                i++;
		shift_on = 0;
                break;
        case 18:
                putchar('e');
                buffstr[i] = 'e';
                i++;
		shift_on = 0;
                break;
        case 19:
                putchar('r');
                buffstr[i] = 'r';
                i++;
		shift_on = 0;
                break;
        case 20:
                putchar('t');
                buffstr[i] = 't';
                i++;
		shift_on = 0;
                break;
        case 21:
                putchar('y');
                buffstr[i] = 'y';
                i++;
		shift_on = 0;
                break;
        case 22:
                putchar('u');
                buffstr[i] = 'u';
                i++;
                break;
        case 23:
                putchar('i');
                buffstr[i] = 'i';
                i++;
		shift_on = 0;
                break;
        case 24:
                putchar('o');
                buffstr[i] = 'o';
                i++;
		shift_on = 0;
                break;
        case 25:
                putchar('p');
                buffstr[i] = 'p';
                i++;
		shift_on = 0;
                break;
        case 26:
                putchar('[');
                buffstr[i] = '[';
                i++;
		shift_on = 0;
                break;
        case 27:
                putchar(']');
                buffstr[i] = ']';
                i++;
		shift_on = 0;
                break;
        case 28:
               // putchar('\n');
               // buffstr[i] = '\n';
                  i++;
                reading = 0;
                break;
      /*  case 29:
                putchar('q');           Left Control
                buffstr[i] = 'q';
                i++;
                break;*/
        case 30:
                putchar('a');
                buffstr[i] = 'a';
                i++;
		shift_on = 0;
                break;
        case 31:
                putchar('s');
                buffstr[i] = 's';
                i++;
		shift_on = 0;
                break;
        case 32:
                putchar('d');
                buffstr[i] = 'd';
                i++;
		shift_on = 0;
                break;
        case 33:
                putchar('f');
                buffstr[i] = 'f';
                i++;
		shift_on = 0;
                break;
        case 34:
                putchar('g');
                buffstr[i] = 'g';
                i++;
		shift_on = 0;
                break;
        case 35:
                putchar('h');
                buffstr[i] = 'h';
                i++;
		shift_on = 0;
                break;
        case 36:
                putchar('j');
                buffstr[i] = 'j';
                i++;
		shift_on = 0;
                break;
        case 37:
                putchar('k');
                buffstr[i] = 'k';
                i++;
		shift_on = 0;
                break;
        case 38:
                putchar('l');
                buffstr[i] = 'l';
                i++;
		shift_on = 0;
                break;
        case 39:
                putchar(';');
                buffstr[i] = ';';
                i++;
		shift_on = 0;
                break;
        case 40:
                putchar((char)44);               //   Single quote (')
                buffstr[i] = (char)44;
                i++;
		shift_on = 0;
                break;
        case 41:
                putchar((char)44);               // Back tick (`)
                buffstr[i] = (char)44;
                i++;
		shift_on = 0;
                break;
    	case 42:
                shift_on = 1;
                break;
        /*case 43:
                putchar((char)92);
                buffstr[i] = 'q';
                i++;
                break;*/
        case 44:
                putchar('z');
                buffstr[i] = 'z';
                i++;
		shift_on = 0;
                break;
        case 45:
                putchar('x');
                buffstr[i] = 'x';
                i++;
		shift_on = 0;
                break;
        case 46:
                putchar('c');
                buffstr[i] = 'c';
                i++;
		shift_on = 0;
                break;
        case 47:
                putchar('v');
                buffstr[i] = 'v';
                i++;
		shift_on = 0;
                break;                
        case 48:
                putchar('b');
                buffstr[i] = 'b';
                i++;
		shift_on = 0;
                break;               
        case 49:
                putchar('n');
                buffstr[i] = 'n';
                i++;
		shift_on = 0;
                break;                
        case 50:
                putchar('m');
                buffstr[i] = 'm';
                i++;
		shift_on = 0;
                break;               
        case 51:
                putchar(',');
                buffstr[i] = ',';
                i++;
		shift_on = 0;
                break;                
        case 52:
                putchar('.');
                buffstr[i] = '.';
                i++;
		shift_on = 0;
                break;            
        case 53:
                putchar('/');
                buffstr[i] = '/';
                i++;
		shift_on = 0;
                break;            
        case 54:
                shift_on = 1;
                break;                                
        case 57:
                putchar(' ');
                buffstr[i] = ' ';
                i++;
		shift_on = 0;
                break;
	//Up Arrow
	case 0x48:
		for (size_t u = 0; u < strlen(buffstr); u++) {
			printf("\b");
		}
		printf(previousCommands[count]);
		buffstr = previousCommands[count];
		count++;
		thing = 1;
		shift_on = 0;
	//Down Arrow
	case 0x50:
		if (thing == 1) {
			thing = 0;
		}
		else {
			if (count > 0) {
				for (size_t u = 0; u < strlen(buffstr); u++) {
					printf("\b");
				}
				count--;
				printf(previousCommands[count]);
				buffstr = previousCommands[count];
				
			}
		}
		shift_on = 0;
	    }
	}

	buffstr[i] = ' ';
	i++;
	buffstr[i] = ' ';
	i++;
	buffstr[i] = ' ';
	i--;
	buffstr[i] = 0;
	i--;
	buffstr[i] = 0;
    }
    return buffstr;
}

void keyboard_handler(struct regs *r) {
	outb(0x20,0x20);
}
