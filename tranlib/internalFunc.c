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
#include <stdio.h>
char *__do_str_add(char *str1, char *str2)
{
    char *res = allocate((strlen(str1) + strlen(str2) + 15) / 8);
    strcpy(res, str1);
    strcat(res, str2);
    //printf("~~str1=%s, str2=%s, res=%s~~\n", str1, str2, res);
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

int __do_cmp_str_equal(char *str1, char *str2)
{
    return !strcmp(str1, str2);
}

int __do_cmp_str_neq(char *str1, char *str2)
{
    return strcmp(str1, str2);
}

double __do_double_int_mul(long d, int i)
{
    double *p = (double *)(&d);
    return *p * i;
}

double __do_double_int_div(long d, int i)
{
    double *p = (double *)(&d);
    return *p / i;
}

double __do_double_int_add(long d, int i)
{
    double *p = (double *)(&d);
    return *p + i;
}