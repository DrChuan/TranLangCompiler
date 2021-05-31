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
        return scanProgram_1(node, table);
    else if (node->is(function))
        return scanFunction_1(node, table);
    else if (node->is(class_def))
        return scanClass_1(node, table);
}

// 扫描program结点，将其中的函数和类定义加入到总符号表中
void TreeScanner::scanProgram_1(ASTNode *programNode, SymbolTable &table)
{
    for (int i = 0; i < programNode->childCount(); i++)
        _firstScan(programNode->getChild(i), table);
}

// 扫描函数结点，创建函数的子符号表，并在总符号表中添加该函数对应的项
void TreeScanner::scanFunction_1(ASTNode *functionNode, SymbolTable &table)
{
    // 从函数结点中获取相关的函数信息
    int type = createSymbolType(functionNode->getChild(0)->getSymbol(), true, false, false);
    string name = functionNode->getChild(1)->getStringValue();
    const vector<ASTNode*> &paraNodes = functionNode->getChild(2)->getChildren();
    const vector<ASTNode*> &stmtNodes = functionNode->getChild(3)->getChildren();
    
    SymbolTable *subTable = new SymbolTable();  // 创建函数的子符号表
    table.insertItem(name, SymbolTableItem(0, type, paraNodes.size(), subTable));  // 将函数保存到总符号表中
    scanParas_1(paraNodes, *subTable);   // 将参数填入符号表
    scanStmts_1(stmtNodes, *subTable);   // 将局部变量填入符号表
}

void TreeScanner::scanParas_1(const vector<ASTNode*> &paraNodes, SymbolTable &funcTable)
{
    int offset = 0;
    for (int i = 0; i < paraNodes.size(); i++)
    {
        funcTable.insertItem(paraNodes[i]->getLastChild()->getStringValue(), scanPara_1(paraNodes[i], offset));
        offset += 8;
    }
}

SymbolTableItem TreeScanner::scanPara_1(ASTNode *paraNode, int offset)
{
    ASTNode *typeNode = paraNode->getChild(0);
    bool isArray = paraNode->getChild(1)->is(LB);
    int type = createSymbolType(typeNode->getSymbol(), false, false, isArray);
    return SymbolTableItem(offset, type);
}

void TreeScanner::_scanStmts_1(const vector<ASTNode*> &stmtNodes, SymbolTable &funcTable, int &offset)
{
    // 遍历所有statements
    for (int i = 0; i < stmtNodes.size(); i++)
    {
        // 对于声明语句，将相应符号放入符号表
        if (stmtNodes[i]->is(dcl_statement))
        {
            funcTable.insertItem(stmtNodes[i]->getChild(0)->getChild(1)->getStringValue(), scanPara_1(stmtNodes[i]->getChild(0), offset));
            offset += 8;
        }
        // 对于条件语句或循环语句，对其statements部分递归调用
        else if (stmtNodes[i]->is(if_statement) || stmtNodes[i]->is(while_loop))
        {
            _scanStmts_1(stmtNodes[i]->getChild(1)->getChildren(), funcTable, offset);
        }
        // 对于条件语句的else部分，根据子结点数目判断其为elsif块或else块，并对statements部分递归调用
        else if (stmtNodes[i]->is(else_part))
        {
            if (stmtNodes[i]->childCount() == 1)
                _scanStmts_1(stmtNodes[i]->getChild(0)->getChildren(), funcTable, offset);
            else
                _scanStmts_1(stmtNodes[i]->getChild(1)->getChildren(), funcTable, offset);
        }
    }
}

void TreeScanner::scanStmts_1(const vector<ASTNode*> &stmtNodes, SymbolTable &funcTable)
{
    int offset = 0;
    _scanStmts_1(stmtNodes, funcTable, offset);
}

SymbolTableItem TreeScanner::scanDecl_1(ASTNode *declNode, int offset)
{
    ASTNode *typeNode = declNode->getChild(0);
    bool isArray = declNode->childCount() == 3;
    int type = createSymbolType(typeNode->getSymbol(), false, false, isArray);
    return SymbolTableItem(offset, type);
}

void TreeScanner::scanClass_1(ASTNode *functionNode, SymbolTable &table)
{
    
}

InterCodeList *TreeScanner::secondScan(AST &tree, SymbolTable &table)
{
    if (tree.getRoot() == nullptr)  // 检查根节点是否为空
        return nullptr;
    InterCodeList *result = new InterCodeList();
    _secondScan(tree.getRoot()->getChild(0), table, result);
    return result;
}

void TreeScanner::_secondScan(ASTNode *node, SymbolTable &table, InterCodeList *result)
{
    if (node == nullptr)
        return;
    else if (node->is(program))
        scanProgram_2(node, table, result);
    else if (node->is(function))
        scanFunction_2(node, table, result);
}

void TreeScanner::scanProgram_2(ASTNode *node, SymbolTable &table, InterCodeList *result)
{
    for (int i = 0; i < node->childCount(); i++)
        _secondScan(node->getChild(i), table, result);
}

void TreeScanner::scanFunction_2(ASTNode *node, SymbolTable &table, InterCodeList *result)
{
    ASTNode *IDNode = node->getChild(1);
    SymbolTable *funcTable = table.getItem(IDNode->getStringValue())->getSubTable();
    result->add(InterCode(InterCodeOperator::IC_LABEL, InterCodeOperand::createLiteral(IDNode->getStringValue())));
}