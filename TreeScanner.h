#ifndef __TREE_SCANNER__

#define __TREE_SCANNER__

#include "utility.h"
#include "SymbolTable/SymbolTable.h"
#include "IntermediateCode/IntermediateCode.h"
#include "AST.h"

// 单例类，用于对AST进行扫描分析
class TreeScanner
{
public:
    static TreeScanner &getScanner();    // 获取单例对象
    SymbolTable *firstScan(AST &tree);   // 构建符号表并返回
    InterCodeList *secondScan(AST &tree, SymbolTable &table);  // 生成中间代码
    bool hasError() const { return errorMsgs.size() != 0; }
    void printErrors() const;
private:
    TreeScanner() {}
    static TreeScanner *scanner;

    int while_cnt;
    int br_cnt;

    vector<string> errorMsgs;
    // 第一次扫描，创建符号表
    void _firstScan(ASTNode *node, SymbolTable &table);  // 执行实际的扫描和符号表构造动作
    void loadLibFunction(SymbolTable &table);
    void scanProgram_1(ASTNode *node, SymbolTable &table);  // 扫描program结点
    void scanFunction_1(ASTNode *node, SymbolTable &table);  // 扫描function结点
    void scanClass_1(ASTNode *node, SymbolTable &table);   // 扫描class结点
    void scanParas_1(const vector<ASTNode*> &paraNodes, SymbolTable &funcTable);  // 扫描para_list
    SymbolTableItem scanPara_1(ASTNode *paraNode, int offset);   // 扫描para
    void scanStmts_1(const vector<ASTNode*> &stmtNodes, SymbolTable &funcTable);  // 扫描function结点下的statements
    void _scanStmts_1(const vector<ASTNode*> &stmtNodes, SymbolTable &funcTable, int &offset);  // 扫描子statements（if,while等语句的statements）
    SymbolTableItem scanDecl_1(ASTNode *declNode, int offset);  // 扫描declaration
    // 第二次扫描，生成中间代码
    void _secondScan(ASTNode *node, SymbolTable &table, InterCodeList *result);
    void scanProgram_2(ASTNode *node, SymbolTable &table, InterCodeList *result);
    void scanFunction_2(ASTNode *node, SymbolTable &table, InterCodeList *result);
    void scanStmts_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scanDecl_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void _scanExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scanAssignExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scan1Exp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scan2Exp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scanIndexExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scanExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scanWhile_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scanIf_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scanCallExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    void scanReturn_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    InterCodeOperand *terminalToOperand(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result);
    InterCodeOperand *__terminalToOperand(ASTNode *node, SymbolTable &localTable);  // 如果结点的孩子是变量名或字面量，则构造相应的中间代码操作数返回；否则返回nullptr
    //bool checkOperand(string id, bool isArray, SymbolTable &globalTable, SymbolTable &localTable);
    bool checkOperandType(InterCodeOperand *src1, InterCodeOperand *src2, InterCodeOperator optr);
    VarType inferDstType(InterCodeOperand *src1, InterCodeOperand *src2, InterCodeOperator optr);
    void addErrorMsg(int line, string msg);
};

#endif