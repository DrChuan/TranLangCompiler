#ifndef __TREE_SCANNER__
#define __TREE_SCANNER__

#include "utility.h"
#include "SymbolTable/SymbolTable.h"
#include "AST.h"

// 单例类，用于对AST进行扫描分析
class TreeScanner
{
public:
    static TreeScanner &getScanner();    // 获取单例对象
    SymbolTable *firstScan(AST &tree);   // 构建符号表并返回

private:
    TreeScanner() {}
    static TreeScanner *scanner;
    void _firstScan(ASTNode *node, SymbolTable &table);  // 执行实际的扫描和符号表构造动作
    void scanProgram(ASTNode *node, SymbolTable &table);  // 扫描program结点
    void scanFunction(ASTNode *node, SymbolTable &table);  // 扫描function结点
    void scanClass(ASTNode *node, SymbolTable &table);   // 扫描class结点
    void scanParas(const vector<ASTNode*> &paraNodes, SymbolTable &funcTable);  // 扫描para_list
    SymbolTableItem scanPara(ASTNode *paraNode, int offset);   // 扫描para
    void scanStmts(const vector<ASTNode*> &stmtNodes, SymbolTable &funcTable);  // 扫描function结点下的statements
    void _scanStmts(const vector<ASTNode*> &stmtNodes, SymbolTable &funcTable, int &offset);  // 扫描子statements（if,while等语句的statements）
    SymbolTableItem scanDecl(ASTNode *declNode, int offset);  // 扫描declaration
};

#endif