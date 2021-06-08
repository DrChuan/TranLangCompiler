#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *allocate(int size)
{
    return malloc(size * 8);
}

void printString(char *str)
{
    printf("%s", str);
}

void printInt(int i, int width)
{
    // char buf[128];
    // sprintf(buf, "%d\n", i);
    // write(1, buf, strlen(buf));
    printf("%*d", width, i);
}

void printDouble(double d, int afterPoint)
{
    asm("movq %rdi, %xmm0\n\t"
	"movq %rsi, %rdi\n\t"
	"movsd %xmm0, -8(%rbp)\n\t"
	"movl %edi, -12(%rbp)");
    printf("%.*lf", afterPoint, d);
}

int readInt()
{
    char read_buf[128];
    scanf("%s", read_buf);
    // asm("mov $0, %%rax\n\t"
    //     "mov $0, %%rdi\n\t"
    //     "mov %0, %%rsi\n\t"
    //     "movl %1, %%edx\n\t"
    //     "syscall\n\t"
    //     :
    //     :"r"(read_buf), "r"(128)
    //     :"%rax", "%rdi", "%rsi", "%rdx");
    //printf("%s", buf);
    return atoi(read_buf);
}

double readDouble()
{
    char read_buf[128];
    scanf("%s", read_buf);
    return atof(read_buf);
}

char *readString()
{
    char *read_buf = (char *)allocate(128 / 8);
    scanf("%s", read_buf);
    return read_buf;
}

char *readLine()
{
    char *read_buf = (char *)allocate(128 / 8);
    char ch;
    int cur = 0;
    while((ch = getchar()) != '\n')
        read_buf[cur++] = ch;
    read_buf[cur] = 0;
    return read_buf;
}

