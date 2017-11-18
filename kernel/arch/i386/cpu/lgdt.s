.global flush
.extern gdt_p
 
flush:
    lgdt gdt_p
    movw $0x10,%ax    # 0x10 is the offset in the GDT to our data segment
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%fs
    movw %ax,%gs
    movw %ax,%ss
    ljmp $0x08, $flush2

flush2:
    ret
