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

    std::stringstream ssData;   // 用于生成数据段字符串
    std::stringstream ssText;   // 用于生成代码段字符串

    static const int REG_CNT = 7;
    string regs[REG_CNT] = {"rbx", "rcx", "rdx", "r10", "r11", "r12", "r13"};
    int regsUse[REG_CNT] = {-1, -1, -1, -1, -1, -1, -1};
    int regsUseTmp[REG_CNT] = {-1, -1, -1, -1, -1, -1, -1};

    static int codeCnt;
    void generateStartPart();
    void translateOneIC(InterCode &code);

    void doLabel(InterCode &code);
    void doFunc(InterCode &code);
    void doJump(InterCode &code);
    void doOffset(InterCode &code);
    void doStringOperate(InterCode &code, string optr);
    void doDoubleIntOperate(InterCode &code, string optr);
    void doDoubleOperate(InterCode &code, string optr);
    void doIntOperate(InterCode &code, string optr);
    void doIntOperateSingle(InterCode &code, string optr);
    void doLogicOperate(InterCode &code, string optr);
    void doCmpOperate(InterCode &code, string optr);
    void doLogicOperateSingle(InterCode &code, string optr);
    void doAdd(InterCode &code);
    void doSub(InterCode &code);
    void doMulDiv(InterCode &code);
    void doModLogic(InterCode &code);
    void doCompare(InterCode &code);
    void doMove(InterCode &code);
    void doCall(InterCode &code);
    void doArg(InterCode &code);
    void doRet(InterCode &code);
    void doRRet(InterCode &code);

    void moveToReg(InterCodeOperand *oprand, string reg);
    void moveFromReg(string reg, InterCodeOperand *oprand);
    void moveFromReg(string reg, string expr);
    string getReg(int tempVarID=-1);
    void setRegUse(string reg, int tempVarID);
    void backRegUse(string reg);

    int idToOffset(string id);
};

#endif