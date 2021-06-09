#include "IntermediateCode.h"

const string InterCode::operators[] = {"add", "sub", "mul", "div", "mod", "equal", "neq", "greater", "less", "geq", "leq", 
    "and", "or", "not", "jl", "jle", "jz", "jnz", "jmp", "label", "move", "offset", "arg", "call", "return", "func", "real_return"};

int InterCodeOperand::tempCount = 0;
VarType InterCodeOperand::lastVarType = 0;

InterCodeOperator optrFromToken(int token)
{
    switch (token)
    {
    case ADD: return IC_ADD;
    case SUB: return IC_SUB;
    case MUL: return IC_MUL;
    case DIV: return IC_DIV;
    case MOD: return IC_MOD;
    case EQUAL: return IC_EQUAL;
    case NEQ: return IC_NEQ;
    case GREATER: return IC_GREATER;
    case LESS: return IC_LESS;
    case GEQ: return IC_GEQ;
    case LEQ: return IC_LEQ;
    case AND: return IC_AND;
    case OR: return IC_OR;
    case NOT: return IC_NOT;
    default: return IC_MOVE;
    }
}

InterCodeOperand *InterCodeOperand::createLiteral(int i)
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->type = InterCodeOperandType::I_LITERAL;
    ret->value.ival = i;
    ret->varType = SymbolType::INT_S;
    return ret;
}

InterCodeOperand *InterCodeOperand::createLiteral(double d)
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->type = InterCodeOperandType::D_LITERAL;
    ret->value.dval = d;
    ret->varType = SymbolType::DOUBLE_S;
    return ret;
}

InterCodeOperand *InterCodeOperand::createLiteral(string s)
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->type = InterCodeOperandType::S_LITERAL;
    ret->value.sval = s;
    ret->varType = SymbolType::STRING_S;
    return ret;
}

// id=0: 以最后一个中间变量创建操作数   id=1: 创建新的中间变量作为操作数
InterCodeOperand *InterCodeOperand::createTemp(VarType varType, int id, SymbolTable *table)
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->type = InterCodeOperandType::TEMP;
    ret->pTable = table;
    if (id == 0)
    {
        ret->value.ival = tempCount - 1;
        ret->varType = lastVarType;
    }
    else if (id == -1)
    {
        ret->value.ival = tempCount++;
        ret->varType = varType;
        lastVarType = varType;
    }
    else
    {
        ret->value.ival = id;
        ret->varType = varType;
        lastVarType = varType;
    }
    return ret;
}

InterCodeOperand *InterCodeOperand::createVar(string varName, VarType varType, SymbolTable *table)
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->pTable = table;
    ret->type = InterCodeOperandType::ORI_ID;
    ret->value.sval = varName;
    ret->varType = varType;
    return ret;
}

void InterCode::printOperand(InterCodeOperand *operand)
{
    if (operand == nullptr)
        cout << "null         ";
    else
        operand->print();
}

void InterCode::print()
{
    cout << "[dst]";
    printOperand(dst);
    cout << "  [src1]";
    printOperand(src1);
    cout << "  [src2]";
    printOperand(src2);
    cout << "  [operator]" << operators[optr] << std::endl;
}

#include <iomanip>

void InterCodeOperand::print()
{
    switch (type)
    {
    case I_LITERAL: cout << std::left << std::setw(13) << value.ival; break;
    case D_LITERAL: cout << std::left << std::setw(13) << value.dval; break;
    case S_LITERAL: cout << std::left << std::setw(13) << value.sval; break;
    case ORI_ID: cout << "*" << std::left << std::setw(12) <<  value.sval; break;
    case TEMP: cout << "t" << std::left << std::setw(12) << value.ival; break;
    default: break;
    }
}

void InterCodeList::add(InterCode code)
{
    codeList.push_back(code);
}

void InterCodeList::print()
{
    cout << "#####################Intermediate Codes###################\n";
    for(int i = 0; i < codeList.size(); i++)
    {
        cout << i << ": ";
        codeList[i].print();
    }
    cout << "##################End Intermediate Codes#################\n";
}