#include <unistd.h>
#include <iostream>

int main()
{
    void *p_asm;
    void *p_brk;
    void *p_sbrk;
    void *p_malloc;
    void *p_new;
    asm("mov $12, %%eax;\n\t"
        "mov $0, %%edi;\n\t"
        "syscall;\n\t"
        "mov %%rax, %0;"
        :"=r"(p_asm)
        :
        :"%rax", "%edi");
    printf("%lx", p_asm);
    p_brk = (void*)syscall(12, 0);
    p_sbrk = sbrk(0);
    p_malloc = malloc(10);
    p_new = new int;
    printf("%lx\n%lx\n%lx\n%lx\n%lx\n", p_asm, p_brk, p_sbrk, p_malloc, p_new);
}