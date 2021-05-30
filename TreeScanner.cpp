#include "TreeScanner.h"

TreeScanner *TreeScanner::scanner = nullptr;

TreeScanner &TreeScanner::getScanner()
{
    if (scanner == nullptr)
        scanner = new TreeScanner();
    return *scanner;
}

SymbolTable *TreeScanner::firstScan(AST &tree)
{
    if (tree.getRoot() == nullptr)
        return nullptr;
    SymbolTable *table = new SymbolTable();
    //_firstScan(tree.getRoot()->child, *table);
    return table;
}

void TreeScanner::_firstScan(ASTNode *node, SymbolTable &table)
{
    if (node == nullptr)
        return;
    else if (node->symbol == program)
        return scanProgram(node, table);
    else if (node->symbol == function)
        return scanFunction(node, table);
    else if (node->symbol == class_def)
        return scanClass(node, table);
}

void TreeScanner::scanProgram(ASTNode *programNode, SymbolTable &table)
{
    // ASTNode *t = programNode->child;
    // for (; t != nullptr; t = t->sibling)
    // {
    //     _firstScan(t, table);
    // }
}

void TreeScanner::scanFunction(ASTNode *functionNode, SymbolTable &table)
{
    //int type = createSymbolType(functionNode->child->symbol, true, false, false);
}

void TreeScanner::scanClass(ASTNode *functionNode, SymbolTable &table)
{

}