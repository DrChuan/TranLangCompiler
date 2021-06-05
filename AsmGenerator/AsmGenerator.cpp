#include "AsmGenerator.h"

AsmGenerator *AsmGenerator::generator = nullptr;

AsmGenerator &AsmGenerator::getGenerator()
{
    if (generator == nullptr)
        generator = new AsmGenerator();
    return *generator;
}

void AsmGenerator::generateAsm(std::ostream &out, SymbolTable &table, InterCodeList &interCodes)
{
    this->out = &out;
    this->table = &table;
    for (int i = 0; i < interCodes.size(); i++)
        translateOneIC(interCodes.get(i));
    string s;
    s = ss.str();
    out << s;
    for (int i = 0; i < errorMsgs.size(); i++)
        cout << errorMsgs[i] << std::endl;
    if (errorMsgs.size() == 0)
        cout << "Successfully generate assembly codes!\n";
}

void AsmGenerator::translateOneIC(InterCode &code)
{
    static int i = 0;
    ss << "[" << i++ << "]\n";
    switch (code.optr)
    {
    case IC_ADD:
    case IC_SUB:
    case IC_MUL:
    case IC_DIV:
    case IC_MOD:
    case IC_EQUAL:
    case IC_NEQ:
    case IC_GREATER:
    case IC_LESS:
    case IC_GEQ:
    case IC_LEQ:
    case IC_AND:
    case IC_OR:
    case IC_NOT:
    case IC_OFFSET:
        doCalculate(code);
        break;
    case IC_JMP:
    case IC_JZ:
    case IC_JNZ:
        doJump(code);
        break;
    case IC_FUNC:
        doFunc(code);
        break;
    case IC_LABEL:
        doLabel(code);
        break;
    case IC_MOVE:
        doMove(code);
        break;
    case IC_ARG:
        doArg(code);
        break;
    case IC_CALL:
        doCall(code);
        break;
    case IC_RET:
        doRet(code);
        break;
    default:
        break;
    }
}

void AsmGenerator::doLabel(InterCode &code)
{
    ss << code.dst->getStringVal() + ":\n";
}

void AsmGenerator::doFunc(InterCode &code)
{
    string id = code.dst->getStringVal();
    SymbolTableItem *item = table->getItem(id);
    curFuncTable = item->getSubTable();
    int nArgs = item->getExtraInfo();
    ss << code.dst->getStringVal() + ":\n";
    ss << "    push %rbp\n";
    ss << "    mov %rsp, %rbp\n";
    ss << "    sub $" << nArgs * 8 << ", %rsp\n";
}

void AsmGenerator::doJump(InterCode &code)
{

}

void AsmGenerator::doCalculate(InterCode &code)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *src2 = code.src2;
    InterCodeOperand *dst = code.dst;

}

void AsmGenerator::doArithAndLogic(InterCode code)
{

}

void AsmGenerator::doCompare(InterCode code)
{

}

void AsmGenerator::doMove(InterCode &code)
{
    if (code.src1 == nullptr || code.src1->getIntVal() == -1)  // dst = src2
    {
        string reg = getReg();
        moveToReg(code.src2, reg);
        moveFromReg(reg, code.dst);
    }
    else  // dst[src1] = src2
    {
        string reg = getReg();
        setRegUse(reg, 99999);
        string regSrc = getReg();
        moveToReg(code.dst, reg);
        moveToReg(code.src1, regSrc);
        setRegUse(reg, -1);
        ss << "    add %" << regSrc << ", %" << reg << "\n";
        moveToReg(code.src2, reg);
        moveFromReg(reg, "(%" + reg + ")");
    }
}

void AsmGenerator::doCall(InterCode &code)
{
    ss << "    call " << code.dst->getStringVal() << "\n";
}

void AsmGenerator::doArg(InterCode &code)
{
    moveToReg(code.dst, "rax");
    ss << "    push %rax\n";
}

void AsmGenerator::doRet(InterCode &code)
{
    if (code.dst != nullptr)  // 有返回值的情形，将返回值放入%rax
        moveToReg(code.dst, "rax");
    ss << "    leave\n";
    ss << "    ret\n";
}

void AsmGenerator::moveToReg(InterCodeOperand *oprand, string reg)
{
    InterCodeOperandType type = oprand->getType();
    if (type == InterCodeOperandType::I_LITERAL)
        ss << "    mov $" << oprand->getIntVal() << ", %" << reg << "\n";
    else if (type == InterCodeOperandType::ORI_ID)
        ss << "    mov $" << idToOffset(oprand->getStringVal()) << "(%rbp), %" << reg << "\n";
    else if (type == InterCodeOperandType::TEMP)
    {
        string treg = getReg(oprand->getIntVal());
        if (reg != treg)
            ss << "    mov %" << treg << ", %" << reg << "\n";
    }
    
}

void AsmGenerator::moveFromReg(string reg, InterCodeOperand *oprand)
{
    InterCodeOperandType type = oprand->getType();
    if (type == InterCodeOperandType::ORI_ID)
        ss << "    mov %" << reg << ", $" << idToOffset(oprand->getStringVal()) << "(%rbp)\n";
    else if (type == InterCodeOperandType::TEMP)
    {
        string treg = getReg();
        setRegUse(reg, oprand->getIntVal());
        ss << "    mov %" << reg << ", %" << treg << "\n";
    }
}

void AsmGenerator::moveFromReg(string reg, string expr)
{
    ss << "    mov %" << reg << ", " << expr << "\n";
}

string AsmGenerator::getReg(int tempVarID)
{
    int min_id = 0;
    for (int i = 0; i < REG_CNT; i++)
    {
        if (regsUse[i] == -1)
            return regs[i];
        if (regsUse[i] < regsUse[min_id])
            min_id = i;
    }
    return regs[min_id];
}

void AsmGenerator::setRegUse(string reg, int tempVarID)
{
    for (int i = 0; i < REG_CNT; i++)
        if (regs[i] == reg)
        {
            regsUse[i] = tempVarID;
            return;
        }
}

int AsmGenerator::idToOffset(string id)
{
    return curFuncTable->getItem(id)->getOffset();
}

bool AsmGenerator::isInt(InterCodeOperand *oprand)
{
    return oprand->getType() == InterCodeOperandType::I_LITERAL || oprand->getType() == InterCodeOperandType::ORI_ID;
}

bool AsmGenerator::isString(InterCodeOperand *oprand)
{

}