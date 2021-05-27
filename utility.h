#ifndef __UTILITY__
#define __UTILITY__

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>

using std::string;
using std::cout;
using std::vector;

// enum ValueType
// 结点中储存的值的类型。包括整形字面量、双精度浮点字面量、字符串字面量、标识符，以及没有值
enum ValueType{
    INT_L, DOUBLE_L, STRING_L, IDENTIFIER, NOTHING
};

// Value
// 结点中的值，包括一个union和一个指示union中类型的枚举量
class Value {
public:
    Value() {}
    union U {
    public:
        U() { new (&sval) string; }
        ~U() { sval.~string(); }
        int ival;
        double dval;
        string sval;
    } val;
    ValueType type;
};

class ASTNode
{
public:
    ASTNode(int symbol, vector<ASTNode*> children);
    ASTNode(int symbol, int val, vector<ASTNode*> children);
    ASTNode(int symbol, double val, vector<ASTNode*> children);
    ASTNode(int symbol, string val, vector<ASTNode*> children, bool isID=false);
    int symbol;
    Value val;
    ASTNode* child;
    ASTNode* sibling;
};

class AST
{
public:

private:
    ASTNode *root;
};


vector<ASTNode *> generateVector(int n, ...);


// ASTNode
// 抽象语法树结点结构，包含符号信息、值、最左孩子和右兄弟结点
// struct Node {
//     int symbol;
//     Value val;
//     struct Node *child;
//     struct Node *sibling;
// };
// typedef struct Node ASTNode; 

// 抽象语法树外部接口
ASTNode *createNormalNode(int symbol, int nChild, ...);
ASTNode *createIntNode(int symbol, int val, int nChild, ...);
ASTNode *createDoubleNode(int symbol, double val, int nChild, ...);
ASTNode *createStringNode(int symbol, string, int nChild, ...);
ASTNode *createIDNode(int symbol, string val, int nChild, ...);
// 抽象语法树内部函数
void addChild(ASTNode *parent, ASTNode *child);
// 抽象语法树打印函数
void printPreorder(ASTNode *root);
void printInorder(ASTNode *root);
void printLayerorder(ASTNode *root);
void printDirectory(ASTNode *root);

typedef struct//通常这里面每个成员，每次只会使用其中一个，一般是定义成union以节省空间(但这里用了string等复杂类型造成不可以)  
{  
    ASTNode* node;
} YYSTYPE;

#endif