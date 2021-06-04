#ifndef __INTERMEDIATE_CODE__
#define __INTERMEDIATE_CODE__

#include "../utility.h"
#include "../y.tab.h"

enum InterCodeOperandType {
    I_LITERAL, D_LITERAL, S_LITERAL, TEMP, ORI_ID
};

class InterCodeOperand
{
public:
    static InterCodeOperand *createLiteral(int i);
    static InterCodeOperand *createLiteral(double d);
    static InterCodeOperand *createLiteral(string s);
    static InterCodeOperand *createTemp(int id=-1);
    static InterCodeOperand *createVar(string varName);
    static int getTempCount() { return tempCount; }
    void print();
    InterCodeOperandType getType() const { return type; }
    int getIntVal() const { return value.ival; }
    double getDoubleVal() const { return value.dval; }
    string getStringVal() const { return value.sval; }
private:
    static int tempCount;
    InterCodeOperandType type;
    U value;  // 若为字面量，直接保存在相应字段。
              // 若为变量名，用sval字段保存。
              // 若为中间变量，用ival字段保存编号
};

enum InterCodeOperator
{
    IC_ADD, IC_SUB, IC_MUL, IC_DIV, IC_MOD, IC_EQUAL, IC_NEQ, IC_GREATER, IC_LESS, IC_GEQ, IC_LEQ, 
    IC_AND, IC_OR, IC_NOT, IC_JL, IC_JLE, IC_JZ, IC_JNZ, IC_JMP, IC_LABEL, IC_MOVE, IC_OFFSET, IC_ARG, IC_CALL, IC_RET, IC_FUNC
};

InterCodeOperator optrFromToken(int token);

class InterCode
{
public:
    InterCode(InterCodeOperator optr, InterCodeOperand *dst=nullptr, InterCodeOperand *src1=nullptr, 
        InterCodeOperand *src2=nullptr) : optr(optr), src1(src1), src2(src2), dst(dst) {}
    InterCodeOperand *src1;
    InterCodeOperand *src2;
    InterCodeOperand *dst;
    InterCodeOperator optr;
    void print();
private:
    static const string operators[];
    void printOperand(InterCodeOperand *operand);

};

class InterCodeList
{
public:
    InterCodeList() {}
    void add(InterCode code);
    void print();
    int size() const { return codeList.size(); }
    InterCode &get(int i) { return codeList[i]; }
private:
    vector<InterCode> codeList;
};

#endif