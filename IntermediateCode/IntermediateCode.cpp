#include "IntermediateCode.h"

const string InterCode::operators[] = {"add", "sub", "mul", "div", "mod", "and", "or", "not", "jl", "jle", "jeq", "label"};

int InterCodeOperand::tempCount = 0;

InterCodeOperand *InterCodeOperand::createLiteral(int i)
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->type = InterCodeOperandType::I_LITERAL;
    ret->value.ival = i;
    return ret;
}

InterCodeOperand *InterCodeOperand::createLiteral(double d)
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->type = InterCodeOperandType::D_LITERAL;
    ret->value.dval = d;
    return ret;
}

InterCodeOperand *InterCodeOperand::createLiteral(string s)
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->type = InterCodeOperandType::S_LITERAL;
    ret->value.sval = s;
    return ret;
}

InterCodeOperand *InterCodeOperand::createTemp()
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->type = InterCodeOperandType::TEMP;
    ret->value.ival = tempCount++;
    return ret;
}

InterCodeOperand *InterCodeOperand::createVar(string varName)
{
    InterCodeOperand *ret = new InterCodeOperand();
    ret->type = InterCodeOperandType::ORI_ID;
    ret->value.sval = varName;
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