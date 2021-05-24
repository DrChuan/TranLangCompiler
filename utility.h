#ifndef __UTILITY__
#define __UTILITY__

#include <stdio.h>
#include <stdarg.h>

// enum ValueType
// 结点中储存的值的类型。包括整形字面量、双精度浮点字面量、字符串字面量、标识符，以及没有值
typedef enum {
    INT_L, DOUBLE_L, STRING_L, IDENTIFIER, NOTHING
} ValueType;

// Value
// 结点中的值，包括一个union和一个指示union中类型的枚举量
typedef struct {
    union {
        int ival;
        double dval;
        char *sval;
    } val;
    ValueType type;
} Value;

// ASTNode
// 抽象语法树结点结构，包含符号信息、值、最左孩子和右兄弟结点
struct Node {
    int symbol;
    Value val;
    struct Node *child;
    struct Node *sibling;
};
typedef struct Node ASTNode; 

// 抽象语法树外部接口
ASTNode *createNormalNode(int symbol, int nChild, ...);
ASTNode *createIntNode(int symbol, int val, int nChild, ...);
ASTNode *createDoubleNode(int symbol, double val, int nChild, ...);
ASTNode *createStringNode(int symbol, char* val, int nChild, ...);
ASTNode *createIDNode(int symbol, char* val, int nChild, ...);
// 抽象语法树内部函数
void addChild(ASTNode *parent, ASTNode *child);
// 抽象语法树打印函数
void printPreorder(ASTNode *root);
void printInorder(ASTNode *root);
void printLayerorder(ASTNode *root);
void printDirectory(ASTNode *root);

#endif