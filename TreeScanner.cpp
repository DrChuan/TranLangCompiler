#include "TreeScanner.h"

TreeScanner *TreeScanner::scanner = nullptr;

TreeScanner &TreeScanner::getScanner()
{
    if (scanner == nullptr)
        scanner = new TreeScanner();
    return *scanner;
}

void TreeScanner::printErrors() const
{
    for(int i = 0; i < errorMsgs.size(); i++)
        cout << "[Error] " << errorMsgs[i] << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// 第一次扫描语法树，构建符号表
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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

// 扫描参数列表结点
void TreeScanner::scanParas_1(const vector<ASTNode*> &paraNodes, SymbolTable &funcTable)
{
    int offset = 16;  // offset从16开始（(%rbp)为old_rbp，(%rbp+8)为return_addr）
    for (int i = 0; i < paraNodes.size(); i++)
    {
        funcTable.insertItem(paraNodes[i]->getLastChild()->getStringValue(), scanPara_1(paraNodes[i], offset));
        offset += 8;
    }
}

// 扫描参数结点
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
            funcTable.insertItem(stmtNodes[i]->getChild(0)->getChild(1)->getStringValue(), scanDecl_1(stmtNodes[i]->getChild(0), offset));//(stmtNodes[i]->getChild(0)->getChild(1)->getStringValue(), scanPara_1(stmtNodes[i]->getChild(0), offset));
            offset -= 8;
        }
        // 对于条件语句或循环语句，对其statements部分递归调用
        else if (stmtNodes[i]->is(if_statement))
        {
            _scanStmts_1(stmtNodes[i]->getChild(1)->getChildren(), funcTable, offset);
            stmtNodes[i]->simplify();
        }
        else if (stmtNodes[i]->is(while_loop))
            _scanStmts_1(stmtNodes[i]->getChild(1)->getChildren(), funcTable, offset);
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
    int offset = -8;
    _scanStmts_1(stmtNodes, funcTable, offset);
}

SymbolTableItem TreeScanner::scanDecl_1(ASTNode *declNode, int offset)
{
    ASTNode *typeNode = declNode->getChild(0)->getChild(0);
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
    while_cnt = 0;
    br_cnt = 0;
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
    result->add(InterCode(InterCodeOperator::IC_FUNC, InterCodeOperand::createLiteral(IDNode->getStringValue())));
    scanStmts_2(node->getLastChild(), table, *funcTable, result);
    if (result->get(result->size() - 1).optr != InterCodeOperator::IC_RET)
        result->add(InterCode(InterCodeOperator::IC_RET));
}

void TreeScanner::scanStmts_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    for (int i = 0; i < node->childCount(); i++)
    {
        if (node->getChild(i)->is(dcl_statement))
            scanDecl_2(node->getChild(i), globalTable, localTable, result);
        else if (node->getChild(i)->is(exp_statement))
            scanExp_2(node->getChild(i), globalTable, localTable, result);
        else if (node->getChild(i)->is(while_loop))
            scanWhile_2(node->getChild(i), globalTable, localTable, result);
        else if (node->getChild(i)->is(if_statement))
            scanIf_2(node->getChild(i), globalTable, localTable, result);
        else if (node->getChild(i)->is(return_statement))
            scanReturn_2(node->getChild(i), globalTable, localTable, result);
    }
}

// 扫描声明语句
void TreeScanner::scanDecl_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    // 如果语句只有一个孩子，即没有初始化部分，则什么都不做
    if (node->childCount() == 1)
        return;
    // 否则，获取其初始化结点
    ASTNode *initNode = node->getChild(1);
    InterCodeOperand *src2 = terminalToOperand(initNode->getChild(0), globalTable, localTable, result);
    string id = node->getChild(0)->getChild(1)->getStringValue();
    // 检查类型
    VarType dstType = localTable.getItem(id)->getType();
    if (src2->getVarType() != dstType)
    {
        errorMsgs.push_back("Initialization of variable " + id + " has a wrong type!");
        return;
    }
    InterCodeOperand *dst = InterCodeOperand::createVar(id, dstType);
    result->add(InterCode(InterCodeOperator::IC_MOVE, dst, nullptr, src2));
}

void TreeScanner::_scanExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    int optr;
    if (node->childCount() == 3)
    {
        optr = node->getChild(1)->getSymbol();
        if (optr == ASSIGN)
            scanAssignExp_2(node, globalTable, localTable, result);
        else
            scan2Exp_2(node, globalTable, localTable, result);
    }
    else if (node->childCount() == 2)
    {
        scan1Exp_2(node, globalTable, localTable, result);
    }
    else if (node->childCount() == 1)
    {
        if (node->getChild(0)->getSymbol() == func_call)
            scanCallExp_2(node->getChild(0), globalTable, localTable, result);
        else  // 下标访问
            scanIndexExp_2(node->getChild(0), globalTable, localTable, result);
    }
}

// 扫描赋值语句
void TreeScanner::scanAssignExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    ASTNode *lvalNode = node->getChild(0);
    bool isArray = node->childCount() == 2;
    InterCodeOperand *src1;
    InterCodeOperand *src2;
    InterCodeOperand *dst;
    // 检查目标变量是否存在
    string id = lvalNode->getChild(0)->getStringValue();
    SymbolTableItem *item = localTable.getItem(id);
    if (item == nullptr)
    {
        errorMsgs.push_back("Identifier " + id + " does not exist in this scope!");
        return;
    }
    dst = InterCodeOperand::createVar(id, item->getType());

    if (isArray)
    {
        // 若dst不是数组类型
        if (!(dst->getVarType() & SymbolType::ARRAY_S))
        {
            errorMsgs.push_back("Cannot use index to access non-array variable " + id + "!");
            return;
        }
        ASTNode *indexExp = lvalNode->getChild(1);
        src1 = terminalToOperand(indexExp, globalTable, localTable, result);
        if (!(src1->getVarType() & SymbolType::INT_S))
        {
            errorMsgs.push_back("Index of array must be an integer expression!");
            return;
        }
    }
    else
        src1 = InterCodeOperand::createLiteral(-1);

    src2 = terminalToOperand(node->getLastChild(), globalTable, localTable, result);
    if ( (!isArray && !((src2->getVarType() & SymbolType::ARRAY_S) ^ (dst->getVarType() & SymbolType::ARRAY_S))) ||   // 不是下标访问，dst和src1的数组标志位相同
         (isArray && (dst->getVarType() & SymbolType::ARRAY_S) && !(src2->getVarType() & SymbolType::ARRAY_S))    // 是下标访问，dst是数字，src1不是数组
       )
    {
        if ((dst->getVarType() & ~SymbolType::ARRAY_S) != (src2->getVarType() & ~SymbolType::ARRAY_S))   // 如果dst和src2的基本类型不同，则报错
        {
            errorMsgs.push_back("Assign to variable " + id + " has a wrong type!");
            return;
        }
        result->add(InterCode(InterCodeOperator::IC_MOVE, dst, src1, src2));
    }
    else
        errorMsgs.push_back("Assign to variable " + id + " has a wrong type!");
}

void TreeScanner::scan1Exp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    ASTNode *expr = node->getChild(1);
    int optr = node->getChild(0)->getSymbol();
    InterCodeOperand *src1 = terminalToOperand(expr, globalTable, localTable, result);
    if (src1->getVarType() != (int)SymbolType::INT_S)
    {
        errorMsgs.push_back("Oprand for '!' is not an integer!");
        return;
    }
    InterCodeOperand *dst = InterCodeOperand::createTemp(SymbolType::INT_S, -1);
    result->add(InterCode(optrFromToken(optr), dst, src1));
}

void TreeScanner::scan2Exp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    ASTNode *left = node->getChild(0);
    ASTNode *right = node->getChild(2);
    int optr = node->getChild(1)->getSymbol();
    InterCodeOperand *src1 = terminalToOperand(left, globalTable, localTable, result);
    InterCodeOperand *src2 = terminalToOperand(right, globalTable, localTable, result);
    if (!checkOperandType(src1, src2, optrFromToken(optr)))
    {
        errorMsgs.push_back("Oprand does not match!\n");
        return;
    }
    InterCodeOperand *dst = InterCodeOperand::createTemp(inferDstType(src1, src2, optrFromToken(optr)), -1);
    result->add(InterCode(optrFromToken(optr), dst, src1, src2));
}

void TreeScanner::scanIndexExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    ASTNode *expr = node->getChild(1);
    string id = node->getChild(0)->getStringValue();
    InterCodeOperand *src1 = InterCodeOperand::createVar(id, localTable.getItem(id)->getType());
    InterCodeOperand *src2 = terminalToOperand(expr, globalTable, localTable, result);
    if (!(src2->getVarType() & SymbolType::INT_S))
    {
        errorMsgs.push_back("Index of array must be an integer expression!");
        return;
    }
    InterCodeOperand *dst = InterCodeOperand::createTemp(src1->getVarType() & ~SymbolType::ARRAY_S, -1);
    result->add(InterCode(InterCodeOperator::IC_OFFSET, dst, src1, src2));
}

void TreeScanner::scanExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    _scanExp_2(node->getChild(0), globalTable, localTable, result);
}

void TreeScanner::scanWhile_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    InterCodeOperand *dst_begin = InterCodeOperand::createLiteral("while_" + std::to_string(while_cnt) + "_begin");
    InterCodeOperand *dst_end = InterCodeOperand::createLiteral("while_" + std::to_string(while_cnt) + "_end");
    while_cnt += 1;
    result->add(InterCode(InterCodeOperator::IC_LABEL, dst_begin));
    InterCodeOperand *src1 = terminalToOperand(node->getChild(0), globalTable, localTable, result);
    result->add(InterCode(InterCodeOperator::IC_JZ, dst_end, src1));
    scanStmts_2(node->getChild(1), globalTable, localTable, result);
    result->add(InterCode(InterCodeOperator::IC_JMP, dst_begin));
    result->add(InterCode(InterCodeOperator::IC_LABEL, dst_end));
}

void TreeScanner::scanIf_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    int nChild = node->childCount();
    InterCodeOperand *dst;
    for (int i = 0; i < nChild / 2; i++)
    {
        ASTNode *curExpNode = node->getChild(2 * i);
        InterCodeOperand *src1 = terminalToOperand(curExpNode, globalTable, localTable, result);
        dst = InterCodeOperand::createLiteral("br_" + std::to_string(br_cnt) + "_" + std::to_string(i));
        result->add(InterCode(InterCodeOperator::IC_JNZ, dst, src1));
    }
    dst = nChild % 2 == 1 ? 
          InterCodeOperand::createLiteral("br_" + std::to_string(br_cnt) + "_" + std::to_string(nChild / 2)) : 
          InterCodeOperand::createLiteral("br_" + std::to_string(br_cnt) + "_end");
    result->add(InterCode(InterCodeOperator::IC_JMP, dst));
    for (int i = 0; i < nChild / 2; i++)
    {
        dst = InterCodeOperand::createLiteral("br_" + std::to_string(br_cnt) + "_" + std::to_string(i));
        result->add(InterCode(InterCodeOperator::IC_LABEL, dst));
        scanStmts_2(node->getChild(2 * i + 1), globalTable, localTable, result);
        dst = InterCodeOperand::createLiteral("br_" + std::to_string(br_cnt) + "_end");
        result->add(InterCode(InterCodeOperator::IC_JMP, dst));
    }
    if (nChild % 2 == 1)
    {
        dst = InterCodeOperand::createLiteral("br_" + std::to_string(br_cnt) + "_" + std::to_string(nChild / 2));
        result->add(InterCode(InterCodeOperator::IC_LABEL, dst));
        _scanExp_2(node->getLastChild(), globalTable, localTable, result);
    }
    dst = InterCodeOperand::createLiteral("br_" + std::to_string(br_cnt) + "_end");
    result->add(InterCode(InterCodeOperator::IC_LABEL, dst));
    br_cnt += 1;
}

void TreeScanner::scanReturn_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    if (node->childCount() == 0)
    {
        result->add(InterCode(InterCodeOperator::IC_RET));
    }
    else
    {
        InterCodeOperand *dst = terminalToOperand(node->getChild(0), globalTable, localTable, result);
        result->add(InterCode(InterCodeOperator::IC_RET, dst));
    }
}

InterCodeOperand *TreeScanner::terminalToOperand(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    InterCodeOperand *res = __terminalToOperand(node, localTable);
    if (!res)
    {
        _scanExp_2(node, globalTable, localTable, result);
        res = InterCodeOperand::createTemp(0, 0);
    }
}

InterCodeOperand *TreeScanner::__terminalToOperand(ASTNode *node, SymbolTable &localTable)
{
    if (node->childCount() != 1)
        return nullptr;
    ASTNode *t = node->getChild(0);
    if (t->is(literal))
    {
        if (t->getChild(0)->is(INT_LITERAL))
            return InterCodeOperand::createLiteral(t->getChild(0)->getIntValue());
        else if (t->getChild(0)->is(DOUBLE_LITERAL))
            return InterCodeOperand::createLiteral(t->getChild(0)->getDoubleValue());
        else if (t->getChild(0)->is(STRING_LITERAL))
            return InterCodeOperand::createLiteral(t->getChild(0)->getStringValue());
    }
    else if (t->is(lexp) && t->childCount() == 1)
    {
        string id = t->getChild(0)->getStringValue();
        return InterCodeOperand::createVar(id, localTable.getItem(id)->getType());
    }
    return nullptr;
}

void TreeScanner::scanCallExp_2(ASTNode *node, SymbolTable &globalTable, SymbolTable &localTable, InterCodeList *result)
{
    string id = node->getChild(0)->getStringValue();
    InterCodeOperand *dst = InterCodeOperand::createVar(id, localTable.getItem(id)->getType() & ~SymbolType::FUNC_S);
    SymbolTableItem *item = globalTable.getItem(id);
    if (item == nullptr)
    {
        errorMsgs.push_back("Function " + id + " does not exist!");
        return;
    }
    int paraNum = item->getExtraInfo();
    ASTNode *argsNode = node->getChild(1);
    if (paraNum != argsNode->childCount())
    {
        errorMsgs.push_back("Function " + id + " requires " + std::to_string(paraNum) + 
            " parameters, but " + std::to_string(argsNode->childCount()) + " were given!");
        return;
    }
    for (int i = paraNum - 1; i >= 0; i--)   // 倒序压参数
    {
        ASTNode *curChild = argsNode->getChild(i);
        InterCodeOperand *dst_ = terminalToOperand(curChild, globalTable, localTable, result);
        result->add(InterCode(InterCodeOperator::IC_ARG, dst_));
    }
    InterCodeOperand *src1 = InterCodeOperand::createLiteral(paraNum);
    result->add(InterCode(InterCodeOperator::IC_CALL, dst, src1));
}

bool TreeScanner::checkOperandType(InterCodeOperand *src1, InterCodeOperand *src2, InterCodeOperator optr)
{

}

VarType TreeScanner::inferDstType(InterCodeOperand *src1, InterCodeOperand *src2, InterCodeOperator optr)
{

}