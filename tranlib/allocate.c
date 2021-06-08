// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>

void *program_break;

void init_heap()
{
    asm("mov $12, %%eax;\n\t"
        "mov $0, %%edi;\n\t"
        "syscall;\n\t"
        "mov %%rax, %0;"
        :"=r"(program_break)
        :
        :"%rax", "%edi");
}

void *allocate(unsigned int size)
{
    void *cur = program_break;
    program_break += size;
    asm("mov $12, %%eax;\n\t"
        "mov %0, %%rdi;\n\t"
        "syscall;\n\t"
        :
        :"r"(program_break)
        :"%rax", "%edi");
    return cur;
}

// int main()
// {
//     void *p_asm;
//     void *p_brk;
//     void *p_sbrk;
//     void *p_malloc;
//     //void *p_new;
//     asm("mov $12, %%eax;\n\t"
//         "mov $0, %%edi;\n\t"
//         "syscall;\n\t"
//         "mov %%rax, %0;"
//         :"=r"(p_asm)
//         :
//         :"%rax", "%edi");
//     //printf("%lx", p_asm);
//     p_brk = (void*)syscall(12, 0);
//     p_sbrk = sbrk(52);
//     p_sbrk = sbrk(0);
//     p_malloc = malloc(10000);
//     p_brk = (void*)syscall(12, 0);
//     printf("%lx\n%lx\n%lx\n%lx\n", p_asm, p_brk, p_sbrk, p_malloc);
// }