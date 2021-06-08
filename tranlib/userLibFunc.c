#include <stdio.h>
void *program_break;

char write_buf[128];
char read_buf[128];

void printInt(int i, int width);
void *allocate(unsigned int size);

void __stack_chk_fail()
{

}

void init_heap()
{
    asm("mov $12, %%eax;\n\t"
        "mov $0, %%edi;\n\t"
        "syscall;\n\t"
        "mov %%rax, %0;"
        :"=r"(program_break)
        :
        :"%rax", "%edi");
    //printInt(program_break, 12);
    // printInt(program_break, 0);
    // allocate(10);
    // printInt(program_break, 0);
}

void *allocate(unsigned int size)
{
    init_heap();
    void *cur = program_break;
    program_break += 8 * size;
    asm("mov $12, %%eax;\n\t"
        "mov %0, %%rdi;\n\t"
        "syscall;\n\t"
        :
        :"r"(program_break)
        :"%rax", "%edi");
    //printInt(program_break, 12);
    return cur;
}

void printInt(int i, int width)
{
    //ADJUST_ARGS;
    int cur = 63;
    int isNeg = i < 0;
    i = i < 0 ? -i : i;
    do
    {
        int tail = i % 10;
        write_buf[cur--] = tail + '0';
        i = i / 10;
    } while(i > 0);
    if (isNeg)
        write_buf[cur--] = '-';
    int len = 63 - cur;
    int k;
    for (k = 0; k < width - len; k++)
    {
        write_buf[k] = ' ';
    }
    int j;
    for (j = 0; j < len; j++)
        write_buf[k + j] = write_buf[++cur];
    write_buf[k+j] = '\r';
    write_buf[k+j+1] = '\n';
    asm("mov $1, %%rax\n\t"
        "mov $1, %%rdi\n\t"
        "mov %0, %%rsi\n\t"
        "movl %1, %%edx\n\t"
        "syscall\n\t"
        :
        :"r"(write_buf), "r"(k+j+2)
        :"%rax", "%rdi", "%rsi", "%rdx");
}

int __string_to_int(char *str)
{
    int p = 0;
    while(str[p] >= '0' && str[p] <= '9')
        p++;
    p--;
    int base = 1;
    int ret = 0;
    int start = (str[0] == '-');
    while(p >= start)
    {
        ret += base * (str[p] - '0');
        base *= 10;
        p--;
    }
    if (start == 1)
        ret = -ret;
    return ret;
}

int readInt()
{
    asm("mov $0, %%rax\n\t"
        "mov $0, %%rdi\n\t"
        "mov %0, %%rsi\n\t"
        "movl %1, %%edx\n\t"
        "syscall\n\t"
        :
        :"r"(read_buf), "r"(64)
        :"%rax", "%rdi", "%rsi", "%rdx");
    //printf("%s", buf);
    return __string_to_int(read_buf);
}