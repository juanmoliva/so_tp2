GLOBAL syscall

section .text

syscall:
    int 80h
    ret