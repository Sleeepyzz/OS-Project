#ifdef ARCH_i386_PANIC_H
#define ARCH_I386_PANIC_H

void panic(const char* message, const char* file, uint32_t line);
#define PANIC(msg) panic(msg, __FILE__, __LINE__)

#endif
