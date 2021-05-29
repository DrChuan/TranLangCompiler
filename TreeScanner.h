#ifndef __TREE_SCANNER__
#define __TREE_SCANNER__

#include "utility.h"
#include "SymbolTable.h"
#include "AST.h"

class TreeScanner
{
public:
    static TreeScanner &getScanner();
    SymbolTable *firstScan(AST &tree);

private:
    TreeScanner() {}
    static TreeScanner *scanner;
    void _firstScan(ASTNode *node, SymbolTable &table);
    void scanProgram(ASTNode *node, SymbolTable &table);
    void scanFunction(ASTNode *node, SymbolTable &table);
    void scanClass(ASTNode *node, SymbolTable &table);
};

#endif