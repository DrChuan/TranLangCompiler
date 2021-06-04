#include <stdio.h>
#include "utility.h"
#include "AST.h"
#include "TreeScanner.h"
#include "AsmGenerator/AsmGenerator.h"

extern FILE* yyin;
int yyparse();

extern AST tree;

int main()
{
    // 打开源代码文件
    yyin = fopen("test.t", "r");
    // 执行词法分析和语法分析
    printf("start parsing\n");
    if (yyparse() != 0)
        return 0;
    printf("Parsing success!\n");
    // 打印语法树
    tree.printDirectory();
    // 扫描语法树生成符号表
    SymbolTable *symbolTable = TreeScanner::getScanner().firstScan(tree);
    symbolTable->print();
    // 扫描语法树生成中间代码
    InterCodeList *interCodeList = TreeScanner::getScanner().secondScan(tree, *symbolTable);
    interCodeList->print();
    // 打印语法树扫描过程中的错误信息
    if (TreeScanner::getScanner().hasError())
        TreeScanner::getScanner().printErrors();
    else
        cout << "Intermediate code generating success!" << std::endl;
    // 生成汇编代码
    AsmGenerator::getGenerator().generateAsm(cout, *symbolTable, *interCodeList);   
}