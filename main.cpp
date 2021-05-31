#include <stdio.h>
#include "utility.h"
#include "AST.h"
#include "TreeScanner.h"


extern FILE* yyin;
int yyparse();

extern AST tree;

int main()
{
    // ASTNode *p0 = createNormalNode(0);
    // ASTNode *p1 = createNormalNode(1);
    // ASTNode *p2 = createNormalNode(2);
    // ASTNode *p3 = createNormalNode(3);
    // ASTNode *p4 = createNormalNode(4);
    // ASTNode *p5 = createNormalNode(5);
    // ASTNode *p6 = createNormalNode(6);
    // ASTNode *p7 = createNormalNode(7);
    // ASTNode *p8 = createNormalNode(8);
    // p0->child = p1;
    // p1->sibling = p2;
    // p2->sibling = p3;
    // p2->child = p4;
    // p4->sibling = p5;
    // p5->sibling = p6;
    // p6->sibling = p7;
    // p3->child = p8;
    // printLayerorder(p0);

    yyin = fopen("test.t", "r");
    printf("start parsing\n");
    if (yyparse() != 0)
        return 0;
    printf("Success!\n");
    tree.printDirectory();
    SymbolTable *symbolTable = TreeScanner::getScanner().firstScan(tree);
    symbolTable->print();
}