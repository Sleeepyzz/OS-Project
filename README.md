# OS-Project
This operating system was written to help me learn more about how computers work and what operating systems do. It can be helpful to others looking to start os development. The code is simple and easy to understand for most people new to operating system development. Read the COMPILING section for information about compiling and modifying the code. For anyone looking to get started on writing a memory manager, my operating system contains a working recursive paging setup and a kernel heap(in development). This operating system does not boot on real hardware, but does work on x86 emulators. The shell provides basic commands such as listing the heap, changing virtual terminals, and listing cpu and memory info. This code is based off the Meaty Skeleton operating system on http://wiki.osdev.org/Meaty_Skeleton.


---------------------------------------------------------------------------------------------------------------------------------
Features
---------------------------------------------------------------------------------------------------------------------------------
- Multiboot Booting
- GDT
- IDT
- IRQs and ISRs
- Physical memory management
- Higher half kernel
- Recursive paging
- Kernel Heap
- Keyboard
- Textmode interface with printf
- PIT
![alt text](https://raw.githubusercontent.com/wasd424/OS-Project/master/Screenshot%20from%202017-12-09%2017-13-57.png)

---------------------------------------------------------------------------------------------------------------------------------
COMPILING
---------------------------------------------------------------------------------------------------------------------------------
To compile the operating system a i686 cross compiler is needed. The instructions for creating a i686 cross compiler can be found at http://wiki.osdev.org/GCC_Cross-Compiler. I used gcc 7.1.0 and binutils 2.28 and those can e download from the links on osdev.org. Once a cross compiler is created chmod +x each .sh file in the root directory. Then run ./headers.sh, ./iso.sh. This will create the plpos.iso file that can be used in any x86 emulator. To clean the files created by iso.sh, run the command ./clean.sh. After modifying the code for a change to take place you must recompile the operating system.


---------------------------------------------------------------------------------------------------------------------------------
RUNNING/DEBUGGING
---------------------------------------------------------------------------------------------------------------------------------
To run/debug in bochs using the command bochs in the root directory.
You can install bochs on Ubuntu/Debian using sudo apt-get install bochs.

To run/debug in qemu using the command qemu-system-i386 -cdrom plpos.iso in the root directory. Then using GDB to debug.
You can install qemu on Ubuntu/Debian using sudo apt-get install qemu-system-i386.


---------------------------------------------------------------------------------------------------------------------------------
CONTRIBUTE
---------------------------------------------------------------------------------------------------------------------------------
I would appreciate it if you would contribute in any way to make this operating system better.
Issue Tracker - https://github.com/wasd424/OS-Project/issues
Source Code   - https://github.com/wasd424/OS-Project


---------------------------------------------------------------------------------------------------------------------------------
LICENSE
---------------------------------------------------------------------------------------------------------------------------------
The GNU General Public License, version 3.
