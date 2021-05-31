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
    if (tree.getRoot() == nullptr)  // 检查根节点是否为空
        return nullptr;
    SymbolTable *table = new SymbolTable();  // 初始化总符号表
    _firstScan(tree.getRoot()->getChild(0), *table);  // 对start结点下的program结点进行扫描
    return table;
}

void TreeScanner::_firstScan(ASTNode *node, SymbolTable &table)
{
    if (node == nullptr)
        return;
    else if (node->is(program))
        return scanProgram(node, table);
    else if (node->is(function))
        return scanFunction(node, table);
    else if (node->is(class_def))
        return scanClass(node, table);
}

// 扫描program结点，将其中的函数和类定义加入到总符号表中
void TreeScanner::scanProgram(ASTNode *programNode, SymbolTable &table)
{
    for (int i = 0; i < programNode->childCount(); i++)
        _firstScan(programNode->getChild(i), table);
}

// 扫描函数结点，创建函数的子符号表，并在总符号表中添加该函数对应的项
void TreeScanner::scanFunction(ASTNode *functionNode, SymbolTable &table)
{
    // 从函数结点中获取相关的函数信息
    int type = createSymbolType(functionNode->getChild(0)->getSymbol(), true, false, false);
    string name = functionNode->getChild(1)->getStringValue();
    const vector<ASTNode*> &paraNodes = functionNode->getChild(2)->getChildren();
    const vector<ASTNode*> &stmtNodes = functionNode->getChild(3)->getChildren();
    
    SymbolTable *subTable = new SymbolTable();  // 创建函数的子符号表
    table.insertItem(name, SymbolTableItem(0, type, paraNodes.size(), subTable));  // 将函数保存到总符号表中
    scanParas(paraNodes, *subTable);   // 将参数填入符号表
}

void TreeScanner::scanParas(const vector<ASTNode*> &paraNodes, SymbolTable &funcTable)
{
    int offset = 0;
    for (int i = 0; i < paraNodes.size(); i++)
    {
        funcTable.insertItem(paraNodes[i]->getLastChild()->getStringValue(), scanPara(paraNodes[i], offset));
        offset += 8;
    }
}

SymbolTableItem TreeScanner::scanPara(ASTNode *paraNode, int offset)
{
    ASTNode *typeNode = paraNode->getChild(0);
    bool isArray = paraNode->getChild(1)->is(LB);
    int type = createSymbolType(typeNode->getSymbol(), false, false, isArray);
    return SymbolTableItem(offset, type);
}

void TreeScanner::scanClass(ASTNode *functionNode, SymbolTable &table)
{

}