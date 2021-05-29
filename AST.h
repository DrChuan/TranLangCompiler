#ifndef __AST__
#define __AST__

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "utility.h"

using std::string;
using std::cout;
using std::vector;

class ASTNode
{
public:
    ASTNode(int symbol);
    ASTNode(int symbol, vector<ASTNode*> children);
    ASTNode(int symbol, int val, vector<ASTNode*> children);
    ASTNode(int symbol, double val, vector<ASTNode*> children);
    ASTNode(int symbol, string val, vector<ASTNode*> children, bool isID=false);
    void printDirectory(int depth);
    ASTNode *simplify();
    int symbol;
    Value val;
    ASTNode* child;
    ASTNode* sibling;
private:
    static const string non_terminals[];
    static const string terminals[];
    static const int bias = 258;
    void print();
    void merge();
    void addChild(ASTNode *child);
};

class AST
{
public:
    AST() {}
    AST(ASTNode *root) : root(root) {}
    void printDirectory();
    ASTNode *getRoot() const { return root; }
private:
    ASTNode *root;
};

vector<ASTNode *> generateVector(int n, ...);

#endif