#ifndef __UTILITY__
#define __UTILITY__

#include <stdio.h>
#include <stdarg.h>

typedef enum {
    INT_L, DOUBLE_L, STRING_L, IDENTIFIER, NOTHING
} ValueType;

typedef struct {
    union {
        int ival;
        double dval;
        char *sval;
    } val;
    ValueType type;
} Value;

struct Node {
    int symbol;
    Value val;
    struct Node *child;
    struct Node *sibling;
};

typedef struct Node ASTNode; 

ASTNode *createNormalNode(int symbol, int nChild, ...);
ASTNode *createIntNode(int symbol, int val, int nChild, ...);
ASTNode *createDoubleNode(int symbol, double val, int nChild, ...);
ASTNode *createStringNode(int symbol, char* val, int nChild, ...);
ASTNode *createIDNode(int symbol, char* val, int nChild, ...);
void addChild(ASTNode *parent, ASTNode *child);

void printPreorder(ASTNode *root);
void printInorder(ASTNode *root);
void printLayerorder(ASTNode *root);


#endif