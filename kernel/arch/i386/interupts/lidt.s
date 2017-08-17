.section .text
.global load_idt
.extern idt_p

load_idt:
    lidt  idt_p
    ret
