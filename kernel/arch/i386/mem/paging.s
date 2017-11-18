.section .text
	.global read_cr2
	.type read_cr2, @function
	read_cr2:
		mov %cr2, %eax
		ret

	.global read_cr3
	.type read_cr3, @function
	read_cr3:
		mov %cr3, %eax
		ret
	
	.global write_cr3
	.type write_cr3, @function
	write_cr3:
		push %ebp
		mov %esp, %ebp
		mov 8(%esp), %eax
		mov %eax, %cr3
		mov %ebp, %esp
		pop %ebp
		ret
	
	.global readcr4
	.type readcr4, @function
	readcr4:
		mov %cr4, %eax
		ret
