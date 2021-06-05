#ifndef __ASM_GENERATOR__
#define __ASM_GENERATOR__

#include "../utility.h"
#include "../IntermediateCode/IntermediateCode.h"
#include "../SymbolTable/SymbolTable.h"
#include <ostream>
#include <sstream>

class AsmGenerator
{
public:
    static AsmGenerator &getGenerator();
    void generateAsm(std::ostream &out, SymbolTable &table, InterCodeList &interCodes);
private:
    AsmGenerator() {}
    static AsmGenerator *generator;

    vector<string> errorMsgs;

    std::ostream *out;     // 最终输出汇编代码的输出流
    SymbolTable *table;    // 全局符号表
    SymbolTable *curFuncTable;    // 当前所在的函数的符号表

    std::stringstream ss;   // 用于生成汇编代码字符串

    static const int REG_CNT = 7;
    string regs[REG_CNT] = {"rbx", "rcx", "rdx", "r10", "r11", "r12", "r13"};
    int regsUse[REG_CNT] = {-1, -1, -1, -1, -1, -1, -1};

    void translateOneIC(InterCode &code);

    void doLabel(InterCode &code);
    void doFunc(InterCode &code);
    void doJump(InterCode &code);
    void doCalculate(InterCode &code);
    void doArithAndLogic(InterCode code);
    void doCompare(InterCode code);
    void doMove(InterCode &code);
    void doCall(InterCode &code);
    void doArg(InterCode &code);
    void doRet(InterCode &code);

    void moveToReg(InterCodeOperand *oprand, string reg);
    void moveFromReg(string reg, InterCodeOperand *oprand);
    void moveFromReg(string reg, string expr);
    string getReg(int tempVarID=-1);
    void setRegUse(string reg, int tempVarID);

    int idToOffset(string id);

    bool isInt(InterCodeOperand *oprand);
    bool isString(InterCodeOperand *oprand);
};

#endif