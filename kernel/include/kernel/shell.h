#ifndef ARCH_I386_SHELL_H
#define ARCH_I386_SHELL_H

#include <kernel/tty.h>

int executeCommand(char* ch1);
int shell();

extern virtual_terminal_t terminals[3];
extern int shell_init;
extern int termNum;
extern int currentSpace;
extern char *previousCommands[15];
extern int commands_entered;

/*PROTOTYPES*/
int shutdown();
int help();
int shell();
int color();
int setusername();
int invalid();
int clear();
int list();
int calculator();
int sysInfo();
int reboot();

#endif
