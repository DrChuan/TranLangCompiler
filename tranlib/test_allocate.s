    .text
    .global _start
_start:
    call init_heap
    mov $12, %rdi
    call allocate
    mov $0x3333, %rbx
    mov %rbx, (%rax)
    mov $100, %rdi
    call allocate
    mov $0x4444, %rbx
    mov %rbx, (%rax)
    mov $60, %rax
    mov $0, %rdi
    syscall
