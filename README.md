# OS-Project
This operating system was written to help me learn more about how computers work and what operating systems do. It can be helpful to others looking to start os development. The code is simple and easy to understand for most people new to operating system development. Read the COMPILING section for information about compiling and modifying the code. For anyone looking to get started on writing a memory manager, my operating system contains a working recursive paging setup and a kernel heap(in development). This operating system does not boot on real hardware, but does work on x86 emulators. The shell provides basic commands such as listing the heap, changing virtual terminals, and listing cpu and memory info. This code is based off the Meaty Skeleton operating system on http://wiki.osdev.org/Meaty_Skeleton.


Features:
- Multiboot Booting
- GDT
- IDT
- Physical memory management
- Higher half kernel
- Recursive paging
- Kernel Heap
- Keyboard
- Textmode interface with printf
- PIT


COMPILING:
To compile the operating system a i686 cross compiler is needed. The instructions for creating a i686 cross compiler can be found at http://wiki.osdev.org/GCC_Cross-Compiler. Once a cross compiler is created chmod +x each .sh file in the root directory. Then run ./headers.sh, ./iso.sh. This will create the plpos.iso file that can be used in any x86 emulator. To clean the files created by iso.sh, run the command ./clean.sh. After modifying the code for a change to take place you must recompile the operating system.

To run in bochs use the command bochs in the root directory.
To use in qemu use the command qemu-system-i386 -cdrom plpos.iso in the root directory.


CONTRIBUTE:
I would appreciate it if you would contribute in any way to make this operating system better.
Issue Tracker - https://github.com/wasd424/OS-Project/issues
Source Code   - https://github.com/wasd424/OS-Project


LICENSE:
The GNU General Public License unless otherwise said in that file.
