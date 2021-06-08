// 一些内部使用的函数，原则上不提供给用户使用

#include <string.h>

void *allocate(int n);

int __do_logic_and(int op1, int op2)
{
    return op1 && op2;
}

int __do_logic_or(int op1, int op2)
{
    return op1 || op2;
}

int __do_logic_not(int op1)
{
    return !op1;
}

int __do_cmp_equal(int op1, int op2)
{
    return op1 == op2;
}

int __do_cmp_neq(int op1, int op2)
{
    return op1 != op2;
}

int __do_cmp_geq(int op1, int op2)
{
    return op1 >= op2;
}

int __do_cmp_leq(int op1, int op2)
{
    return op1 <= op2;
}

int __do_cmp_greater(int op1, int op2)
{
    return op1 > op2;
}

int __do_cmp_less(int op1, int op2)
{
    return op1 < op2;
}

char *__do_str_add(char *str1, char *str2)
{
    char *res = allocate((strlen(str1) + strlen(str2) + 7) / 8);
    strcpy(res, str1);
    strcat(res, str2);
    return res;
}

int __do_str_sub(char *str1, char *str2)
{
    return str1[0] - str2[0];
}

double __do_double_add(double d1, double d2)
{
    return d1 + d2;
}

double __do_double_sub(double d1, double d2)
{
    return d1 - d2;
}

double __do_double_mul(double d1, double d2)
{
    return d1 * d2;
}

double __do_double_div(double d1, double d2)
{
    return d1 / d2;
}