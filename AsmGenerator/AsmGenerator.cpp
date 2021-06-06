#include "AsmGenerator.h"

AsmGenerator *AsmGenerator::generator = nullptr;

int AsmGenerator::codeCnt = 0;

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

    //ss << "[" << codeCnt++ << "]\n";
    switch (code.optr)
    {
    case IC_ADD:
        doAdd(code);
        break;
    case IC_SUB:
        doSub(code);
        break;
    case IC_MUL:
    case IC_DIV:
        doMulDiv(code);
        break;
    case IC_MOD:
    case IC_AND:
    case IC_OR:
    case IC_NOT:
        doModLogic(code);
        break;
    case IC_EQUAL:
    case IC_NEQ:
    case IC_GREATER:
    case IC_LESS:
    case IC_GEQ:
    case IC_LEQ:
        doCompare(code);
        break;
    case IC_OFFSET:
        doOffset(code);
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
    int nArgs = curFuncTable->getSize() - item->getExtraInfo();
    ss << code.dst->getStringVal() + ":\n";
    ss << "    push %rbp\n";
    ss << "    mov %rsp, %rbp\n";
    ss << "    sub $" << nArgs * 8 << ", %rsp\n";
}

void AsmGenerator::doJump(InterCode &code)
{
    string dst_label = code.dst->getStringVal();
    if (code.optr == InterCodeOperator::IC_JMP)
    {
        ss << "    jmp " << dst_label << "\n";
        return;
    }
    string reg = getReg();
    moveToReg(code.src1, reg);
    ss << "    cmp $0, %" << reg << "\n";
    ss << "    " << (code.optr == InterCodeOperator::IC_JZ ? "je" : "jne") << " " << dst_label << "\n";
}

// dst = src1[src2]
void AsmGenerator::doOffset(InterCode &code)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *src2 = code.src2;
    InterCodeOperand *dst = code.dst;

    string reg1 = getReg();   // 为第一个源操作数分配寄存器
    setRegUse(reg1, 99999);   // 暂时将其占用
    moveToReg(src1, reg1);    // 将第一个操作数移入该寄存器
    string reg2 = getReg();   // 为第二个源操作数分配寄存器
    setRegUse(reg2, 99999);   // 暂时将其占用
    moveToReg(src2, reg2);    // 将第二个操作数移入该寄存器
    string regDst = getReg();
    backRegUse(reg1);   // 取消两个寄存器的占用
    backRegUse(reg2);
    ss << "    mov (%" << reg1 << ", %" << reg2 << "), %" << regDst << "\n";  // 执行运算
    if (dst->getType() == InterCodeOperandType::TEMP)
        setRegUse(regDst, dst->getIntVal());
    else
        moveFromReg(regDst, dst);
}

void AsmGenerator::doIntOperate(InterCode &code, string optr)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *src2 = code.src2;
    InterCodeOperand *dst = code.dst;

    string reg1 = getReg();   // 为第一个源操作数分配寄存器
    setRegUse(reg1, 99999);   // 暂时将其占用
    moveToReg(src1, reg1);    // 将第一个操作数移入该寄存器
    string reg2 = getReg();   // 为第二个源操作数分配寄存器
    setRegUse(reg2, 99999);   // 暂时将其占用
    moveToReg(src2, reg2);    // 将第二个操作数移入该寄存器
    backRegUse(reg1);   // 取消两个寄存器的占用
    backRegUse(reg2);
    ss << "    " << optr << " %" << reg2 << ", %" << reg1 << "\n";  // 执行运算
    if (dst->getType() == InterCodeOperandType::TEMP)
        setRegUse(reg1, dst->getIntVal());
    else
        moveFromReg(reg1, dst);
}

void AsmGenerator::doIntOperateSingle(InterCode &code, string optr)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *dst = code.dst;

    string reg1 = getReg();   // 为第一个源操作数分配寄存器
    moveToReg(src1, reg1);    // 将第一个操作数移入该寄存器

    ss << "    " << optr << " %" << reg1 << "\n";  // 执行运算
    if (dst->getType() == InterCodeOperandType::TEMP)
        setRegUse(reg1, dst->getIntVal());
    else
        moveFromReg(reg1, dst);

}

void AsmGenerator::doLogicOperate(InterCode &code, string optr)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *src2 = code.src2;
    InterCodeOperand *dst = code.dst;

    moveToReg(src1, "rdi");    // 将第一个操作数移入该寄存器
    moveToReg(src2, "rsi");    // 将第二个操作数移入该寄存器
    ss << "    call __do_logic_" << optr << "\n";

    moveFromReg("rax", dst);
}

void AsmGenerator::doCmpOperate(InterCode &code, string optr)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *src2 = code.src2;
    InterCodeOperand *dst = code.dst;

    moveToReg(src1, "rdi");    // 将第一个操作数移入该寄存器
    moveToReg(src2, "rsi");    // 将第二个操作数移入该寄存器
    ss << "    call __do_cmp_" << optr << "\n";

    moveFromReg("rax", dst);
}

void AsmGenerator::doLogicOperateSingle(InterCode &code, string optr)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *dst = code.dst;

    moveToReg(src1, "rdi");    // 将第一个操作数移入该寄存器

    ss << "    call __do_logic_" << optr << "\n";

    moveFromReg("rax", dst);
}

void AsmGenerator::doAdd(InterCode &code)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *src2 = code.src2;
    InterCodeOperand *dst = code.dst;
    VarType type1 = src1->getVarType();
    VarType type2 = src2->getVarType();
    if (isInt(type1) && isInt(type2))
    {
        doIntOperate(code, "addq");
    }
}

void AsmGenerator::doSub(InterCode &code)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *src2 = code.src2;
    InterCodeOperand *dst = code.dst;
    VarType type1 = src1->getVarType();
    VarType type2 = src2->getVarType();
    if (isInt(type1) && isInt(type2))
    {
        doIntOperate(code, "subq");
    }
}

void AsmGenerator::doMulDiv(InterCode &code)
{
    InterCodeOperand *src1 = code.src1;
    InterCodeOperand *src2 = code.src2;
    InterCodeOperand *dst = code.dst;
    VarType type1 = src1->getVarType();
    VarType type2 = src2->getVarType();
    if (isInt(type1) && isInt(type2))
    {
        doIntOperate(code, code.optr == InterCodeOperator::IC_MUL ? "imulq" : "idivq");
    }
}

void AsmGenerator::doModLogic(InterCode &code)
{
    switch (code.optr)
    {
    case InterCodeOperator::IC_MOD:
        doIntOperate(code, "");
        break;
    case InterCodeOperator::IC_AND:
        doLogicOperate(code, "and");
        break;
    case InterCodeOperator::IC_OR:
        doLogicOperate(code, "or");
        break;
    case InterCodeOperator::IC_NOT:
        doLogicOperateSingle(code, "not");
        break;
    default:
        break;
    }
}

void AsmGenerator::doCompare(InterCode &code)
{
    switch (code.optr)
    {
    case InterCodeOperator::IC_EQUAL:
        doCmpOperate(code, "equal");
        break;
    case InterCodeOperator::IC_NEQ:
        doCmpOperate(code, "neq");
        break;
    case InterCodeOperator::IC_GEQ:
        doCmpOperate(code, "geq");
        break;
    case InterCodeOperator::IC_LEQ:
        doCmpOperate(code, "leq");
        break;
    case InterCodeOperator::IC_GREATER:
        doCmpOperate(code, "greater");
        break;
    case InterCodeOperator::IC_LESS:
        doCmpOperate(code, "less");
        break;
    default:
        break;
    }
}

void AsmGenerator::doMove(InterCode &code)
{
    if (code.src1 == nullptr || code.src1->getIntVal() == -1)  // dst = src2
    {
        string reg = getReg();  // 临时获取一个可用的寄存器
        moveToReg(code.src2, reg);
        moveFromReg(reg, code.dst);
    }
    else  // dst[src1] = src2
    {
        string regDst = getReg();  // 获取两个可用的寄存器，标记为不可用
        setRegUse(regDst, 99999);
        string regSrc1 = getReg();  
        setRegUse(regSrc1, 99999);
        string regSrc2 = getReg();  // 获取另一个可用的寄存器
        moveToReg(code.src2, regSrc2);
        moveToReg(code.dst, regDst);
        moveToReg(code.src1, regSrc1);
        backRegUse(regDst);
        backRegUse(regSrc1);
        ss << "    mov %" << regSrc2 << ", (%" << regDst << ", %" << regSrc1 << ")\n";
        // ss << "    add %" << regSrc << ", %" << reg << "\n";
        // moveToReg(code.src2, reg);
        // moveFromReg(reg, "(%" + reg + ")");
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
    if (oprand->getVarType() & SymbolType::FUNC_S)   // 函数返回值
        ss << "    mov %rax, %" << reg << "\n";
    else if (type == InterCodeOperandType::I_LITERAL)
        ss << "    mov $" << oprand->getIntVal() << ", %" << reg << "\n";
    else if (type == InterCodeOperandType::ORI_ID)
        ss << "    mov " << idToOffset(oprand->getStringVal()) << "(%rbp), %" << reg << "\n";
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
        ss << "    mov %" << reg << ", " << idToOffset(oprand->getStringVal()) << "(%rbp)\n";
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
        if (tempVarID == regsUse[i])
            return regs[i];
        if (regsUse[i] == -1 && tempVarID == -1)
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
            regsUseTmp[i] = regsUse[i];
            regsUse[i] = tempVarID;
            return;
        }
}

void AsmGenerator::backRegUse(string reg)
{
    for (int i = 0; i < REG_CNT; i++)
        if (regs[i] == reg)
        {
            regsUse[i] = regsUseTmp[i];
            regsUseTmp[i] = -1;
            return;
        }
}

int AsmGenerator::idToOffset(string id)
{
    return curFuncTable->getItem(id)->getOffset();
}
