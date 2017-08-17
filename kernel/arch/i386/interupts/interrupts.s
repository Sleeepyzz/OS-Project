.section .text
.align 4
	.global load_idt
	.extern idt_p

	load_idt:
  	    lidt  idt_p
   	    ret

	.macro ISR_NO_ERR_CODE arg1
		.global isr\arg1
		.type isr\arg1, @function
		isr\arg1:
			cli
			push $0
			push $\arg1
			jmp isr_common_stub
	.endm
	
	.macro ISR_ERR_CODE arg1
		.global isr\arg1
		.type isr\arg1, @function
		isr\arg1:
			cli
			push $\arg1
			jmp isr_common_stub
	.endm
			
	
	ISR_NO_ERR_CODE 0
	ISR_NO_ERR_CODE 1
	ISR_NO_ERR_CODE 2
	ISR_NO_ERR_CODE 3
	ISR_NO_ERR_CODE 4
	ISR_NO_ERR_CODE 5
	ISR_NO_ERR_CODE 6
	ISR_NO_ERR_CODE 7
	ISR_ERR_CODE   8
	ISR_NO_ERR_CODE 9
	ISR_ERR_CODE   10
	ISR_ERR_CODE   11
	ISR_ERR_CODE   12
	ISR_ERR_CODE   13
	ISR_ERR_CODE   14
	ISR_NO_ERR_CODE 15
	ISR_NO_ERR_CODE 16
	ISR_NO_ERR_CODE 17
	ISR_NO_ERR_CODE 18
	ISR_NO_ERR_CODE 19
	ISR_NO_ERR_CODE 20
	ISR_NO_ERR_CODE 21
	ISR_NO_ERR_CODE 22
	ISR_NO_ERR_CODE 23
	ISR_NO_ERR_CODE 24
	ISR_NO_ERR_CODE 25
	ISR_NO_ERR_CODE 26
	ISR_NO_ERR_CODE 27
	ISR_NO_ERR_CODE 28
	ISR_NO_ERR_CODE 29
	ISR_NO_ERR_CODE 30
	ISR_NO_ERR_CODE 31
	
	.global irq0
	.type irq0, @function
	irq0:
		cli
		push $0
		push $32
		jmp timer_stub

	.global irq1
	.type irq1, @function
	irq1:
		cli
		push $0
		push $33
		jmp keyboard_stub

	.global irq2
	.type irq2, @function
	irq2:
		cli
		push $0
		push $34
		jmp irq_common_stub

	.global irq3
	.type irq3, @function
	irq3:
		cli
		push $0
		push $35
		jmp irq_common_stub

	.global irq4
	.type irq4, @function
	irq4:
		cli
		push $0
		push $36
		jmp irq_common_stub

	.global irq5
	.type irq5, @function
	irq5:
		cli
		push $0
		push $37
		jmp irq_common_stub

	.global irq6
	.type irq6, @function
	irq6:
		cli
		push $0
		push $38
		jmp irq_common_stub

	.global irq7
	.type irq7, @function
	irq7:
		cli
		push $0
		push $39
		jmp irq_common_stub

	.global irq8
	.type irq8, @function
	irq8:
		cli
		push $0
		push $40
		jmp irq_common_stub

	.global irq9
	.type irq9, @function
	irq9:
		cli
		push $0
		push $41
		jmp irq_common_stub

	.global irq10
	.type irq10, @function
	irq10:
		cli
		push $0
		push $42
		jmp irq_common_stub

	.global irq11
	.type irq11, @function
	irq11:
		cli
		push $0
		push $43
		jmp irq_common_stub

	.global irq12
	.type irq12, @function
	irq12:
		cli
		push $0
		push $44
		jmp irq_common_stub

	.global irq13
	.type irq13, @function
	irq13:
		cli
		push $0
		push $45
		jmp irq_common_stub

	.global irq14
	.type irq14, @function
	irq14:
		cli
		push $0
		push $46
		jmp irq_common_stub

	.global irq15
	.type irq15, @function
	irq15:
		cli
		push $0
		push $47
		jmp irq_common_stub

	# Tell the assembler theres an external symbol in use
	.extern isr_handler

	# isr common stub
	.global isr_common_stub
	.type isr_common_stub, @function
	isr_common_stub:
		pusha
		
		pushw %ds
		pushw %es
		pushw %fs
		pushw %gs

		movw $0x10, %ax
		movw %ax, %ds 
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs
		movl %esp, %eax
		pushl %eax
		call isr_handler
		popl %eax
		popw %gs
		popw %es
		popw %fs
		popw %gs
		
		popa
		addl $8, %esp
		iret
	
	.extern irq_handler
	.global irq_common_stub
	.type irq_commmon_stub, @function
	irq_common_stub:
		pusha

		pushw %ds
		pushw %es
		pushw %fs
		pushw %gs

		movw $0x10, %ax
		movw %ax, %ds 
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs
		movl %esp, %eax
		pushl %eax
		call irq_handler
		popl %eax
		popw %gs
		popw %es
		popw %fs
		popw %gs
		
		popa
		addl $8, %esp
		iret
	.extern timer_handler
	.global timer_stub
	.type timer_stub, @function
	timer_stub:
		pusha

		pushw %ds
		pushw %es
		pushw %fs
		pushw %gs

		movw $0x10, %ax
		movw %ax, %ds 
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs
		movl %esp, %eax
		pushl %eax
		call timer_handler
		popl %eax
		popw %gs
		popw %es
		popw %fs
		popw %gs
		
		popa
		addl $8, %esp
		iret

	.extern keyboard_handler
	.global keyboard_stub
	.type keyboard_stub, @function
	keyboard_stub:
		pusha

		pushw %ds
		pushw %es
		pushw %fs
		pushw %gs

		movw $0x10, %ax
		movw %ax, %ds 
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs
		movl %esp, %eax
		pushl %eax
		call keyboard_handler
		popl %eax
		popw %gs
		popw %es
		popw %fs
		popw %gs
		
		popa
		addl $8, %esp
		iret
