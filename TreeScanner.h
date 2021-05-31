#ifndef __TREE_SCANNER__
#define __TREE_SCANNER__

#include "utility.h"
#include "SymbolTable.h"
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
    void _firstScan(ASTNode *node, SymbolTable &table);
    void scanProgram(ASTNode *node, SymbolTable &table);
    void scanFunction(ASTNode *node, SymbolTable &table);
    void scanClass(ASTNode *node, SymbolTable &table);
    void scanParas(const vector<ASTNode*> &paraNodes, SymbolTable &funcTable);
    SymbolTableItem scanPara(ASTNode *paraNode, int offset);
};

#endif