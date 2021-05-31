#ifndef __INTERMEDIATE_CODE__
#define __INTERMEDIATE_CODE__

#include "../utility.h"

enum InterCodeOperandType {
    I_LITERAL, D_LITERAL, S_LITERAL, TEMP, ORI_ID
};

class InterCodeOperand
{
public:
    static InterCodeOperand *createLiteral(int i);
    static InterCodeOperand *createLiteral(double d);
    static InterCodeOperand *createLiteral(string s);
    static InterCodeOperand *createTemp();
    static InterCodeOperand *createVar(string varName);
    void print();
private:
    static int tempCount;
    InterCodeOperandType type;
    U value;  // 若为字面量，直接保存在相应字段。
              // 若为变量名，用sval字段保存。
              // 若为中间变量，用ival字段保存编号
};

enum InterCodeOperator
{
    IC_ADD, IC_SUB, IC_MUL, IC_DIV, IC_MOD, IC_AND, IC_OR, IC_NOT, IC_JL, IC_JLE, IC_JEQ, IC_LABEL
};

class InterCode
{
public:
    InterCode(InterCodeOperator optr, InterCodeOperand *dst=nullptr, InterCodeOperand *src1=nullptr, 
        InterCodeOperand *src2=nullptr) : optr(optr), src1(src1), src2(src2), dst(dst) {}
    void print();
private:
    static const string operators[];
    void printOperand(InterCodeOperand *operand);
    InterCodeOperand *src1;
    InterCodeOperand *src2;
    InterCodeOperand *dst;
    InterCodeOperator optr;
};

class InterCodeList
{
public:
    InterCodeList() {}
    void add(InterCode code);
    void print();
private:
    vector<InterCode> codeList;
};

#endif